#pragma once
#include "Weapon.h"

namespace GunslingerMod
{
enum EGunsAddonStatus
{
    addonStatusDisabled = ALife::eAddonDisabled,
    addonStatusPermanent = ALife::eAddonPermanent,
    addonStatusAttachable = ALife::eAddonAttachable,
};

pcstr GetSection(CHudItemObject* wpn);
pcstr GetCurrentScopeSection(CWeapon* wpn);

bool IsBino(CHudItemObject* wpn);

EGunsAddonStatus GetScopeStatus(CHudItemObject* wpn);
EGunsAddonStatus GetSilencerStatus(CHudItemObject* wpn);
EGunsAddonStatus GetGLStatus(CHudItemObject* wpn);

u32 get_addons_state(CWeapon* wpn);
bool IsSilencerAttached(CWeapon* wpn);
bool IsScopeAttached(CWeapon* wpn);
bool IsGLAttached(CWeapon* wpn);
bool IsGLEnabled(CWeapon* wpn);
bool IsGrenadeMode(CWeapon* wpn);

bool IsAimNow(CHudItemObject* wpn);
} // namespace GunslingerMod
