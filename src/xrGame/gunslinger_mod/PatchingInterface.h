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
};
} // namespace GunslingerMod
