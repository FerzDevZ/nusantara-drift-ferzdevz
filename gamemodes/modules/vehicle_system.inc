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
    Module: Vehicle System
    Spawn and mod vehicles for drifting
*/

#include <open.mp>

stock SpawnDriftVehicle(playerid, modelid)
{
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    
    new vehicleid = CreateVehicle(modelid, x, y, z, a, -1, -1, 100);
    InitVehicleFuel(vehicleid);
    PutPlayerInVehicle(playerid, vehicleid, 0);
    
    // Auto Tune (Nitro & Wheels)
    AddVehicleComponent(vehicleid, 1010); // Nitro 10x
    AddVehicleComponent(vehicleid, 1073); // Shadow Wheels
    
    SendClientMessage(playerid, 0x00FF00FF, "Kendaraan drift berhasil dimunculkan!");
    return vehicleid;
}
