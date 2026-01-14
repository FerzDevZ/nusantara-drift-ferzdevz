/*
================================================================
    FerzRP - MODERN RUNNER (open.mp)
    ------------------------------------------------------------
    Author      : FerzDevZ
    Base        : FerzGamemodeZero
    Version     : 0.1 (Modular Patch)
    
    Youtube     : Ferzsampp
    Instagram   : ferzchills
    Discord     : ferzdevz
================================================================
*/

/*
    Module: Garage & Ownership
    Buy, Modify, Keep.
*/

#include <open.mp>

#define MAX_OWNED_CARS 50

stock ShowGarageMenu(playerid)
{
    new name[MAX_PLAYER_NAME], query[128];
    GetPlayerName(playerid, name, sizeof(name));
    
    format(query, sizeof(query), "SELECT id, model FROM player_vehicles WHERE owner = '%s'", name);
    new DBResult:res = db_query(gDB, query);
    
    if(db_num_rows(res) == 0)
    {
        SendClientMessage(playerid, -1, "You don't own any vehicles. Use /buycar.");
        db_free_result(res);
        return 1;
    }
    
    new str[512], model, id, modelname[32];
    format(str, sizeof(str), "ID\tVehicle");
    
    for(new i=0; i<db_num_rows(res); i++)
    {
        id = db_get_field_assoc_int(res, "id");
        model = db_get_field_assoc_int(res, "model");
        
        // Simple manual mapping or use a library for Names. Using ID for now.
        format(modelname, 32, "Model %d", model); 
        
        format(str, sizeof(str), "%s\n%d\t%s", str, id, modelname);
        db_next_row(res);
    }
    
    ShowPlayerDialog(playerid, 300, DIALOG_STYLE_TABLIST_HEADERS, "My Garage - Select to Manage", str, "Select", "Close");
    db_free_result(res);
    return 1;
}

stock BuyVehicle(playerid, modelid, cost)
{
    if(gPlayerData[playerid][total_points] < cost) return SendClientMessage(playerid, -1, "Not enough drift points!");
    
    gPlayerData[playerid][total_points] -= cost;
    
    new name[MAX_PLAYER_NAME], query[256];
    GetPlayerName(playerid, name, sizeof(name));
    
    // Default: No mods, colors 1,1
    format(query, sizeof(query), "INSERT INTO player_vehicles (owner, model, components, colors, paintjob, speed_stage) VALUES ('%s', %d, '', '1,1', 3, 0)", name, modelid);
    db_free_result(db_query(gDB, query));
    
    SendClientMessage(playerid, 0x00FF00FF, "Vehicle Purchased! Use /mycars to spawn it.");
    DB_SavePlayer(playerid); // Save points deduction
    return 1;
}

