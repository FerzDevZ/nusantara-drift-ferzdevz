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
    Module: Admin System
    Handle admin commands and levels
*/

#include <open.mp>

// Commands
stock ProcessAdminCommands(playerid, cmdtext[])
{
    new cmd[32], idx;
    cmd = str_cmd(cmdtext, idx);

    // ------------------------------------------------
    // General
    // ------------------------------------------------
    if (!strcmp(cmd, "/admin", true))
    {
        new str[64];
        format(str, sizeof(str), "Your Admin Level: %d", gPlayerData[playerid][admin_level]);
        SendClientMessage(playerid, -1, str);
        return 1;
    }
    
    // ------------------------------------------------
    // RCON (Owner)
    // ------------------------------------------------
    if (!strcmp(cmd, "/makeadmin", true))
    {
        if (!IsPlayerAdmin(playerid)) return 0;
        
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /makeadmin [id] [level]");
        new targetid = strval(tmp);
        
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /makeadmin [id] [level]");
        new level = strval(tmp);

        if (!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "Player not found!");
        
        gPlayerData[targetid][admin_level] = level;
        DB_SavePlayer(targetid);
        
        new str[128];
        format(str, sizeof(str), "You set %s's admin level to %d", ret_GetPlayerName(targetid), level);
        SendClientMessage(playerid, 0x00FF00FF, str);
        return 1;
    }

    // ------------------------------------------------
    // Admin Level 1+
    // ------------------------------------------------
    if (gPlayerData[playerid][admin_level] > 0)
    {
        if (!strcmp(cmd, "/kick", true))
        {
            new tmp[32];
            tmp = str_cmd(cmdtext, idx);
            if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /kick [id] [reason]");
            new targetid = strval(tmp);
            
            if (!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "Player not found!");
            
            // Reconstruct reason from remaining string
            new reason[64];
            strmid(reason, cmdtext, idx, strlen(cmdtext));
            if(!strlen(reason)) format(reason, sizeof(reason), "No Reason");

            new str[128];
            format(str, sizeof(str), "Admin %s kicked %s (Reason: %s)", ret_GetPlayerName(playerid), ret_GetPlayerName(targetid), reason);
            SendClientMessageToAll(0xFF0000FF, str);
            
            Kick(targetid);
            return 1;
        }

        if (!strcmp(cmd, "/goto", true))
        {
            new tmp[32];
            tmp = str_cmd(cmdtext, idx);
            if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /goto [id]");
            new targetid = strval(tmp);

            if (!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "Player not found!");
            
            new Float:x, Float:y, Float:z;
            GetPlayerPos(targetid, x, y, z);
            SetPlayerPos(playerid, x + 2.0, y, z);
            SendClientMessage(playerid, -1, "Teleported.");
            return 1;
        }
        
        if (!strcmp(cmd, "/veh", true))
        {
            if (gPlayerData[playerid][admin_level] < 2) return SendClientMessage(playerid, -1, "You need level 2+");
            
            new tmp[32];
            tmp = str_cmd(cmdtext, idx);
            if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /veh [modelid]");
            new modelid = strval(tmp);
            
            if (modelid < 400 || modelid > 611) return SendClientMessage(playerid, -1, "Invalid vehicle model.");
            
            SpawnDriftVehicle(playerid, modelid);
            return 1;
        }
    }
    return 0;
}

stock str_cmd(const string[], &index)
{
    new length = strlen(string);
    while ((index < length) && (string[index] <= ' '))
    {
        index++;
    }

    new offset = index;
    new result[32];
    while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
    {
        result[index - offset] = string[index];
        index++;
    }
    result[index - offset] = EOS;
    return result;
}

// Helpers
stock ret_GetPlayerName(playerid)
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}

// Custom sscanf implementation for basic usage if plugin not available
// Note: For now assuming manual parsing or single param standard commands would be safer without plugin dependency,
// but for quality admin commands, sscanf logic is simulated here for common "u" (user) and "i" (int) types.
stock sscanf(string[], format[], {Float,_}:...)
{
    // Simplified stub. In real production, use sscanf plugin.
    // This is just to allow the code above to Structure logically.
    // For simplicity in this environment without plugins, we revert to manual parsing below.
    return 1; 
}
// REAL IMPLEMENTATION OF COMMANDS WITHOUT SSCANF PLUGIN DEPENDENCY
// Redefining OnPlayerCommandText below to use standard string parsing
