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
    Module: Dynamic Weather & Time
    Sync with Real World + Weather Cycle.
*/

#include <open.mp>

new gGameWeather = 1; // Default Sunny
new gWeatherTimer = 0; // Duration counter

// Helpers
stock IsWeatherWet(weatherid)
{
    if(weatherid == 8 || weatherid == 16 || weatherid == 2) return 1;
    return 0;
}

stock ProcessWeather()
{
    // 1. Sync Time
    new h, m, s;
    gettime(h, m, s);
    SetWorldTime(h); 
    
    // 2. Weather Cycle (Runs every sec)
    gWeatherTimer++;
    
    // Change weather every 10 mins (600 secs) approx
    // But randomized
    if(gWeatherTimer > 600 && random(60) == 1) 
    {
        gWeatherTimer = 0;
        
        // Logic: 
        // 60% Chance Clear (1, 10, 11)
        // 20% Chance Cloudy (4, 7)
        // 10% Chance Rain (8, 16)
        // 10% Chance Fog (9, 20)
        
        new r = random(100);
        new old_w = gGameWeather;
        
        if(r < 60) // Clear
        {
            if(random(2)==0) gGameWeather = 1; 
            else gGameWeather = 10;
        }
        else if(r < 80) // Cloudy
        {
             gGameWeather = 4;
        }
        else if(r < 90) // Rain
        {
             gGameWeather = 8;
        }
        else // Fog
        {
             gGameWeather = 20; // 9 is fog, 20 also
        }
        
        SetWeather(gGameWeather);
        
        // Notify if changed dramatically
        if(IsWeatherWet(gGameWeather) && !IsWeatherWet(old_w))
        {
             SendClientMessageToAll(0xFF0000FF, "WARNING: Heavy Rain! Track is slippery!");
        }
        else if(!IsWeatherWet(gGameWeather) && IsWeatherWet(old_w))
        {
             SendClientMessageToAll(0x00FF00FF, "INFO: Rain stopped. Track drying up.");
        }
    }
}
