#pragma once

class CCartridge;
class CWeaponMagazinedWGrenade;

namespace GunslingerMod
{
CCartridge* GetCartridgeFromMagVector(CWeapon* wpn, u32 index);
CCartridge* GetGrenadeCartridgeFromGLVector(CWeaponMagazinedWGrenade* wpn, u32 index);

u8 GetAmmoTypeChangingStatus(CWeapon* wpn);

u8 GetAmmoTypeIndex(CWeapon* wpn, bool second = false);
//Вернет индекс для текущего активного магазина (т.е. разные в режиме подствола и обычном)
u8 GetAmmoTypeToReload(CWeapon* wpn);

u8 GetCartridgeType(CCartridge* c);
} // namespace GunslingerMod
