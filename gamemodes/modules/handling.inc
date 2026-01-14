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
    Module: Handling System (Realism Update)
    Physics Simulation & Presets.
*/

#include <open.mp>

// Preset Definitions
#define HANDLING_TOUGE     0 // Standard / Default
#define HANDLING_PRO       1 // D1GP (Heavy/Snappy)
#define HANDLING_WET       2 // Snow (Floaty/Slippery)
#define HANDLING_DRAG      3 // Drag (Accel Boost)
#define HANDLING_MOON      4 // Fun (Old Fun)

new gPlayerHandlingMode[MAX_PLAYERS];

stock ShowHandlingMenu(playerid)
{
    new str[256];
    format(str, sizeof(str), "Mode\tDescription");
    format(str, sizeof(str), "%s\nTouge (Default)\tBalanced for Technical Tracks", str);
    format(str, sizeof(str), "%s\nPro Drift (D1GP)\tHeavy & Snappy (High Angle)", str);
    format(str, sizeof(str), "%s\nWet / Snow\tSlippery & Floaty (Long Slides)", str);
    format(str, sizeof(str), "%s\nDrag Racing\tLinear Acceleration Boost", str);
    format(str, sizeof(str), "%s\nMoon Gravity\tFun Mode (Low Gravity)", str);
    
    ShowPlayerDialog(playerid, 700, DIALOG_STYLE_TABLIST_HEADERS, "Handling Configuration", str, "Apply", "Cancel");
    return 1;
}

stock ProcessHandlingDialog(playerid, response, listitem)
{
    if(!response) return 1;
    
    gPlayerHandlingMode[playerid] = listitem;
    
    new veh = GetPlayerVehicleID(playerid);
    new msg[64];
    
    switch(listitem)
    {
        case HANDLING_TOUGE: 
        {
            if(veh) {} // SetVehicleGravity(veh, 0.008); // SA Default
            format(msg, sizeof(msg), "Handling: Touge (Standard)");
        }
        case HANDLING_PRO: 
        {
            if(veh) {} // SetVehicleGravity(veh, 0.015); // Heavy
            format(msg, sizeof(msg), "Handling: Pro Drift (D1GP)");
        }
        case HANDLING_WET: 
        {
            if(veh) {} // SetVehicleGravity(veh, 0.005); // Floaty
            format(msg, sizeof(msg), "Handling: Wet/Snow Mode");
        }
        case HANDLING_DRAG: 
        {
            if(veh) {} // SetVehicleGravity(veh, 0.008);
            format(msg, sizeof(msg), "Handling: Drag Mode (Boost Active)");
        }
        case HANDLING_MOON: 
        {
            if(veh) {} // SetVehicleGravity(veh, 0.002); // Very Low
            format(msg, sizeof(msg), "Handling: Moon Gravity");
        }
    }
    
    ShowNotification(playerid, "Handling", msg, 3000);
    return 1;
}

stock ProcessHandlingPhysics(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid)) return;
    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return;
    
    new mode = gPlayerHandlingMode[playerid];
    new veh = GetPlayerVehicleID(playerid);
    
    // Continuous Physics Logic
    if(mode == HANDLING_DRAG)
    {
        new keys, ud, lr;
        GetPlayerKeys(playerid, keys, ud, lr);
        
        // If holding gas (KEY_SPRINT/KEY_ACTION usually or just Analog Up)
        // Check velocity
        if(keys & KEY_CROUCH) // Horn/Sprint to boost? Or just auto-boost?
        {
            // Let's use Horn/Crouch button for "Launch Control" or Boost
            // Or just multiply existing velocity if moving forward
            new Float:vx, Float:vy, Float:vz;
            GetVehicleVelocity(veh, vx, vy, vz);
            new Float:speed = floatsqroot(vx*vx + vy*vy + vz*vz);
            
            if(speed > 0.1 && speed < 3.0) // Cap max speed to avoid flying
            {
               SetVehicleVelocity(veh, vx*1.02, vy*1.02, vz); // 2% boost per frame
            }
        }
    }
    else if(mode == HANDLING_PRO)
    {
         // if(GetVehicleGravity(veh) < 0.014) SetVehicleGravity(veh, 0.015);
    }
    
    // Phase 14: Wet Weather Physics (Hydroplaning)
    // Defined in weather.inc
    if(IsWeatherWet(gGameWeather))
    {
        new Float:vx, Float:vy, Float:vz;
        GetVehicleVelocity(veh, vx, vy, vz);
        new Float:speed = floatsqroot(vx*vx + vy*vy + vz*vz) * 180.0;
        
        if(speed > 40.0) // Only at speed
        {
            // If turning (Side velocity high?)
            // Or just boost slightly to reduce friction friction
            // 0.5% boost per tick to counteract SA friction
            SetVehicleVelocity(veh, vx*1.005, vy*1.005, vz);
        }
    }
}
