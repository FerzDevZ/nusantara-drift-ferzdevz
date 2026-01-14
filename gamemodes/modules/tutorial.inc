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
    Module: Interactive Drift School (Tutorial)
    Welcome to FerzRP.
*/

#include <open.mp>

new bool:gInTutorial[MAX_PLAYERS];
new gTutorialStep[MAX_PLAYERS];
new gTutorialVeh[MAX_PLAYERS];

stock StartTutorial(playerid)
{
    gInTutorial[playerid] = true;
    gTutorialStep[playerid] = 0;
    
    TogglePlayerControllable(playerid, false);
    SetPlayerVirtualWorld(playerid, playerid + 1000); // Private world
    
    // Cinematic Cam: Airport Flyover
    SetPlayerCameraPos(playerid, -1426.6833, -934.0206, 200.0000);
    SetPlayerCameraLookAt(playerid, -1426.6833, -256.0206, 14.1484);
    
    ShowNotification(playerid, "Welcome", "Welcome to Nusantara Drift!", 5000);
    
    SetTimerEx("TutorialStep1", 5000, false, "i", playerid);
}

forward TutorialStep1(playerid);
public TutorialStep1(playerid)
{
    if(!IsPlayerConnected(playerid) || !gInTutorial[playerid]) return;
    
    // Step 1: Explain Drift
    ShowNotification(playerid, "Drift School", "Goal: Earn 1000 Drift Points to get your license.", 5000);
    
    // Spawn in car (Slightly higher to avoid floor glitch)
    new veh = CreateVehicle(562, -1426.6833, -256.0206, 15.5, 0.0, 1, 1, -1); // Elegy
    gTutorialVeh[playerid] = veh;
    SetVehicleVirtualWorld(veh, playerid + 1000);
    LinkVehicleToInterior(veh, 0);
    
    // Give time for world to stream
    SetTimerEx("TutorialPutInVeh", 1000, false, "ii", playerid, veh);
    
    SetPlayerCameraPos(playerid, -1436.6833, -266.0206, 20.0000);
    SetPlayerCameraLookAt(playerid, -1426.6833, -256.0206, 14.1484);
}

forward TutorialPutInVeh(playerid, vehicleid);
public TutorialPutInVeh(playerid, vehicleid)
{
    if(!IsPlayerConnected(playerid) || !gInTutorial[playerid]) 
    {
        if(IsValidVehicle(vehicleid)) DestroyVehicle(vehicleid);
        return;
    }
    
    PutPlayerInVehicle(playerid, vehicleid, 0);
    SetTimerEx("TutorialStep2", 3000, false, "i", playerid);
}

forward TutorialStep2(playerid);
public TutorialStep2(playerid)
{
    if(!IsPlayerConnected(playerid) || !gInTutorial[playerid]) return;
    
    SetCameraBehindPlayer(playerid);
    TogglePlayerControllable(playerid, 1);
    
    ShowNotification(playerid, "Test Start", "Start Drifting Now! (Use Handbrake)", 5000);
    gTutorialStep[playerid] = 1; // Mark as "In Test"
}

// Called from ProcessDrift in main script
stock CheckTutorialProgress(playerid, current_score)
{
    if(gInTutorial[playerid] && gTutorialStep[playerid] == 1)
    {
        if(current_score >= 1000)
        {
            gTutorialStep[playerid] = 2; // Passed
            ShowNotification(playerid, "PASSED", "License Aquired! Joining Main World...", 5000);
            SetTimerEx("EndTutorial", 3000, false, "i", playerid);
        }
    }
}

forward EndTutorial(playerid);
public EndTutorial(playerid)
{
    if(!IsPlayerConnected(playerid)) return;
    
    gInTutorial[playerid] = false;
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetPlayerVirtualWorld(playerid, 0);
    
    // Destroy Tutorial Veh
    if(gTutorialVeh[playerid] && IsValidVehicle(gTutorialVeh[playerid]))
    {
        DestroyVehicle(gTutorialVeh[playerid]);
        gTutorialVeh[playerid] = 0;
    }
    
    // Fixed Safe Spawn for completion
    SetPlayerPos(playerid, 411.3323, 2520.1252, 16.6341);
    SpawnDriftVehicle(playerid, 562); 
    
    gPlayerData[playerid][tutorial_passed] = 1;
    DB_SavePlayer(playerid); // Save "Passed" state
}

stock ProcessTutorialCommands(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/skiptutorial", true))
    {
        if(gInTutorial[playerid])
        {
            EndTutorial(playerid);
            SendClientMessage(playerid, -1, "Tutorial Skipped.");
            return 1;
        }
    }
    return 0;
}
