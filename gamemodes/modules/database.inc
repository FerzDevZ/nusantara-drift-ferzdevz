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
    Module: Database
    Handle player statistics using Open.mp Native SQLite
*/

#include <open.mp>

enum E_PLAYER_DATA
{
    best_drift,
    total_points,
    admin_level,
    clan_id,
    tutorial_passed,
    mission_data[128],
    password_hash[65] // SHA256 hex string is 64 chars + null
};
new gPlayerData[MAX_PLAYERS][E_PLAYER_DATA];

new DB:gDB;
new bool:gPlayerLoggedIn[MAX_PLAYERS];

// Helper to sanitize logic
stock SQL_Escape(const string[])
{
    new out[128];
    new len = strlen(string);
    new j = 0;
    for(new i = 0; i < len && j < 127; i++)
    {
        if(string[i] == '\'' || string[i] == '\"' || string[i] == '\\' || string[i] == '`')
        {
            // Skip or replace dangerous characters
            continue; 
        }
        out[j++] = string[i];
    }
    out[j] = '\0';
    return out;
}

stock DB_Init()
{
    gDB = db_open("ferz_drift.db");
    if (gDB)
    {
        // Players Table
        db_free_result(db_query(gDB, "CREATE TABLE IF NOT EXISTS players (name TEXT UNIQUE, best_drift INTEGER, total_points INTEGER, admin_level INTEGER, clan_id INTEGER, tutorial_passed INTEGER DEFAULT 0, password TEXT)"));
        
        // Migration: Attempt to add password column if it doesn't exist (Quick & Dirty for SQLite)
        // If it fails (exists), no harm.
        db_free_result(db_query(gDB, "ALTER TABLE players ADD COLUMN password TEXT"));
        db_free_result(db_query(gDB, "ALTER TABLE players ADD COLUMN tutorial_passed INTEGER DEFAULT 0"));
        db_free_result(db_query(gDB, "ALTER TABLE players ADD COLUMN mission_data TEXT"));
        
        // Clans Table
        db_free_result(db_query(gDB, "CREATE TABLE IF NOT EXISTS clans (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, leader_name TEXT, total_points INTEGER)"));
        
        // Garage Table
        db_free_result(db_query(gDB, "CREATE TABLE IF NOT EXISTS player_vehicles (id INTEGER PRIMARY KEY AUTOINCREMENT, owner TEXT, model INTEGER, components TEXT, colors TEXT, paintjob INTEGER, speed_stage INTEGER, neon_model INTEGER DEFAULT 0)"));
        db_free_result(db_query(gDB, "ALTER TABLE player_vehicles ADD COLUMN neon_model INTEGER DEFAULT 0"));
        
        // Stickers Table
        db_free_result(db_query(gDB, "CREATE TABLE IF NOT EXISTS vehicle_stickers (veh_db_id INTEGER, slot INTEGER, text TEXT, x FLOAT, y FLOAT, z FLOAT, rx FLOAT, ry FLOAT, rz FLOAT)"));
        
        print("Database: Connected to ferz_drift.db");
    }
    else
    {
        print("Database: Failed to connect!");
    }
}

stock DB_SavePlayer(playerid)
{
    if(!gPlayerLoggedIn[playerid]) return; // Don't save if not logged in
    
    new name[MAX_PLAYER_NAME], query[1024], clean_name[MAX_PLAYER_NAME]; 
    GetPlayerName(playerid, name, sizeof(name));
    format(clean_name, sizeof(clean_name), "%s", SQL_Escape(name));
    
    format(query, sizeof(query), "INSERT OR REPLACE INTO players (name, best_drift, total_points, admin_level, clan_id, tutorial_passed, password, mission_data) VALUES ('%s', %d, %d, %d, %d, %d, '%s', '%s')", 
        clean_name, gPlayerData[playerid][best_drift], gPlayerData[playerid][total_points], gPlayerData[playerid][admin_level], gPlayerData[playerid][clan_id], gPlayerData[playerid][tutorial_passed], gPlayerData[playerid][password_hash], gPlayerData[playerid][mission_data]);
    
    db_free_result(db_query(gDB, query));
}

stock DB_LoadPlayer(playerid)
{
    // Now called ONLY after Login
    new name[MAX_PLAYER_NAME], query[256], DBResult:res, clean_name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    format(clean_name, sizeof(clean_name), "%s", SQL_Escape(name));
    
    format(query, sizeof(query), "SELECT best_drift, total_points, admin_level, clan_id, tutorial_passed, password, mission_data FROM players WHERE name = '%s'", clean_name);
    res = db_query(gDB, query);
    
    if (db_num_rows(res))
    {
        gPlayerData[playerid][best_drift] = db_get_field_assoc_int(res, "best_drift");
        gPlayerData[playerid][total_points] = db_get_field_assoc_int(res, "total_points");
        gPlayerData[playerid][admin_level] = db_get_field_assoc_int(res, "admin_level");
        gPlayerData[playerid][clan_id] = db_get_field_assoc_int(res, "clan_id");
        gPlayerData[playerid][tutorial_passed] = db_get_field_assoc_int(res, "tutorial_passed");
        db_get_field_assoc(res, "password", gPlayerData[playerid][password_hash], 65);
        db_get_field_assoc(res, "mission_data", gPlayerData[playerid][mission_data], 128);
    }
    db_free_result(res);
}

// Check if player is registered
stock DB_IsPlayerRegistered(playerid)
{
    new name[MAX_PLAYER_NAME], query[128], DBResult:res, clean_name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    format(clean_name, sizeof(clean_name), "%s", SQL_Escape(name));
    format(query, sizeof(query), "SELECT password FROM players WHERE name = '%s'", clean_name);
    res = db_query(gDB, query);
    
    new bool:registered = false;
    if(db_num_rows(res))
    {
        // Load password hash to verify against input
        db_get_field_assoc(res, "password", gPlayerData[playerid][password_hash], 65);
        if(strlen(gPlayerData[playerid][password_hash]) > 0) registered = true;
    }
    db_free_result(res);
    return registered;
}
