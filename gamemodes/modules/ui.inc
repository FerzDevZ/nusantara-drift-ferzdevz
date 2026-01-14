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
    Module: Modern UI & Notifications
    Sleek notifications instead of spammy chat.
*/

#include <open.mp>

// Notification TextDraws
new PlayerText:NotificationTD[MAX_PLAYERS];
new PlayerText:NotificationTitle[MAX_PLAYERS];
new PlayerText:NotificationMsg[MAX_PLAYERS];
new NotificationTimer[MAX_PLAYERS];

stock InitUI(playerid)
{
    // Background Box (Right Side)
    NotificationTD[playerid] = CreatePlayerTextDraw(playerid, 500.000000, 150.000000, "_");
    PlayerTextDrawLetterSize(playerid, NotificationTD[playerid], 0.000000, 6.000000);
    PlayerTextDrawTextSize(playerid, NotificationTD[playerid], 630.000000, 0.000000); // Width
    PlayerTextDrawAlignment(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawColor(playerid, NotificationTD[playerid], -1);
    PlayerTextDrawUseBox(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawBoxColor(playerid, NotificationTD[playerid], 0x000000AA); // Transparent Black
    PlayerTextDrawSetShadow(playerid, NotificationTD[playerid], 0);
    PlayerTextDrawSetOutline(playerid, NotificationTD[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, NotificationTD[playerid], 255);
    PlayerTextDrawFont(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, NotificationTD[playerid], 1);

    // Title
    NotificationTitle[playerid] = CreatePlayerTextDraw(playerid, 505.000000, 152.000000, "Notification");
    PlayerTextDrawLetterSize(playerid, NotificationTitle[playerid], 0.400000, 1.600000);
    PlayerTextDrawAlignment(playerid, NotificationTitle[playerid], 1);
    PlayerTextDrawColor(playerid, NotificationTitle[playerid], -1); // White
    PlayerTextDrawSetShadow(playerid, NotificationTitle[playerid], 0);
    PlayerTextDrawSetOutline(playerid, NotificationTitle[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, NotificationTitle[playerid], 255);
    PlayerTextDrawFont(playerid, NotificationTitle[playerid], 1);
    PlayerTextDrawSetProportional(playerid, NotificationTitle[playerid], 1);

    // Message (Multi-line approximation)
    NotificationMsg[playerid] = CreatePlayerTextDraw(playerid, 505.000000, 170.000000, "Message goes here...");
    PlayerTextDrawLetterSize(playerid, NotificationMsg[playerid], 0.250000, 1.200000);
    PlayerTextDrawAlignment(playerid, NotificationMsg[playerid], 1);
    PlayerTextDrawColor(playerid, NotificationMsg[playerid], 0xCCCCCCFF); // Light Grey
    PlayerTextDrawSetShadow(playerid, NotificationMsg[playerid], 0);
    PlayerTextDrawSetOutline(playerid, NotificationMsg[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, NotificationMsg[playerid], 255);
    PlayerTextDrawFont(playerid, NotificationMsg[playerid], 1);
    PlayerTextDrawSetProportional(playerid, NotificationMsg[playerid], 1);
}

stock ShowNotification(playerid, title[], message[], duration=5000)
{
    // Hide previous if exists
    PlayerTextDrawHide(playerid, NotificationTD[playerid]);
    PlayerTextDrawHide(playerid, NotificationTitle[playerid]);
    PlayerTextDrawHide(playerid, NotificationMsg[playerid]);
    KillTimer(NotificationTimer[playerid]);

    // Update Text
    PlayerTextDrawSetString(playerid, NotificationTitle[playerid], title);
    PlayerTextDrawSetString(playerid, NotificationMsg[playerid], message);

    // Show
    PlayerTextDrawShow(playerid, NotificationTD[playerid]);
    PlayerTextDrawShow(playerid, NotificationTitle[playerid]);
    PlayerTextDrawShow(playerid, NotificationMsg[playerid]);
    
    // Auto Hide Timer
    NotificationTimer[playerid] = SetTimerEx("HideNotification", duration, false, "i", playerid);
}

forward HideNotification(playerid);
public HideNotification(playerid)
{
    PlayerTextDrawHide(playerid, NotificationTD[playerid]);
    PlayerTextDrawHide(playerid, NotificationTitle[playerid]);
    PlayerTextDrawHide(playerid, NotificationMsg[playerid]);
}
