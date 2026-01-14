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
    Module: Drift System
    Calculate drift points based on vehicle velocity and angle
*/

#include <open.mp>

new gDriftScore[MAX_PLAYERS];

stock Float:GetVehicleDriftAngle(vehicleid)
{
    new Float:vx, Float:vy, Float:vz, Float:va;
    GetVehicleVelocity(vehicleid, vx, vy, vz);
    GetVehicleZAngle(vehicleid, va);
    
    new Float:angle = atan2(vy, vx) - 90.0;
    angle = va - angle;
    
    while (angle > 180.0) angle -= 360.0;
    while (angle < -180.0) angle += 360.0;
    
    return floatabs(angle);
}

new bool:gIsTandem[MAX_PLAYERS];

stock ProcessDrift(playerid)
{
    gIsTandem[playerid] = false; // Reset
    
    if (!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) 
    {
        if (gDriftScore[playerid] > 0)
        {
            if (gDriftScore[playerid] > gPlayerData[playerid][best_drift])
                gPlayerData[playerid][best_drift] = gDriftScore[playerid];
            
            gPlayerData[playerid][total_points] += gDriftScore[playerid];
            DB_SavePlayer(playerid);
        }
        gDriftScore[playerid] = 0;
        return 0;
    }

    new vehicleid = GetPlayerVehicleID(playerid);
    new Float:vx, Float:vy, Float:vz, Float:speed;
    GetVehicleVelocity(vehicleid, vx, vy, vz);
    speed = floatsqroot(vx*vx + vy*vy + vz*vz) * 180.0;

    if (speed < 30.0) // Minimum speed for drift
    {
        gDriftScore[playerid] = 0;
        return 0;
    }

    new Float:angle = GetVehicleDriftAngle(vehicleid);
    if (angle > 15.0 && angle < 85.0) // Realistic drift angle
    {
        new added_points = floatround(angle / 5.0);
        
        // Tandem Check
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(i != playerid && IsPlayerConnected(i) && gDriftScore[i] > 10) // Only check against active drifters
            {
                if(GetPlayerDistanceFromPoint(i, x, y, z) < 15.0) 
                {
                    added_points *= 2;
                    gIsTandem[playerid] = true;
                    break;
                }
            }
        }
        
        gDriftScore[playerid] += added_points;
    }
    else
    {
        if (gDriftScore[playerid] > 0)
        {
            if (gDriftScore[playerid] > gPlayerData[playerid][best_drift])
                gPlayerData[playerid][best_drift] = gDriftScore[playerid];
            
            gPlayerData[playerid][total_points] += gDriftScore[playerid];
            DB_SavePlayer(playerid);
        }
        gDriftScore[playerid] = 0;
    }

    return gDriftScore[playerid];
}
