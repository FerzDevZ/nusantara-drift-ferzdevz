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
    Module: Custom Tire Smoke
    Realism & Style.
*/

#include <open.mp>

// Last smoke timestamp to limit object count
new gLastSmokeTime[MAX_PLAYERS];

stock ProcessTireSmoke(playerid)
{
    if(GetTickCount() - gLastSmokeTime[playerid] < 150) return; // Limit rate (ms)
    
    new veh = GetPlayerVehicleID(playerid);
    if(!veh) return;
    
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(veh, vx, vy, vz);
    new Float:speed = floatsqroot(vx*vx + vy*vy + vz*vz) * 180.0;
    
    if(speed < 30.0) return; // Too slow
    
    // Check Drift Angle?
    // Simplified: If moving sideways?
    // Vector dot product or just check if velocity angle differs from facing angle.
    new Float:heading;
    GetVehicleZAngle(veh, heading);
    new Float:move_angle = atan2(vy, vx); // -180 to 180
    move_angle = move_angle - 90.0; // Correction for GTA coords? Or just test?
    
    // Actually, simpler logic: Input based?
    // If player is holding HANDBRAKE or turning sharply at speed?
    // Let's rely on our drift score var if available? 
    // `GetDriftScore` isn't global.
    // Let's use simple logic: If (Speed > 30) and (Keys & Turn and Keys & Gas/Brake)?
    // Or just "Always on" for testing?
    // Let's use Drift Logic from drift_system if accessible.
    // But drift_system logic is local to that module usually.
    // Let's duplicate simple angle check.
    
    // Just spawning objects at rear wheels for now if speed > 30 and (Keys & Handbrake)
    new keys, ud, lr;
    GetPlayerKeys(playerid, keys, ud, lr);
    
    if(keys & KEY_HANDBRAKE || (speed > 50 && lr != 0))
    {
        gLastSmokeTime[playerid] = GetTickCount();
        
        new Float:x, Float:y, Float:z, Float:a;
        GetVehiclePos(veh, x, y, z);
        GetVehicleZAngle(veh, a);
        
        // Rear Wheel Offsets (approx)
        // Left: -0.9, -1.5, -0.5
        // Right: 0.9, -1.5, -0.5
        // Need to rotate offset by vehicle angle.
        
        // Quick math helper? 
        // Or just spawn near car center for low cost version.
        // Let's try center-rear.
        
        // Offset (-1.5 behind)
        a = 360 - a; // GTA Angle
        x += 1.5 * floatsin(-a, degrees); // Backwards?
        y += 1.5 * floatcos(-a, degrees); 
        z -= 0.5;
        
        // Create Object 18723 (White Smoke)
        // Use PlayerObject to avoid global limit clogs
        
        new obj = CreatePlayerObject(playerid, 18723, x, y, z, 0, 0, 0);
        
        // Destroy after 1 sec (Smoke doesn't need to stay)
        // SetTimerEx? Or Object Material?
        // Actually, just let it exist? OpenMP doesn't auto-destroy.
        // We MUST destroy it.
        // If we spawn 5 per second... 
        // Let's rename function `CreateSmokePuff` and set timer.
        
        SetTimerEx("DestroySmoke", 1000, false, "ii", playerid, obj);
    }
}

forward DestroySmoke(playerid, objectid);
public DestroySmoke(playerid, objectid)
{
    if(IsValidPlayerObject(playerid, objectid))
    {
        DestroyPlayerObject(playerid, objectid);
    }
}
