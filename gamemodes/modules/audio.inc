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
    Module: Dynamic Engine Sounds
    Streaming Audio for Immersion
*/

#include <open.mp>

// URLs (Placeholders - Ensure these are direct links to .mp3/.ogg)
// Best practice: Use a dedicated hosting or GitHub Raw
#define URL_2JZ "https://github.com/HM-LC/Samp-Engine-Sounds/raw/main/2jz.mp3" // Example fake URL for structure
#define URL_V12 "https://upload.wikimedia.org/wikipedia/commons/6/6c/V12_Type_Engine_Sound.ogg" 

stock ProcessVehicleAudio(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        new model = GetVehicleModel(vehicleid);
        
        if(model == 562 || model == 560 || model == 559) // Elegy, Sultan, Jester (JDM)
        {
            // Use the defined constant. Note: You must replace URL_2JZ with a WORKING direct link (e.g. upload to Discord).
            PlayAudioStreamForPlayer(playerid, URL_2JZ); 
            SendClientMessage(playerid, 0x00FF00FF, "Audio: RB26 Engine Sound Active 🎵");
        }
        else if(model == 411) // Infernus
        {
             PlayAudioStreamForPlayer(playerid, URL_V12);
             SendClientMessage(playerid, 0x00FF00FF, "Audio: V12 Engine Sound Active 🎵");
        }
    }
    
    if(oldstate == PLAYER_STATE_DRIVER)
    {
        StopAudioStreamForPlayer(playerid);
        SendClientMessage(playerid, -1, "Audio: Engine Sound Stopped.");
    }
}
