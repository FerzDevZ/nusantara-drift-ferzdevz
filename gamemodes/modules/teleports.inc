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
    Module: Teleport System
    Instant travel to drift spots.
*/

#include <open.mp>

#define DIALOG_TELES 400

stock ShowTeleportMenu(playerid)
{
    new str[256];
    format(str, sizeof(str), "Location\tDescription\n");
    format(str, sizeof(str), "%sLS Airport (Spawn)\tMain Drift Area\n", str);
    format(str, sizeof(str), "%sOcean Docks\tIndustrial Drift\n", str);
    format(str, sizeof(str), "%sSanta Maria Beach\tScenic Route\n", str);
    format(str, sizeof(str), "%sMount Chilliad\tDownhill Touge (Offroad)\n", str);
    format(str, sizeof(str), "%sSan Fierro Airport\tWide Open Space\n", str);
    
    ShowPlayerDialog(playerid, DIALOG_TELES, DIALOG_STYLE_TABLIST_HEADERS, "Teleport Menu", str, "Go", "Cancel");
    return 1;
}

stock ProcessTeleportDialog(playerid, response, listitem)
{
    if(!response) return 1;
    
    new Float:x, Float:y, Float:z, Float:a;
    new location_name[32];
    
    switch(listitem)
    {
        case 0: // LS Airport
        {
            x = -1426.6833; y = -256.0206; z = 14.1484; a = 0.0;
            format(location_name, 32, "LS Airport");
        }
        case 1: // Ocean Docks
        {
            x = 2779.6423; y = -2455.4382; z = 13.6346; a = 90.0;
             format(location_name, 32, "Ocean Docks");
        }
        case 2: // Santa Maria
        {
            x = 369.8322; y = -2030.5638; z = 7.6719; a = 0.0;
             format(location_name, 32, "Santa Maria");
        }
        case 3: // Chilliad
        {
            x = -2331.4363; y = -1674.8080; z = 483.5658; a = 0.0; // Midway up
             format(location_name, 32, "Mt. Chilliad");
        }
        case 4: // SF Airport
        {
            x = -1542.4921; y = -426.7570; z = 6.0000; a = 90.0; // SF Airport runway
             format(location_name, 32, "SF Airport");
        }
    }
    
    // Teleport Logic
    if(IsPlayerInAnyVehicle(playerid))
    {
        new veh = GetPlayerVehicleID(playerid);
        SetVehiclePos(veh, x, y, z);
        SetVehicleZAngle(veh, a);
        RepairVehicle(veh);
        SetVehicleVirtualWorld(veh, 0);
        LinkVehicleToInterior(veh, 0);
    }
    else
    {
        SetPlayerPos(playerid, x, y, z);
        SetPlayerFacingAngle(playerid, a);
    }
    
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerInterior(playerid, 0);
    
    new msg[64];
    format(msg, sizeof(msg), "Teleported to %s!", location_name);
    ShowNotification(playerid, "Travel", msg, 3000);
    return 1;
}
