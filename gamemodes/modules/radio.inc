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
    Module: Radio System
    Vibes provider.
*/

#include <open.mp>

#define DIALOG_RADIO 600

// Example Streams (Ensure these are valid direct MP3/AAC links for best results)
// Using placeholder logic for demonstration if live links expire.
#define URL_EUROBEAT "http://hi5.1980s.fm/;"
#define URL_PHONK    "http://stream.zeno.fm/f3wvbbqmdg8uv" // Example Phonk station
#define URL_LOFI     "http://stream.zeno.fm/0r0xa792kwzuv" // Example Lofi station

stock ShowRadioDialog(playerid)
{
    new str[256];
    format(str, sizeof(str), "Station\tGenre\n");
    format(str, sizeof(str), "%sEurobeat FM\tHigh BPM Drift\n", str);
    format(str, sizeof(str), "%sPhonk Radio\tAggressive Drift\n", str);
    format(str, sizeof(str), "%sLofi Chill\tRelaxing\n", str);
    format(str, sizeof(str), "%s[Stop Radio]\tTurn off\n", str);
    
    ShowPlayerDialog(playerid, DIALOG_RADIO, DIALOG_STYLE_TABLIST_HEADERS, "Car Radio", str, "Play", "Close");
    return 1;
}

stock ProcessRadioDialog(playerid, response, listitem)
{
    if(!response) return 1;
    
    switch(listitem)
    {
        case 0: 
        {
            PlayAudioStreamForPlayer(playerid, URL_EUROBEAT);
            ShowNotification(playerid, "Radio", "Playing: Eurobeat FM 🎵", 3000);
        }
        case 1: 
        {
            PlayAudioStreamForPlayer(playerid, URL_PHONK);
             ShowNotification(playerid, "Radio", "Playing: Phonk Radio 🎵", 3000);
        }
        case 2: 
        {
            PlayAudioStreamForPlayer(playerid, URL_LOFI);
             ShowNotification(playerid, "Radio", "Playing: Lofi Chill 🎵", 3000);
        }
        case 3: 
        {
            StopAudioStreamForPlayer(playerid);
             ShowNotification(playerid, "Radio", "Radio Stopped.", 3000);
        }
    }
    return 1;
}
