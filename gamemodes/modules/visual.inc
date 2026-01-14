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
    Module: Dynamic Sticker System
    Livery & Visual Identity.
*/

#include <open.mp>

#define MAX_STICKERS 5
new gVehicleStickers[MAX_VEHICLES][MAX_STICKERS]; // Stores Object IDs
new gEditingStickerSlot[MAX_PLAYERS]; // -1 if not editing
new bool:gIsEditing[MAX_PLAYERS];
new Float:gStickerPos[MAX_VEHICLES][MAX_STICKERS][3]; // X, Y, Z offsets
new gStickerText[MAX_VEHICLES][MAX_STICKERS][64];
new gVehicleNeon[MAX_VEHICLES][2]; // Stores Neon Object IDs

// Default Offset (Door side usually)
#define STICKER_X 1.1
#define STICKER_Y 0.0
#define STICKER_Z 0.0

stock CreateSticker(playerid, text[])
{
    new veh = GetPlayerVehicleID(playerid);
    if(!veh) return SendClientMessage(playerid, -1, "Get in a car.");
    
    // Find free slot
    new slot = -1;
    for(new i=0; i<MAX_STICKERS; i++)
    {
        if(gVehicleStickers[veh][i] == 0)
        {
            slot = i;
            break;
        }
    }
    
    if(slot == -1) return SendClientMessage(playerid, -1, "Max 5 stickers per car.");
    
    // Create Object (19477 = White Plate, small)
    // Or 19482 (Sphere)? 19477 is flat.
    // Ideally user 19325 (Glass) for transparent background?
    // Let's use 19477 for visibility now.
    
    gVehicleStickers[veh][slot] = CreateObject(19482, 0,0,0,0,0,0); 
    
    // Set Text
    // MaterialIndex 0
    // MaterialSize 256x128
    // Font "Arial"
    // FontSize 24
    // Bold 1
    // FontColor White
    // BackColor Transparent (0 for ARGB is full transparent? No, 0x00000000)
    // Align Center
    SetObjectMaterialText(gVehicleStickers[veh][slot], text, 0, 90, "Arial", 28, true, 0xFFFFFFFF, 0x00000000, 1);
    
    // Initial Attachment
    AttachObjectToVehicle(gVehicleStickers[veh][slot], veh, STICKER_X, STICKER_Y, STICKER_Z, 0, 0, 0);
    
    new msg[64];
    format(msg, sizeof(msg), "Sticker '%s' created in slot %d.", text, slot+1);
    SendClientMessage(playerid, -1, msg);
    
    // Store text for persistence
    format(gStickerText[veh][slot], 64, "%s", text);
    
    // Auto-enter edit mode
    gEditingStickerSlot[playerid] = slot;
    gIsEditing[playerid] = true;
    SendClientMessage(playerid, 0xFFFF00FF, "EDITOR MODE: Use Arrow Keys (UD/LR) to move. Press SPACE to Save.");
    
    return 1;
}

stock ProcessStickerEditor(playerid)
{
    if(!gIsEditing[playerid]) return;
    
    new veh = GetPlayerVehicleID(playerid);
    if(!veh)
    {
        gIsEditing[playerid] = false;
        return;
    }
    
    new keys, ud, lr;
    GetPlayerKeys(playerid, keys, ud, lr);
    
    new slot = gEditingStickerSlot[playerid];
    new obj = gVehicleStickers[veh][slot];
    
    if(!obj) return; // Error
    
    // Get Current Offset?
    // We can't easily 'GetAttachedObjectOffset'. We must track it in variables.
    // For prototype simplicity, we assume we track it or just nudge it blindly?
    // Nudge blindly doesn't work well.
    // Let's use PVar for current X Y Z
    
    new Float:x, Float:y, Float:z;
    if(GetPVarType(playerid, "StickerX") == 0) // Init
    {
        x = STICKER_X; y = STICKER_Y; z = STICKER_Z;
        SetPVarFloat(playerid, "StickerX", x);
        SetPVarFloat(playerid, "StickerY", y);
        SetPVarFloat(playerid, "StickerZ", z);
    }
    else
    {
        x = GetPVarFloat(playerid, "StickerX");
        y = GetPVarFloat(playerid, "StickerY");
        z = GetPVarFloat(playerid, "StickerZ");
    }
    
    new bool:update = false;
    new Float:speed = 0.05;
    // Analog Support (UD/LR can be > 0 or < 0)
    // Up/Down controls Z (Height)
    if(ud > 0) { z += speed; update = true; }
    else if(ud < 0) { z -= speed; update = true; }
    
    // Left/Right controls Y (Side/Length)
    if(lr > 0) { y += speed; update = true; } // Right positive
    else if(lr < 0) { y -= speed; update = true; } // Left negative
    
    // Switch Axis? (Maybe Sprint/Space for Z?)
    // Let's keep it simple: UD=Z, LR=Y. X needs another key.
    // Maybe Fire/Action for X?
    
    if(keys & KEY_SPRINT)
    {
        // Finish
        gIsEditing[playerid] = false;
        
        // Save to Global Array for persistence logic
        gStickerPos[veh][slot][0] = x;
        gStickerPos[veh][slot][1] = y;
        gStickerPos[veh][slot][2] = z;
        
        SendClientMessage(playerid, -1, "Sticker Saved. Select 'Save Current Mods' to keep it permanently.");
        return;
    }
    
    if(update)
    {
        SetPVarFloat(playerid, "StickerX", x);
        SetPVarFloat(playerid, "StickerY", y);
        SetPVarFloat(playerid, "StickerZ", z);
        AttachObjectToVehicle(obj, veh, x, y, z, 0, 0, 0);
    }
}

