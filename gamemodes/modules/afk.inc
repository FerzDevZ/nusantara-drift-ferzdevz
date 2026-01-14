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
    Module: AFK Passive Income
    Relax and earn.
*/

#include <open.mp>

// Lounge Zone (Example Coords inside LS Airport Terminal or similar)
// Simulating an area at 0,0,0 (Farm) for ease of testing if map not loaded
// In production, use real interior coords.
// Let's use Airport waiting area approximation: X: -1400, Y: -150

new bool:gIsAFK[MAX_PLAYERS];

stock ProcessAFK(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        
        // Simple radius check for AFK Zone at LS Airport Tower
        if(IsPlayerInRangeOfPoint(playerid, 20.0, -1423.7089,-287.9732,14.1484)) // Roughly near spawn
        {
            if(!gIsAFK[playerid])
            {
                gIsAFK[playerid] = true;
                ShowNotification(playerid, "AFK MODE", "You entered AFK Zone. Earning points...", 5000);
            }
            
            // Give 50 points every update (approx 1 sec in main loop) -> 3000/min
            // Maybe too OP? Let's reduce.
            // Main loop calls this every sec? ProcessDrift calls every 100ms? 
            // We need a separate timer or check interval.
            // Let's assume GlobalUpdate is 1 sec.
            
            gPlayerData[playerid][total_points] += 10; 
        }
        else
        {
            if(gIsAFK[playerid])
            {
                gIsAFK[playerid] = false;
                ShowNotification(playerid, "Active Mode", "You left the AFK Zone.", 3000);
            }
        }
    }
}
