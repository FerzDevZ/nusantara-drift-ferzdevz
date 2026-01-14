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

// Compat flags
#define SAMP_COMPAT
#define MIXED_SPELLINGS

#include <open.mp>

// Modules
#include "modules/database.inc"
#include "modules/core.inc"
#include "modules/hud.inc"
#include "modules/visual.inc"
#include "modules/fuel.inc"
#include "modules/garage.inc"
#include "modules/vehicle_system.inc"
#include "modules/drift_system.inc"
#include "modules/auth.inc"
#include "modules/admin.inc"
#include "modules/shop.inc"
#include "modules/maps.inc"
#include "modules/ghost.inc"
#include "modules/clans.inc"
#include "modules/missions.inc"
#include "modules/duel.inc"
#include "modules/tuning.inc"
#include "modules/audio.inc"
#include "modules/ui.inc"
#include "modules/tutorial.inc"
#include "modules/afk.inc"
#include "modules/weather.inc"
#include "modules/teleports.inc"
#include "modules/rank.inc"
#include "modules/icons.inc"
#include "modules/help.inc"
#include "modules/radio.inc"
#include "modules/handling.inc"
#include "modules/smoke.inc"

main()
{
    print("\n----------------------------------");
    print(" FerzRP Ultimate Drift Starting...");
    print(" Author: FerzDevZ");
    print("----------------------------------\n");
}

public OnGameModeInit()
{
    DB_Init();
    Maps_Load();
    SetGameModeText("FerzRP: Ultimate Drift");
    
    // Spawns
    AddPlayerClass(0, 411.3323, 2520.1252, 16.6341, 180.0, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0); 
    
    SetTimer("GlobalUpdate", 100, true);
    SetTimer("PeriodicSave", 300000, true); // Every 5 minutes
    return 1;
}

forward PeriodicSave();
public PeriodicSave()
{
    print("Database: Periodic Save Initiated...");
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && gPlayerLoggedIn[i])
        {
            DB_SavePlayer(i);
        }
    }
}

public OnPlayerConnect(playerid)
{
    // Auth System handles loading after login
    Auth_OnPlayerConnect(playerid);
    
    // Auto-Set Admin for Owner (Check logic)
    // We should move this to Auth Success or leave it here? 
    // If we verify Name, it's fine. But DB Load overwrites level.
    // So this check must happen AFTER DB Load.
    // I will remove it from here and rely on DB Load. 
    // If I need to set Owner Admin first time, I should do it manually in DB or Console command.
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    DB_SavePlayer(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid, 411.3323, 2520.1252, 16.6341);
    return 1;
}

public OnPlayerText(playerid, text[])
{
    new string[144];
    FormatRankChat(playerid, text, string, sizeof(string));
    SendClientMessageToAll(-1, string);
    return 0; // Block default message
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    ProcessVehicleAudio(playerid, newstate, oldstate);
    return 1;
}

