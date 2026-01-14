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
    Module: Authentication
    Login, Register, and Welcome Bonus.
*/

#include <open.mp>

#define DIALOG_LOGIN    1000
#define DIALOG_REGISTER 1001

stock Auth_OnPlayerConnect(playerid)
{
    gPlayerLoggedIn[playerid] = false;
    TogglePlayerControllable(playerid, false); // Freeze
    SendClientMessage(playerid, -1, "Checking Account Status...");
    
    SetTimerEx("CheckAccount", 500, false, "i", playerid);
    return 1;
}

forward CheckAccount(playerid);
public CheckAccount(playerid)
{
    if(DB_IsPlayerRegistered(playerid))
    {
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Welcome back! Please enter your password:", "Login", "Quit");
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", "Welcome New Drifter!\nCreate a password to secure your account:", "Register", "Quit");
    }
}

stock ProcessAuthDialog(playerid, dialogid, response, listitem, inputtext[])
{
    if(!response) 
    {
        Kick(playerid);
        return 1;
    }
    
    if(dialogid == DIALOG_LOGIN)
    {
        if(strlen(inputtext) < 4) return ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Password too short! Try again:", "Login", "Quit");
        
        new hash[65];
        SHA256_Hash(inputtext, "", hash, sizeof(hash));
        
        if(!strcmp(hash, gPlayerData[playerid][password_hash]))
        {
            // Success
            gPlayerLoggedIn[playerid] = true;
            HUD_Create(playerid);
            SendClientMessage(playerid, 0x00FF00FF, "Login Successful! Welcome back.");
            
            DB_LoadPlayer(playerid); // Load remaining stats
            LoadPlayerClanName(playerid);
            InitMissions(playerid);
            LoadMapIcons(playerid);
            
            if(gPlayerData[playerid][tutorial_passed] == 0)
            {
                StartTutorial(playerid);
            }
            else
            {
                TogglePlayerControllable(playerid, true);
                SendClientMessage(playerid, -1, "Use /help to see all features!");
            }
        }
        else
        {
             ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Wrong Password! Try again:", "Login", "Quit");
        }
    }
    else if(dialogid == DIALOG_REGISTER)
    {
        if(strlen(inputtext) < 4) return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Register", "Password too short! Min 4 chars:", "Register", "Quit");
        
        // Save Password
        SHA256_Hash(inputtext, "", gPlayerData[playerid][password_hash], 65);
        
        // Init Stats
        gPlayerData[playerid][total_points] = 1000; // Starting Bonus Points
        gPlayerData[playerid][admin_level] = 0;
        
        gPlayerLoggedIn[playerid] = true;
        DB_SavePlayer(playerid); // Create Account logic
        
        TogglePlayerControllable(playerid, true);
        HUD_Create(playerid);
        SendClientMessage(playerid, 0x00FF00FF, "Account Created! You received 1000 Drift Points.");
        
        // STARTER CAR BONUS
        // Give Free Elegy (Drift Car)
        // Ensure BuyVehicle is available (garage.inc logic)
        // We simulate "Buy" with 0 cost logic or manually Insert.
        // BuyVehicle handles DB insert.
        BuyVehicle(playerid, 562, 0); 
        SendClientMessage(playerid, -1, "*** BONUS: You received a FREE Elegy! Check /mycars ***");
        
        LoadMapIcons(playerid);
        
        // Start Tutorial for New Players
        StartTutorial(playerid);
    }
    return 1;
}