stock SaveStickers(vehicleid, db_id)
{
    // Clear old stickers for this veh in DB
    new query[128];
    format(query, sizeof(query), "DELETE FROM vehicle_stickers WHERE veh_db_id = %d", db_id);
    db_free_result(db_query(gDB, query));
    
    for(new i=0; i<MAX_STICKERS; i++)
    {
        new obj = gVehicleStickers[vehicleid][i];
        if(obj)
        {
            new Float:x = gStickerPos[vehicleid][i][0];
            new Float:y = gStickerPos[vehicleid][i][1];
            new Float:z = gStickerPos[vehicleid][i][2];
            
            format(query, sizeof(query), "INSERT INTO vehicle_stickers (veh_db_id, slot, text, x, y, z, rx, ry, rz) VALUES (%d, %d, '%s', %f, %f, %f, 0, 0, 0)",
                db_id, i, gStickerText[vehicleid][i], x, y, z);
            db_free_result(db_query(gDB, query));
        }
    }
    return 1;
}



stock LoadStickers(vehicleid, db_id)
{
    new query[128];
    format(query, sizeof(query), "SELECT * FROM vehicle_stickers WHERE veh_db_id = %d", db_id);
    new DBResult:res = db_query(gDB, query);
    
    if(db_num_rows(res))
    {
        for(new i=0; i<db_num_rows(res); i++)
        {
            new slot = db_get_field_assoc_int(res, "slot");
            new text[64];
            db_get_field_assoc(res, "text", text, 64);
            new Float:x = db_get_field_assoc_float(res, "x");
            new Float:y = db_get_field_assoc_float(res, "y");
            new Float:z = db_get_field_assoc_float(res, "z");
            
            // Recreate
            if(slot >= 0 && slot < MAX_STICKERS)
            {
                gVehicleStickers[vehicleid][slot] = CreateObject(19482, 0,0,0,0,0,0);
                SetObjectMaterialText(gVehicleStickers[vehicleid][slot], text, 0, 90, "Arial", 28, true, 0xFFFFFFFF, 0x00000000, 1);
                AttachObjectToVehicle(gVehicleStickers[vehicleid][slot], vehicleid, x, y, z, 0, 0, 0);
                format(gStickerText[vehicleid][slot], 64, "%s", text);
                gStickerPos[vehicleid][slot][0] = x;
                gStickerPos[vehicleid][slot][1] = y;
                gStickerPos[vehicleid][slot][2] = z;
            }
            db_next_row(res);
        }
    }
    db_free_result(res);
    return 1;
}
stock CleanupVehicleVisuals(vehicleid)
{
    // Clean Stickers
    for(new i=0; i<MAX_STICKERS; i++)
    {
        if(gVehicleStickers[vehicleid][i])
        {
            DestroyObject(gVehicleStickers[vehicleid][i]);
            gVehicleStickers[vehicleid][i] = 0;
        }
    }
    
    // Clean Neon
    for(new i=0; i<2; i++)
    {
        if(gVehicleNeon[vehicleid][i])
        {
            DestroyObject(gVehicleNeon[vehicleid][i]);
            gVehicleNeon[vehicleid][i] = 0;
        }
    }
    return 1;
}