forward GlobalUpdate();
public GlobalUpdate()
{
    ProcessWeather();
    
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i)) continue;
        
        ProcessAFK(i);
        ProcessGhost(i);
        ProcessVehicleCore(i);
        ProcessHandlingPhysics(i);
        ProcessStickerEditor(i);
        ProcessTireSmoke(i);
        ProcessFuelConsumption(i);
        
        // HUD updated in driver block or here? 
        // Let's call it once correctly.
        if(GetPlayerState(i) != PLAYER_STATE_DRIVER) HUD_Update(i, 0, 0, gIsTandem[i], 0.0);
        
        if(GetPlayerState(i) == PLAYER_STATE_DRIVER)
        {
            new drift_score = ProcessDrift(i);
            CheckTutorialProgress(i, drift_score);
            
            if(drift_score > 0)
            {
                 UpdateMission(i, 0, drift_score); // Score Mission
                 UpdateMission(i, 1, 1); // Drift Count Mission
                 if(gIsTandem[i]) UpdateMission(i, 2, 1); // Tandem Mission
            }
            
            new vehicleid = GetPlayerVehicleID(i);
            new Float:vx, Float:vy, Float:vz;
            GetVehicleVelocity(vehicleid, vx, vy, vz);
            new kmh = floatround(floatsqroot(vx*vx + vy*vy + vz*vz) * 180.0);
            
            HUD_Update(i, kmh, drift_score, gIsTandem[i], gVehicleFuel[vehicleid]);
            
            // Ghost Process
            ProcessGhost(i);
        }
    }
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    // Admin Commands (has its own parser)
    if (ProcessAdminCommands(playerid, cmdtext)) return 1;
    if (ProcessClanCommands(playerid, cmdtext)) return 1;
    if (ProcessTutorialCommands(playerid, cmdtext)) return 1;

    new cmd[32], idx;
    cmd = str_cmd(cmdtext, idx);
    
    if (!strcmp(cmd, "/teles", true) || !strcmp(cmd, "/drift", true))
    {
        ShowTeleportMenu(playerid);
        return 1;
    }

    if (!strcmp(cmd, "/help", true))
    {
        ShowHelpDialog(playerid);
        return 1;
    }
    
    if (!strcmp(cmd, "/radio", true))
    {
        ShowRadioDialog(playerid);
        return 1;
    }

    if (!strcmp(cmd, "/handling", true))
    {
        ShowHandlingMenu(playerid);
        return 1;
    }

    if (!strcmp(cmd, "/missions", true))
    {
        ShowMissions(playerid);
        return 1;
    }
    
    if (!strcmp(cmd, "/duel", true))
    {
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /duel [ghost_name]");
        StartDuel(playerid, tmp);
        return 1;
    }
    

    if (!strcmp(cmd, "/buycar", true))
    {
        // Example: Buy Elegy for 20000 points
        // In real system, dialog menu to pick car.
        BuyVehicle(playerid, 562, 20000); 
        return 1;
    }
    
    if (!strcmp(cmd, "/fill", true))
    {
        FillFuel(playerid);
        return 1;
    }
    
    if (!strcmp(cmd, "/mycars", true))
    {
        ShowGarageMenu(playerid);
        return 1;
    }

    if (!strcmp(cmd, "/drift", true))
    {
        SpawnDriftVehicle(playerid, 562);
        return 1;
    }
    
    if (!strcmp(cmd, "/v", true))
    {
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        
        if (!strcmp(tmp, "elegy", true)) SpawnDriftVehicle(playerid, 562);
        else if (!strcmp(tmp, "sultan", true)) SpawnDriftVehicle(playerid, 560);
        else if (!strcmp(tmp, "jester", true)) SpawnDriftVehicle(playerid, 559);
        else if (!strcmp(tmp, "uranus", true)) SpawnDriftVehicle(playerid, 558);
        else if (!strcmp(tmp, "flash", true)) SpawnDriftVehicle(playerid, 565);
        else SendClientMessage(playerid, -1, "Usage: /v [elegy/sultan/jester/uranus/flash]");
        return 1;
    }

    if (!strcmp(cmd, "/rec", true) || !strcmp(cmd, "/record", true))
    {
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /rec [name]");
        
        StartGhostRecording(playerid, tmp);
        return 1;
    }
    
    if (!strcmp(cmd, "/play", true))
    {
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /play [name]");
        
        StartGhostPlayback(playerid, tmp);
        return 1;
    }
    
    if (!strcmp(cmd, "/stop", true))
    {
        StopGhostRecording(playerid);
        StopGhostPlayback(playerid);
        return 1;
    }
    
    if (!strcmp(cmd, "/shop", true))
    {
        ShowTuningMenu(playerid);
        return 1;
    }

    if (!strcmp(cmd, "/sticker", true))
    {
        // Need rest of string.
        // str_cmd is space delimited.
        if(!strlen(cmdtext[idx])) return SendClientMessage(playerid, -1, "Usage: /sticker [text]");
        
        CreateSticker(playerid, cmdtext[idx]); // Pass the rest of the string
        return 1;
    }
    
    return 0;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == 1000 || dialogid == 1001)
    {
        ProcessAuthDialog(playerid, dialogid, response, listitem, inputtext);
        return 1;
    }
    if(dialogid == 300)
    {
        ProcessGarageDialog(playerid, response, listitem, inputtext);
        return 1;
    }
    if(dialogid == 301)
    {
        ProcessGarageAction(playerid, response, listitem);
        return 1;
    }
    if(dialogid == 400)
    {
        ProcessTeleportDialog(playerid, response, listitem);
        return 1;
    }
    if(dialogid == 500)
    {
        return 1; // Help dialog has no logic besides Close
    }
    if(dialogid == 600)
    {
        ProcessRadioDialog(playerid, response, listitem);
        return 1;
    }
    if(dialogid == 700)
    {
        ProcessHandlingDialog(playerid, response, listitem);
        return 1;
    }
    if(dialogid >= 800 && dialogid <= 804) // Enhanced to 804 for Neon
    {
        ProcessTuningDialog(playerid, dialogid, response, listitem);
        return 1;
    }
    if(dialogid == 2000) // DIALOG_CLAN_INVITE
    {
        ProcessClanDialog(playerid, dialogid, response);
        return 1;
    }
    return 1;
}
