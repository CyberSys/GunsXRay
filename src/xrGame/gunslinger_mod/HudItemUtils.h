#pragma once
#include "Weapon.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
bool FindBoolValueInUpgradesDef(CInventoryItem* itm, pcstr key, bool def, bool scan_after_nodefault);

pcstr GetSection(CHudItemObject* wpn);
pcstr GetCurrentScopeSection(CWeapon* wpn);

void SetWeaponMisfireStatus(CWeapon* wpn, bool status);

bool WpnCanShoot(CHudItemObject* wpn);
bool IsBino(CHudItemObject* wpn);

EGunsAddonStatus GetScopeStatus(CHudItemObject* wpn);
EGunsAddonStatus GetSilencerStatus(CHudItemObject* wpn);
EGunsAddonStatus GetGLStatus(CHudItemObject* wpn);

bool IsSilencerAttached(CWeapon* wpn);
bool IsScopeAttached(CWeapon* wpn);
bool IsGLAttached(CWeapon* wpn);
bool IsGLEnabled(CWeapon* wpn);
bool IsGrenadeMode(CWeapon* wpn);

bool IsAimNow(CHudItemObject* wpn);
} // namespace GunslingerMod
