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
    Module: Ghost Replay System
    Record and Playback drift runs (Binary Mode)
*/

#include <open.mp>
#include <file>

#define GHOST_DIR "ghosts/"

new bool:gIsRecording[MAX_PLAYERS];
new File:gRecFile[MAX_PLAYERS];

new bool:gIsPlaying[MAX_PLAYERS];
new File:gPlayFile[MAX_PLAYERS];
new gGhostVehicle[MAX_PLAYERS];

stock StartGhostRecording(playerid, name[])
{
    if(gIsRecording[playerid]) return 0;
    
    new filepath[64];
    format(filepath, sizeof(filepath), "%s%s.dat", GHOST_DIR, name);
    
    gRecFile[playerid] = fopen(filepath, io_write); // Binary write
    if(gRecFile[playerid])
    {
        gIsRecording[playerid] = true;
        SendClientMessage(playerid, 0x00FF00FF, "Ghost: Recording started...");
        return 1;
    }
    return 0;
}

stock StopGhostRecording(playerid)
{
    if(gIsRecording[playerid])
    {
        gIsRecording[playerid] = false;
        fclose(gRecFile[playerid]);
        SendClientMessage(playerid, 0x00FF00FF, "Ghost: Recording saved.");
    }
}

stock StartGhostPlayback(playerid, name[])
{
    if(gIsPlaying[playerid]) StopGhostPlayback(playerid);
    
    new filepath[64];
    format(filepath, sizeof(filepath), "%s%s.dat", GHOST_DIR, name);
    
    if(!fexist(filepath)) return SendClientMessage(playerid, 0xFF0000FF, "Ghost: File not found.");
    
    gPlayFile[playerid] = fopen(filepath, io_read); // Binary read
    if(gPlayFile[playerid])
    {
        gIsPlaying[playerid] = true;
        
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        gGhostVehicle[playerid] = CreateVehicle(411, x, y, z, 0.0, 0, 0, -1);
        ChangeVehicleColor(gGhostVehicle[playerid], 86, 86); 
        LinkVehicleToInterior(gGhostVehicle[playerid], GetPlayerInterior(playerid));
        SetVehicleVirtualWorld(gGhostVehicle[playerid], GetPlayerVirtualWorld(playerid));
        
        // Disable collision logic would be here if using custom collision filters
        // For now, it's a physical car.
        
        SendClientMessage(playerid, 0x00FF00FF, "Ghost: Playback started.");
    }
    return 1;
}

stock StopGhostPlayback(playerid)
{
    if(gIsPlaying[playerid])
    {
        gIsPlaying[playerid] = false;
        fclose(gPlayFile[playerid]);
        DestroyVehicle(gGhostVehicle[playerid]);
        SendClientMessage(playerid, 0x00FF00FF, "Ghost: Playback stopped.");
    }
}

stock ProcessGhost(playerid)
{
    if(gIsRecording[playerid])
    {
        if(!IsPlayerInAnyVehicle(playerid)) 
        {
            StopGhostRecording(playerid);
            return;
        }

        new vehicleid = GetPlayerVehicleID(playerid);
        new buffer[7]; // Using integer array to store float bits, or tag check
        // fblockwrite expects array. Pawn tags are compile time.
        // We pack floats into this array.
        
        new Float:x, Float:y, Float:z, Float:a;
        new Float:vx, Float:vy, Float:vz;
        
        GetVehiclePos(vehicleid, x, y, z);
        GetVehicleZAngle(vehicleid, a);
        GetVehicleVelocity(vehicleid, vx, vy, vz);
        
        // Cast to untagged for filing
        buffer[0] = _:x;
        buffer[1] = _:y;
        buffer[2] = _:z;
        buffer[3] = _:a;
        buffer[4] = _:vx;
        buffer[5] = _:vy;
        buffer[6] = _:vz;
        
        fblockwrite(gRecFile[playerid], buffer, 7);
    }
    else if(gIsPlaying[playerid])
    {
        new buffer[7];
        if(fblockread(gPlayFile[playerid], buffer, 7) == 7)
        {
            new Float:x = Float:buffer[0];
            new Float:y = Float:buffer[1];
            new Float:z = Float:buffer[2];
            new Float:a = Float:buffer[3];
            new Float:vx = Float:buffer[4];
            new Float:vy = Float:buffer[5];
            new Float:vz = Float:buffer[6];
            
            SetVehiclePos(gGhostVehicle[playerid], x, y, z);
            SetVehicleZAngle(gGhostVehicle[playerid], a);
            SetVehicleVelocity(gGhostVehicle[playerid], vx, vy, vz);
        }
        else
        {
            StopGhostPlayback(playerid);
        }
    }
}
