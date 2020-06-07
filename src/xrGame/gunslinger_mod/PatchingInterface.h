#pragma once

class IReader;
class IWriter;
class CWeapon;

namespace GunslingerMod
{
class PatchingInterface
{
public:
    class CWeapon_class
    {
    public:
        static void load(CWeapon* wpn, IReader* packet);
        static void net_Spawn_middle(CWeapon* wpn);
    };

    class CLevel_class
    {
    public:
        static bool Connect2Server_ForceAuthCalc();
    };

    class CSavedGameWrapper_class
    {
    public:
        static bool valid_saved_game_override(IReader* reader);
    };

    class CALifeStorageManager_class
    {
    public:
        static void save(IWriter* w);
    };

    class ScriptExported
    {
    public:
        class level
        {
        public:
            static bool is_ui_shown();
            static bool indicators_shown();
            static bool electronics_break();
            static bool electronics_restore();
            static bool electronics_reset();
            static bool electronics_apply();
            static s32 valid_saved_game_int(u16 unused, pcstr save_name);
        };
    };

    static void global_on_game_started_init();
};
} // namespace GunslingerMod
