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
    Module: Daily Missions
    Reset everyday, win rewards
*/

#include <open.mp>

#define MAX_MISSIONS 3

enum E_MISSION
{
    mType,      // 0: Score, 1: Distance, 2: Tandem
    mTarget,
    mProgress,
    bool:mCompleted
}

new gMissions[MAX_PLAYERS][MAX_MISSIONS][E_MISSION];
new gLastDate[MAX_PLAYERS];

stock InitMissions(playerid)
{
    new y, m, d;
    getdate(y, m, d);
    
    if(gLastDate[playerid] != d)
    {
        gLastDate[playerid] = d;
        GenerateMissions(playerid);
        SendClientMessage(playerid, 0xFFFF00FF, "New Daily Missions Available! Type /missions");
    }
}

stock GenerateMissions(playerid)
{
    // Mission 1: Score
    gMissions[playerid][0][mType] = 0;
    gMissions[playerid][0][mTarget] = 10000 + random(20000);
    gMissions[playerid][0][mProgress] = 0;
    gMissions[playerid][0][mCompleted] = false;
    
    // Mission 2: Distance (Simulated as Drift counts for ease)
    gMissions[playerid][1][mType] = 1; // "Drift X Times"
    gMissions[playerid][1][mTarget] = 50 + random(50);
    gMissions[playerid][1][mProgress] = 0;
    gMissions[playerid][1][mCompleted] = false;
    
    // Mission 3: Tandem
    gMissions[playerid][2][mType] = 2; // "Start Tandem" count
    gMissions[playerid][2][mTarget] = 5 + random(5);
    gMissions[playerid][2][mProgress] = 0;
    gMissions[playerid][2][mCompleted] = false;
}

stock UpdateMission(playerid, type, amount)
{
    for(new i=0; i<MAX_MISSIONS; i++)
    {
        if(!gMissions[playerid][i][mCompleted] && gMissions[playerid][i][mType] == type)
        {
            gMissions[playerid][i][mProgress] += amount;
            if(gMissions[playerid][i][mProgress] >= gMissions[playerid][i][mTarget])
            {
                gMissions[playerid][i][mCompleted] = true;
                gPlayerData[playerid][total_points] += 5000;
                SendClientMessage(playerid, 0x00FF00FF, "Mission Complete! Reward: 5000 Points");
                DB_SavePlayer(playerid);
            }
        }
    }
}

stock ShowMissions(playerid)
{
    new str[512], mstr[128];
    format(str, sizeof(str), "Daily Missions (%d):\n", gLastDate[playerid]);
    
    for(new i=0; i<MAX_MISSIONS; i++)
    {
        new status[16];
        if(gMissions[playerid][i][mCompleted]) format(status, 16, "{00FF00}DONE");
        else format(status, 16, "%d/%d", gMissions[playerid][i][mProgress], gMissions[playerid][i][mTarget]);
        
        switch(gMissions[playerid][i][mType])
        {
            case 0: format(mstr, 128, "1. Get Total Score: %s\n", status);
            case 1: format(mstr, 128, "2. Successful Drifts: %s\n", status);
            case 2: format(mstr, 128, "3. Do Tandems: %s\n", status);
        }
        strcat(str, mstr);
    }
    
    ShowPlayerDialog(playerid, 200, DIALOG_STYLE_MSGBOX, "Daily Missions", str, "OK", "");
}
