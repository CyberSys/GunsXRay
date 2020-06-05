#pragma once

class IReader;
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
        };
    };
};
} // namespace GunslingerMod
