/*
================================================================
    FerzRP - MODERN RUNNER (open.mp)
    ------------------------------------------------------------
    Author      : FerzDevZ
    Youtube     : Ferzsampp
    Instagram   : ferzchills
    Discord     : ferzdevz
================================================================
*/

#include <open.mp>

// Note: Shop logic has been moved to tuning.inc (Ultimate Tuning Shop)
// This file is kept for backward compatibility if needed.

#define DIALOG_SHOP_MAIN    100
#define DIALOG_SHOP_NEON    101
#define DIALOG_SHOP_WHEELS  102
#define DIALOG_SHOP_CARS    103

stock ShowShopMenu(playerid)
{
    ShowTuningMenu(playerid); // Redirect to new system
    return 1;
}
