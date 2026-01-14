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

// Vars
new gPlayerClanName[MAX_PLAYERS][32];

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
        
        gPlayerData[targetid][clan_id] = gPlayerData[playerid][clan_id];
        format(gPlayerClanName[targetid], 32, "%s", gPlayerClanName[playerid]);
        UpdateClanTag(targetid);
        
        DB_SavePlayer(targetid);
        SendClientMessage(targetid, 0x00FF00FF, "You have been added to the clan!");
        SendClientMessage(playerid, 0x00FF00FF, "Player invited.");
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
    if(gPlayerData[playerid][clan_id] != 0)
    {
        // Very basic implementation: simply reset name to standard format if not already
        // Note: SetPlayerName has restrictions, this is just visual concept
        // Better: use 3D Text Label above head
        
        new Text3D:label = Create3DTextLabel(gPlayerClanName[playerid], 0x00FF00FF, 0.0, 0.0, 0.0, 20.0, 0, false);
        Attach3DTextLabelToPlayer(label, playerid, 0.0, 0.0, 0.4);
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
