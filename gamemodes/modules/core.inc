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
    Module: Core Mechanics
    Health, Collision, Basic Needs.
*/

#include <open.mp>

stock ProcessVehicleCore(playerid)
{
    if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new veh = GetPlayerVehicleID(playerid);
        new Float:health;
        GetVehicleHealth(veh, health);
        
        // Auto-Fix (Godmode)
        // Repair visual damage and engine health
        if(health < 990.0)
        {
            RepairVehicle(veh);
            SetVehicleHealth(veh, 1000.0);
            
            // Optional: Notify or Sound? Nah, silent repair is smoother for drift servers.
        }
        
        // Note: RepairVehicle fixes visuals (panels/doors) too.
        // If we want to fix Visuals aggressively (every frame), it might flicker.
        // Checking < 990 is a good threshold (minor bump).
    }
}

stock SetCollisionMode(playerid, bool:ghost)
{
    new veh = GetPlayerVehicleID(playerid);
    if(veh)
    {
        if(ghost) SetVehicleParamsEx(veh, 1, 1, 1, 1, 1, 1, 0); // Last param objective? No.
        // OMP/SAMP collision toggle per vehicle is tricky without plugins or DisableRemoteVehicleCollisions
        // DisableRemoteVehicleCollisions(playerid, 1); // This works for the player vs others
    }
    
    // Placeholder: OMP native 'SetVehicleParamsCarWindows' etc don't control collision directly.
    // 'DisableRemoteVehicleCollisions' is the standard way.
}
