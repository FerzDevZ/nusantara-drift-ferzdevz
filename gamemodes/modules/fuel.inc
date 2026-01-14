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
    Module: Fuel System
    Realism for Garage.
*/

#include <open.mp>

new Float:gVehicleFuel[MAX_VEHICLES];

// Init fuel on spawn?
// We need OnVehicleSpawn or similar.
// For now, hook into CreateVehicle logic if possible?
// Or just init lazy.

stock SetFuel(vehicleid, Float:amount)
{
    gVehicleFuel[vehicleid] = amount;
    if(gVehicleFuel[vehicleid] > 100.0) gVehicleFuel[vehicleid] = 100.0;
}

stock ProcessFuelConsumption(playerid)
{
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return;
    
    new veh = GetPlayerVehicleID(playerid);
    if(!veh) return;
    
    // Check Engine Status? 
    // Assuming engine is always on for now unless we implement Engine Control.
    // If moving?
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(veh, vx, vy, vz);
    if((vx*vx + vy*vy) > 0.01) // Moving
    {
        gVehicleFuel[veh] -= 0.005; // Consumption Rate
        
        if(gVehicleFuel[veh] <= 0.0)
        {
            gVehicleFuel[veh] = 0.0;
            // Stop Engine properly preserving other params
            new engine, lights, alarm, doors, bonnet, boot, objective;
            GetVehicleParamsEx(veh, engine, lights, alarm, doors, bonnet, boot, objective);
            SetVehicleParamsEx(veh, 0, lights, alarm, doors, bonnet, boot, objective); 
            
            GameTextForPlayer(playerid, "~r~OUT OF FUEL!", 1000, 3);
        }
    }
}

stock FillFuel(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "Get in a car!");
    
    new veh = GetPlayerVehicleID(playerid);
    new cost = 100; // Drift Points
    
    if(gPlayerData[playerid][total_points] < cost) return SendClientMessage(playerid, -1, "Need 100 Points to refuel!");
    
    gPlayerData[playerid][total_points] -= cost;
    gVehicleFuel[veh] = 100.0;
    
    new msg[64];
    format(msg, sizeof(msg), "Refueled for %d points! Full Tank.", cost);
    SendClientMessage(playerid, 0x00FF00FF, msg);
    
    // Turn engine on if off
    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(veh, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(veh, 1, lights, alarm, doors, bonnet, boot, objective); 
    return 1;
}

// Helper to init fuel to 100 when spawning
stock InitVehicleFuel(vehicleid)
{
    gVehicleFuel[vehicleid] = 100.0;
}
