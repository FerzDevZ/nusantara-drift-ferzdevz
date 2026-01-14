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
    Module: Ghost Duel System
    Race against the ghost!
*/

#include <open.mp>

new bool:gDuelActive[MAX_PLAYERS];
new gDuelStartTime[MAX_PLAYERS];
new gDuelTargetTime[MAX_PLAYERS]; // Duration of ghost run

stock StartDuel(playerid, ghost_name[])
{
    StartGhostPlayback(playerid, ghost_name);
    
    if(gIsPlaying[playerid])
    {
        gDuelActive[playerid] = true;
        gDuelStartTime[playerid] = GetTickCount();
        
        // In a real file system we'd read header for duration, 
        // for now we trust the player beats the playback end.
        // Actually, we can check when playback stops automatically.
        
        SendClientMessage(playerid, 0xFF0000FF, "DUEL STARTED! Beat the Ghost to the finish line!");
        
        // Reset player pos to start (Simplified: assumes player is near start)
        // Ideally we read start pos from ghost file header.
    }
}

stock CheckDuelFinish(playerid)
{
    if(gDuelActive[playerid])
    {
        if(!gIsPlaying[playerid]) // Ghost finished/stopped
        {
            // If ghost stopped naturally (EOF) and player hasn't triggered 'win' logic yet (e.g. checkpoint)
            // Then player lost time-wise (simplification).
            // Or simpler: If player is still driving when ghost vanishes.
            
            SendClientMessage(playerid, 0xFF0000FF, "Duel Result: Ghost Finished."); 
            gDuelActive[playerid] = false;
        }
    }
}
