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
    Module: Ultimate Tuning Shop
    Pimp My Ride.
*/

#include <open.mp>

#define DIALOG_TUNING_MAIN 800
#define DIALOG_TUNING_SPOILERS 801
#define DIALOG_TUNING_WHEELS   802
#define DIALOG_TUNING_NITRO    803
#define DIALOG_TUNING_NEON     804

stock ShowTuningMenu(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "Get in a car first!");
    
    ShowPlayerDialog(playerid, DIALOG_TUNING_MAIN, DIALOG_STYLE_LIST, "Ultimate Mod Shop", "Spoilers (Alien/X-Flow)\nWheels (High End/Sport)\nNitro (2x/5x/10x)\nNeon Lights (Underglow)\nSave Current Mods", "Select", "Exit");
    return 1;
}

stock ProcessTuningDialog(playerid, dialogid, response, listitem)
{
    if(!response) return 1;
    new veh = GetPlayerVehicleID(playerid);
    
    if(dialogid == DIALOG_TUNING_MAIN)
    {
        switch(listitem)
        {
            case 0: // Spoilers
            {
                // Component Slot 2 (Spoiler). IDs: 1000-1003, 1014-1016, 1023, 1049, 1050, 1058, 1060, 1138, 1139, 1146, 1147, 1158, 1162, 1163, 1164...
                // Only showing generic ones compatible with most cars (Elegy/Sultan/Flash etc have specific ones)
                // Elegy: 1146, 1147. Sultan: 1138, 1139.
                // For "Ultimate", we list options generic or specific?
                // For simplicity, let's list "Alien" (1138/1146) and "X-Flow" (1139/1147). 
                // We need to check car model.
                
                ShowPlayerDialog(playerid, DIALOG_TUNING_SPOILERS, DIALOG_STYLE_LIST, "Spoilers", "Alien Spoiler ($2000)\nX-Flow Spoiler ($2000)\nStock (Remove)", "Buy", "Back");
            }
            case 1: // Wheels
            {
                // Offroad, Shadow, Mega, Rimshine, Wires...
                ShowPlayerDialog(playerid, DIALOG_TUNING_WHEELS, DIALOG_STYLE_LIST, "Wheels", "Shadow ($1000)\nMega ($1000)\nRimshine ($1000)\nWires ($1000)\nClassic ($1000)\nSwitch ($1000)", "Buy", "Back");
            }
            case 2: // Nitro
            {
                ShowPlayerDialog(playerid, DIALOG_TUNING_NITRO, DIALOG_STYLE_LIST, "Nitro", "2x Nitrous ($500)\n5x Nitrous ($1000)\n10x Nitrous ($2000)", "Buy", "Back");
            }
            case 3: // Neon
            {
                ShowPlayerDialog(playerid, DIALOG_TUNING_NEON, DIALOG_STYLE_LIST, "Neon Underglow", "Red Neon\nBlue Neon\nGreen Neon\nWhite Neon\nPink Neon\nYellow Neon\nRemove Neon", "Buy", "Back");
            }
            case 4: // Save
            {
                SaveVehicleMods(playerid);
            }
        }
    }
    // ... [Same Spoiler/Wheel/Nitro blocks] ... 
    // Wait, I must include them or `replace` will cut them? 
    // I am updating ProcessTuningDialog to include Main + Neon.
    // I need to provide full function body for safer replacement or partial?
    // Partial is risky if I can't target cleanly.
    // But `ProcessTuningDialog` is large.
    // I'll append the Neon block at the end (before return) and update Main switch.
    // Let's do huge replacement for safety.
    
    else if(dialogid == DIALOG_TUNING_SPOILERS)
    {
        // ... (Keep existing)
        new model = GetVehicleModel(veh);
        new component = 0;
        if(listitem == 0) 
        {
            if(model==562) component=1146; else if(model==560) component=1138; else if(model==559) component=1060; else if(model==558) component=1093; else if(model==565) component=1050; 
            else return SendClientMessage(playerid, -1, "Not compatible.");
        }
        else if(listitem == 1) 
        {
             if(model==562) component=1147; else if(model==560) component=1139; else if(model==559) component=1058; else if(model==558) component=1089; else if(model==565) component=1049; 
             else return SendClientMessage(playerid, -1, "Not compatible.");
        }
        else if(listitem == 2)
        {
             new current = GetVehicleComponentInSlot(veh, 2);
             if(current) RemoveVehicleComponent(veh, current);
             SaveVehicleMods(playerid);
             return 1;
        }
        
        if(component) { AddVehicleComponent(veh, component); SaveVehicleMods(playerid); SendClientMessage(playerid, -1, "Installed!"); }
    }
    else if(dialogid == DIALOG_TUNING_WHEELS)
    {
        new wheels[] = {1073, 1074, 1075, 1076, 1077, 1080}; 
        if(listitem < sizeof(wheels)) { AddVehicleComponent(veh, wheels[listitem]); SaveVehicleMods(playerid); SendClientMessage(playerid, -1, "Installed!"); }
    }
    else if(dialogid == DIALOG_TUNING_NITRO)
    {
        new component = 0;
        if(listitem == 0) component = 1009; 
        if(listitem == 1) component = 1008; 
        if(listitem == 2) component = 1010; 
        AddVehicleComponent(veh, component); SaveVehicleMods(playerid); SendClientMessage(playerid, -1, "Installed!");
    }
    else if(dialogid == DIALOG_TUNING_NEON)
    {
        new objid = 0;
        if(listitem == 0) objid = 18647; // Red
        else if(listitem == 1) objid = 18648; // Blue
        else if(listitem == 2) objid = 18649; // Green
        else if(listitem == 3) objid = 18652; // White
        else if(listitem == 4) objid = 18651; // Pink
        else if(listitem == 5) objid = 18650; // Yellow
        else if(listitem == 6) // Remove
        {
             SetPVarInt(playerid, "ActiveNeonModel", 0);
             CleanupVehicleVisuals(veh);
             SendClientMessage(playerid, -1, "Neon Removed! Save to apply.");
             return 1;
        }
        
        if(objid)
        {
             // Save for persistence
             SetPVarInt(playerid, "ActiveNeonModel", objid);
             
             // Apply immediately
             ApplyNeon(veh, objid);
             
             SendClientMessage(playerid, -1, "Neon Installed! Select 'Save Current Mods' to keep it permanently.");
        }
    }
    return 1;
}
