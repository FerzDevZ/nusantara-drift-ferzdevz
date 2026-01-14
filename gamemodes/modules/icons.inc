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
    Module: Global Navigation (Map Icons)
    Radar visual aids.
*/

#include <open.mp>

// Icon IDs
// 5 = Race Flag
// 17 = Dollar
// 55 = Car
// 19 = Enemy (Red) - can use for Docks
// 6 = Blue square (generic)

stock LoadMapIcons(playerid)
{
    // Shop (Dollar) - LS
    // Assuming Shop is somewhere near spawn initially or we pick a spot.
    // Let's use the spawn area for now as "Central Hub"
    SetPlayerMapIcon(playerid, 1, 411.3323, 2520.1252, 16.6341, 17, 0, MAPICON_GLOBAL); 

    // Garage (Car) - LS Airport
    SetPlayerMapIcon(playerid, 2, -1426.6833, -256.0206, 14.1484, 55, 0, MAPICON_GLOBAL);
    
    // Drift: Ocean Docks (Flag)
    SetPlayerMapIcon(playerid, 3, 2779.6423, -2455.4382, 13.6346, 5, 0, MAPICON_GLOBAL);
    
    // Drift: SF Airport (Flag)
    SetPlayerMapIcon(playerid, 4, -1542.4921, -426.7570, 6.0000, 5, 0, MAPICON_GLOBAL);
    
    // Drift: Chilliad (Flag)
    SetPlayerMapIcon(playerid, 5, -2331.4363, -1674.8080, 483.5658, 5, 0, MAPICON_GLOBAL);
    
    // Clan HQ (later)
}
