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
    Module: Ranking System
    Progression & Status
*/

#include <open.mp>

// Rank Thresholds
#define RANK_ROOKIE 0
#define RANK_STREET 10000
#define RANK_PRO    50000
#define RANK_KING   200000

stock GetPlayerRankName(playerid, rankname[], len)
{
    new score = gPlayerData[playerid][total_points];
    
    if(score >= RANK_KING) format(rankname, len, "{FFD700}Drift King");
    else if(score >= RANK_PRO) format(rankname, len, "{00FFFF}Pro Drifter");
    else if(score >= RANK_STREET) format(rankname, len, "{00FF00}Street Drifter");
    else format(rankname, len, "{CCCCCC}Rookie");
}

stock ProcessRankCheck(playerid)
{
    // Call this occasionally to check if they ranked up and notify
    // Optimally in ProcessDrift end
    // For now simple passive check or command display
}

// Hook to Chat (OnPlayerText)
// We need to export a function to format chat
stock FormatRankChat(playerid, text[], output[], len)
{
    new rank[32], name[MAX_PLAYER_NAME];
    GetPlayerRankName(playerid, rank, sizeof(rank));
    GetPlayerName(playerid, name, sizeof(name));
    
    if(gPlayerData[playerid][admin_level] > 0)
    {
         format(output, len, "{FF0000}[Admin] %s %s: {FFFFFF}%s", rank, name, text);
    }
    else
    {
         format(output, len, "[%s] %s: {FFFFFF}%s", rank, name, text);
    }
}
