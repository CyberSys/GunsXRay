#pragma once

class CWeapon;

namespace GunslingerMod
{
void CreateWpnBufIfNeeded(CWeapon* wpn);
void OnWeaponExplode_AfterAnim(CWeapon* wpn, s32 param);
} // namespace GunslingerMod
