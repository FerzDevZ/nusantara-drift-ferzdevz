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
    Module: HUD & Speedometer
    Handle TextDraws for speed and drift score
*/

#include <open.mp>

new PlayerText:SpeedoTD[MAX_PLAYERS];
new PlayerText:DriftScoreTD[MAX_PLAYERS];
new PlayerText:TandemTD[MAX_PLAYERS];
new PlayerText:FuelTD[MAX_PLAYERS];

stock HUD_Create(playerid)
{
    // Speedometer TD
    SpeedoTD[playerid] = CreatePlayerTextDraw(playerid, 510.0, 380.0, "000 KM/H");
    PlayerTextDrawAlignment(playerid, SpeedoTD[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColor(playerid, SpeedoTD[playerid], 255);
    PlayerTextDrawFont(playerid, SpeedoTD[playerid], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, SpeedoTD[playerid], 0.4, 1.6);
    PlayerTextDrawColor(playerid, SpeedoTD[playerid], -1);
    PlayerTextDrawSetOutline(playerid, SpeedoTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, SpeedoTD[playerid], true);

    // Drift Score TD
    DriftScoreTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 320.0, "DRIFTING");
    PlayerTextDrawAlignment(playerid, DriftScoreTD[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColor(playerid, DriftScoreTD[playerid], 255);
    PlayerTextDrawFont(playerid, DriftScoreTD[playerid], TEXT_DRAW_FONT_3);
    PlayerTextDrawLetterSize(playerid, DriftScoreTD[playerid], 0.6, 2.0);
    PlayerTextDrawColor(playerid, DriftScoreTD[playerid], 0xFFFF00FF);
    PlayerTextDrawSetOutline(playerid, DriftScoreTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, DriftScoreTD[playerid], true);
    
    // Tandem Bonus TD
    TandemTD[playerid] = CreatePlayerTextDraw(playerid, 320.0, 350.0, "~g~TANDEM x2");
    PlayerTextDrawAlignment(playerid, TandemTD[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColor(playerid, TandemTD[playerid], 255);
    PlayerTextDrawFont(playerid, TandemTD[playerid], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, TandemTD[playerid], 0.3, 1.2);
    PlayerTextDrawColor(playerid, TandemTD[playerid], 0x00FF00FF);
    PlayerTextDrawSetOutline(playerid, TandemTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, TandemTD[playerid], true);
    
    // Fuel TD (Bottom Right)
    FuelTD[playerid] = CreatePlayerTextDraw(playerid, 510.0, 400.0, "FUEL: 100%");
    PlayerTextDrawAlignment(playerid, FuelTD[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColor(playerid, FuelTD[playerid], 255);
    PlayerTextDrawFont(playerid, FuelTD[playerid], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, FuelTD[playerid], 0.3, 1.2);
    PlayerTextDrawColor(playerid, FuelTD[playerid], 0x00FF00FF);
    PlayerTextDrawSetOutline(playerid, FuelTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, FuelTD[playerid], true);
    
    // Server Name (FiveM Style - Top Center)
    new PlayerText:ServerTD = CreatePlayerTextDraw(playerid, 320.0, 10.0, "~w~NUSANTARA ~y~DRIFT");
    PlayerTextDrawAlignment(playerid, ServerTD, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawBackgroundColor(playerid, ServerTD, 255);
    PlayerTextDrawFont(playerid, ServerTD, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, ServerTD, 0.4, 1.6);
    PlayerTextDrawColor(playerid, ServerTD, -1);
    PlayerTextDrawSetOutline(playerid, ServerTD, 1);
    PlayerTextDrawSetProportional(playerid, ServerTD, true);
    PlayerTextDrawShow(playerid, ServerTD);
}

stock HUD_Update(playerid, KMH, drift_score, bool:is_tandem, Float:fuel)
{
    new str[64];
    format(str, sizeof(str), "~b~%d ~w~KM/H", KMH);
    PlayerTextDrawSetString(playerid, SpeedoTD[playerid], str);
    PlayerTextDrawShow(playerid, SpeedoTD[playerid]);

    format(str, sizeof(str), "FUEL: %.0f%%", fuel);
    PlayerTextDrawSetString(playerid, FuelTD[playerid], str);
    
    // Color coding fuel
    if(fuel > 50.0) PlayerTextDrawColor(playerid, FuelTD[playerid], 0x00FF00FF);
    else if(fuel > 20.0) PlayerTextDrawColor(playerid, FuelTD[playerid], 0xFFFF00FF);
    else PlayerTextDrawColor(playerid, FuelTD[playerid], 0xFF0000FF);
    
    PlayerTextDrawShow(playerid, FuelTD[playerid]);

    if (drift_score > 0)
    {
        format(str, sizeof(str), "~y~DRIFT: ~w~%d", drift_score);
        PlayerTextDrawSetString(playerid, DriftScoreTD[playerid], str);
        PlayerTextDrawShow(playerid, DriftScoreTD[playerid]);
        
        if(is_tandem) PlayerTextDrawShow(playerid, TandemTD[playerid]);
        else PlayerTextDrawHide(playerid, TandemTD[playerid]);
    }
    else
    {
        PlayerTextDrawHide(playerid, DriftScoreTD[playerid]);
        PlayerTextDrawHide(playerid, TandemTD[playerid]);
    }
}
