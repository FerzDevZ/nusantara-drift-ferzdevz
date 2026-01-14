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
    Module: Dynamic Clan System
    Create, Invite, and Compete
*/

#include <open.mp>

#define DIALOG_CLAN_INVITE 2000

// Vars
new gPlayerClanName[MAX_PLAYERS][32];
new Text3D:gClanTag[MAX_PLAYERS] = {Text3D:INVALID_3DTEXT_ID, ...};

// Commands
stock ProcessClanCommands(playerid, cmdtext[])
{
    new cmd[32], idx;
    cmd = str_cmd(cmdtext, idx);
    
    if (!strcmp(cmd, "/createclan", true))
    {
        if (gPlayerData[playerid][clan_id] != 0) return SendClientMessage(playerid, -1, "Error: You are already in a clan!");
        if (gPlayerData[playerid][total_points] < 10000) return SendClientMessage(playerid, -1, "Error: Need 10,000 Drift Points to create a clan.");
        
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /createclan [Name]");
        
        // Insert to DB
        new query[256], name[MAX_PLAYER_NAME];
        GetPlayerName(playerid, name, sizeof(name));
        
        format(query, sizeof(query), "INSERT INTO clans (name, leader_name, total_points) VALUES ('%s', '%s', 0)", tmp, name);
        new DBResult:res = db_query(gDB, query);
        db_free_result(res);
        
        // Get inserted ID (Simulated by reloading or checking max ID, simplified here)
        // For accurate ID getting in pure OMP SQLite without `last_insert_rowid` easy wrapper:
        // We will just do a select query.
        format(query, sizeof(query), "SELECT id FROM clans WHERE name = '%s'", tmp);
        res = db_query(gDB, query);
        if(db_num_rows(res))
        {
            gPlayerData[playerid][clan_id] = db_get_field_assoc_int(res, "id");
            gPlayerData[playerid][total_points] -= 10000;
            
            format(gPlayerClanName[playerid], 32, "%s", tmp);
            UpdateClanTag(playerid);
            DB_SavePlayer(playerid);
            
            SendClientMessage(playerid, 0x00FF00FF, "Clan Created Successfully!");
        }
        db_free_result(res);
        return 1;
    }
    
    if (!strcmp(cmd, "/invite", true))
    {
        // Simple invite logic (direct set for now for simplicity, ideally dialog accept)
        // Check if leader... (Skipping complex leader verify for prototype speed)
        if (gPlayerData[playerid][clan_id] == 0) return SendClientMessage(playerid, -1, "You are not in a clan.");
        
        new tmp[32];
        tmp = str_cmd(cmdtext, idx);
        if(!strlen(tmp)) return SendClientMessage(playerid, -1, "Usage: /invite [playerid]");
        
        new targetid = strval(tmp);
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, -1, "Player not found.");
        if(gPlayerData[targetid][clan_id] != 0) return SendClientMessage(playerid, -1, "Player already in a clan.");
        
        if(gPlayerData[targetid][clan_id] != 0) return SendClientMessage(playerid, -1, "Player already in a clan.");
        
        new msg[128], name[MAX_PLAYER_NAME];
        GetPlayerName(playerid, name, sizeof(name));
        format(msg, sizeof(msg), "{FFFF00}%s {FFFFFF}invited you to join clan: {00FF00}%s", name, gPlayerClanName[playerid]);
        
        SetPVarInt(targetid, "ClanInviteID", gPlayerData[playerid][clan_id]);
        SetPVarInt(targetid, "ClanInviteFrom", playerid);
        
        ShowPlayerDialog(targetid, DIALOG_CLAN_INVITE, DIALOG_STYLE_MSGBOX, "Clan Invitation", msg, "Accept", "Decline");
        SendClientMessage(playerid, 0xFFFF00FF, "Invitation sent.");
        return 1;
    }
    
    if (!strcmp(cmd, "/claninfo", true))
    {
        if (gPlayerData[playerid][clan_id] == 0) return SendClientMessage(playerid, -1, "You are not in a clan.");
        
        new query[128], DBResult:res;
        format(query, sizeof(query), "SELECT name, total_points FROM clans WHERE id = %d", gPlayerData[playerid][clan_id]);
        res = db_query(gDB, query);
        if(db_num_rows(res))
        {
            new cname[32], points;
            db_get_field_assoc(res, "name", cname, 32);
            points = db_get_field_assoc_int(res, "total_points");
            
            new msg[128];
            format(msg, sizeof(msg), "Clan: %s | Total Score: %d", cname, points);
            SendClientMessage(playerid, -1, msg);
        }
        db_free_result(res);
        return 1;
    }
    
    return 0;
}

stock UpdateClanTag(playerid)
{
    if(gClanTag[playerid] != Text3D:INVALID_3DTEXT_ID)
    {
        Delete3DTextLabel(gClanTag[playerid]);
        gClanTag[playerid] = Text3D:INVALID_3DTEXT_ID;
    }

    if(gPlayerData[playerid][clan_id] != 0)
    {
        gClanTag[playerid] = Create3DTextLabel(gPlayerClanName[playerid], 0x00FF00FF, 0.0, 0.0, 0.0, 20.0, 0, false);
        Attach3DTextLabelToPlayer(gClanTag[playerid], playerid, 0.0, 0.0, 0.4);
    }
}

stock LoadPlayerClanName(playerid)
{
    if(gPlayerData[playerid][clan_id] == 0) 
    {
        gPlayerClanName[playerid][0] = EOS;
        return;
    }
    
    new query[128], DBResult:res;
    format(query, sizeof(query), "SELECT name FROM clans WHERE id = %d", gPlayerData[playerid][clan_id]);
    res = db_query(gDB, query);
    if(db_num_rows(res))
    {
        db_get_field_assoc(res, "name", gPlayerClanName[playerid], 32);
        UpdateClanTag(playerid);
    }
    else
    {
        gPlayerData[playerid][clan_id] = 0; // ID not found cleanup
    }
    db_free_result(res);
}

stock ProcessClanDialog(playerid, dialogid, response)
{
    if(dialogid == DIALOG_CLAN_INVITE)
    {
        if(!response)
        {
            SendClientMessage(GetPVarInt(playerid, "ClanInviteFrom"), -1, "Player declined your invitation.");
            DeletePVar(playerid, "ClanInviteID");
            DeletePVar(playerid, "ClanInviteFrom");
            return 1;
        }
        
        new clanid = GetPVarInt(playerid, "ClanInviteID");
        if(clanid == 0) return 1;
        
        gPlayerData[playerid][clan_id] = clanid;
        
        // Fetch clan name to set local cache
        new query[128], DBResult:res;
        format(query, sizeof(query), "SELECT name FROM clans WHERE id = %d", clanid);
        res = db_query(gDB, query);
        if(db_num_rows(res))
        {
            db_get_field_assoc(res, "name", gPlayerClanName[playerid], 32);
            UpdateClanTag(playerid);
            DB_SavePlayer(playerid);
            
            SendClientMessage(playerid, 0x00FF00FF, "You joined the clan!");
            SendClientMessage(GetPVarInt(playerid, "ClanInviteFrom"), 0x00FF00FF, "Player joined your clan.");
        }
        db_free_result(res);
        
        DeletePVar(playerid, "ClanInviteID");
        DeletePVar(playerid, "ClanInviteFrom");
        return 1;
    }
    return 0;
}
