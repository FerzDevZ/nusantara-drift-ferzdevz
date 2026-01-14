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
    Module: Map Loader
    Load custom drift tracks
*/

#include <open.mp>

stock Maps_Load()
{
    // Simple custom track additions (Example: Ramps at Airport)
    CreateObject(1633, 415.0, 2480.0, 15.5, 0.0, 0.0, 0.0); // Ramp
    CreateObject(1633, 415.0, 2460.0, 16.5, 0.0, 0.0, 0.0); // Ramp 2
    
    // More objects can be pasted here from Texture Studio / Map Editor
    
    print("Maps: Custom Drift Track Loaded.");
}
