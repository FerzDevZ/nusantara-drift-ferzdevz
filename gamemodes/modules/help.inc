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
    Module: Help System
    Command center.
*/

#include <open.mp>

#define DIALOG_HELP 500

stock ShowHelpDialog(playerid)
{
    new str[1024];
    format(str, sizeof(str), "Fitur\tPerintah\tDeskripsi\n");
    format(str, sizeof(str), "%sKendaraan\t/buycar, /mycars, /v\tBeli, Kelola, atau Spawn mobil\n", str);
    format(str, sizeof(str), "%sDrifting\t/drift, /teles, /fill\tLokasi drift & Isi bensin\n", str);
    format(str, sizeof(str), "%sModifikasi\t/sticker, /shop, /visual\tEditor visual & Neon\n", str);
    format(str, sizeof(str), "%sSosial\t/createclan, /invite, /kickclan\tSistem Clan\n", str);
    format(str, sizeof(str), "%sReplay\t/rec, /play, /duel, /spec\tRekam & Duel Ghost\n", str);
    format(str, sizeof(str), "%sAdmin\t/kick, /ban, /veh, /goto\tPerintah Staff (Level required)\n", str);
    format(str, sizeof(str), "%sLainnya\t/radio, /skiptutorial\tMusik & Lewati tutorial\n", str);
    format(str, sizeof(str), "%sTips\tGunakan Spasi/Shift\tUntuk rem tangan saat drifting!\n", str);
    
    ShowPlayerDialog(playerid, DIALOG_HELP, DIALOG_STYLE_TABLIST_HEADERS, "Pusat Bantuan FerzRP", str, "Tutup", "");
    return 1;
}
