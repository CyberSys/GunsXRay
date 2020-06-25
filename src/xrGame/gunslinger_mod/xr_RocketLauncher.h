#pragma once

class CRocketLauncher;
class CCustomRocket;
class CWeaponMagazinedWGrenade;

namespace GunslingerMod
{
u32 GetRocketsCount(CRocketLauncher* rl);
CCustomRocket* GetRocket(CRocketLauncher* rl, u32 index);
void UnloadRockets(CRocketLauncher* rl);

void DetachRocket(CRocketLauncher* rl, u16 rocket_id, bool bLaunch);

void CWeaponMagazinedWGrenade__LaunchGrenade(CWeaponMagazinedWGrenade* wpn);

void virtual_CCustomRocket__Contact(CCustomRocket* r, Fvector3* pos, Fvector3* normal);
} // namespace GunslingerMod