stock SellVehicle(playerid, db_id, modelid)
{
    // Hardcoded resale value estimation (e.g. 50% of generic 20k)
    // Ideally we pass original cost or lookup.
    new refund = 10000; 

    new query[128];
    format(query, sizeof(query), "DELETE FROM player_vehicles WHERE id = %d", db_id);
    db_free_result(db_query(gDB, query));
    
    gPlayerData[playerid][total_points] += refund;
    DB_SavePlayer(playerid);
    
    new msg[64];
    format(msg, sizeof(msg), "Vehicle sold! Refunded %d points.", refund);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

stock StartTestDrive(playerid, modelid)
{
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    
    new veh = CreateVehicle(modelid, x+2.0, y, z, a, 1, 1, -1);
    PutPlayerInVehicle(playerid, veh, 0);
    
    ShowNotification(playerid, "Test Drive", "You have 60 seconds!", 5000);
    
    SetTimerEx("EndTestDrive", 60000, false, "ii", playerid, veh);
    return 1;
}

forward EndTestDrive(playerid, vehicleid);
public EndTestDrive(playerid, vehicleid)
{
    if(!IsPlayerConnected(playerid)) 
    {
        if(IsValidVehicle(vehicleid)) DestroyVehicle(vehicleid);
        return;
    }
    
    if(IsPlayerInVehicle(playerid, vehicleid))
    {
        RemovePlayerFromVehicle(playerid);
        ShowNotification(playerid, "Test Drive", "Time's up!", 3000);
    }
    DestroyVehicle(vehicleid);
}

stock SpawnOwnedVehicle(playerid, db_id)
{
    new query[128];
    format(query, sizeof(query), "SELECT * FROM player_vehicles WHERE id = %d", db_id);
    new DBResult:res = db_query(gDB, query);
    
    if(db_num_rows(res))
    {
        new model = db_get_field_assoc_int(res, "model");
        new Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        
        // Offset simple
        x += 2.0; 
        
        // Destroy previous personal vehicle if exists
        if(gPlayerPersonalVehicle[playerid] && IsValidVehicle(gPlayerPersonalVehicle[playerid]))
        {
            CleanupVehicleVisuals(gPlayerPersonalVehicle[playerid]);
            DestroyVehicle(gPlayerPersonalVehicle[playerid]);
        }
        
        new veh = CreateVehicle(model, x, y, z, a, 1, 1, -1);
        gPlayerPersonalVehicle[playerid] = veh;
        LinkVehicleToInterior(veh, GetPlayerInterior(playerid));
        SetVehicleVirtualWorld(veh, GetPlayerVirtualWorld(playerid));
        
        InitVehicleFuel(veh);
        
        // Apply Mods (TODO: Parse string)
        // For simplicity in Phase 5 prototype: Just spawn base car.
        // Full mod loading requires splitting string "1010,1020" by comma.
        
        PutPlayerInVehicle(playerid, veh, 0);
        SendClientMessage(playerid, -1, "Vehicle Spawned from Garage.");
        
        // Load Mods
        SetPVarInt(playerid, "CurrentVehicleDBID", db_id); // Track which DB ID this car belongs to
        LoadVehicleMods(veh, db_id);
        LoadStickers(veh, db_id);
    }
    db_free_result(res);
    return 1;
}

stock SaveVehicleMods(playerid)
{
    new veh = GetPlayerVehicleID(playerid);
    new db_id = GetPVarInt(playerid, "CurrentVehicleDBID");
    if(!veh || !db_id) return 0;
    
    new mods[256], component;
    
    // Loop through slots (0-13 usually cover visual mods)
    for(new i=0; i<14; i++)
    {
        component = GetVehicleComponentInSlot(veh, i);
        if(component)
        {
            if(strlen(mods) > 0) format(mods, sizeof(mods), "%s,%d", mods, component);
            else format(mods, sizeof(mods), "%d", component);
        }
    }
    
    new query[512];
    format(query, sizeof(query), "UPDATE player_vehicles SET components = '%s', neon_model = %d WHERE id = %d", 
        mods, GetPVarInt(playerid, "ActiveNeonModel"), db_id);
    db_free_result(db_query(gDB, query));
    
    SaveStickers(veh, db_id);
    
    ShowNotification(playerid, "Tuning", "Mods & Visuals Saved!", 2000);
    return 1;
}

stock LoadVehicleMods(vehicleid, db_id)
{
    new query[128];
    format(query, sizeof(query), "SELECT components, neon_model FROM player_vehicles WHERE id = %d", db_id);
    new DBResult:res = db_query(gDB, query);
    
    if(db_num_rows(res))
    {
        new components[256];
        db_get_field_assoc(res, "components", components, sizeof(components));
        new neon = db_get_field_assoc_int(res, "neon_model");
        
        // Parse "1010,1020,..."
        new start = 0, end = -1, val_str[16], val;
        while((end = strfind(components, ",", true, start)) != -1)
        {
             strmid(val_str, components, start, end);
             val = strval(val_str);
             if(val > 1000) AddVehicleComponent(vehicleid, val);
             start = end + 1;
        }
        // Last one
        strmid(val_str, components, start, strlen(components));
        val = strval(val_str);
        if(val > 1000) AddVehicleComponent(vehicleid, val);
        
        // Apply Neon if exists
        if(neon > 0)
        {
             ApplyNeon(vehicleid, neon);
        }
    }
    db_free_result(res);
    return 1;
}

stock ProcessGarageDialog(playerid, response, listitem, inputtext[])
{
    if(!response) return 1;
    
    // Inputtext contains "ID \t Name". We need ID.
    new id = strval(inputtext); 
    
    // Store ID in a var to use in next dialog (hacky but works if single threaded per player)
    // Better: SetPVarInt
    SetPVarInt(playerid, "GarageVehID", id);
    
    // Show Sub-Menu
    ShowPlayerDialog(playerid, 301, DIALOG_STYLE_LIST, "Vehicle Management", "Spawn Vehicle\nSell Vehicle (50% Refund)", "Select", "Back");
    return 1;
}

// Handler for 301 is needed in main script or here if we export it.
stock ProcessGarageAction(playerid, response, listitem)
{
    if(!response) return 1; // Back to list?
    
    new db_id = GetPVarInt(playerid, "GarageVehID");
    
    if(listitem == 0) // Spawn
    {
        SpawnOwnedVehicle(playerid, db_id);
    }
    else if(listitem == 1) // Sell
    {
        SellVehicle(playerid, db_id, 0); // Model ID 0 passed as dummy, logic uses DB
    }
    return 1;
}
