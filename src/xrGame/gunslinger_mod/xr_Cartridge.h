#pragma once

class CCartridge;
class CWeaponMagazinedWGrenade;

namespace GunslingerMod
{
CCartridge* GetGrenadeCartridgeFromGLVector(CWeaponMagazinedWGrenade* wpn, u32 index);

u8 GetAmmoTypeIndex(CWeapon* wpn, bool second);

u8 GetCartridgeType(CCartridge* c);
} // namespace GunslingerMod
