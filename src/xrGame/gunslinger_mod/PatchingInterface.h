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
    };
};
} // namespace GunslingerMod
