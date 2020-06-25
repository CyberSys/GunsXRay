#include "StdAfx.h"
#include "xr_Cartridge.h"
#include "EngineFriends.h"
#include "HudItemUtils.h"

#include "WeaponMagazinedWGrenade.h"

namespace GunslingerMod
{
CCartridge* GetGrenadeCartridgeFromGLVector(CWeaponMagazinedWGrenade* wpn, u32 index)
{
    if (wpn == nullptr)
    {
        return nullptr;
    }

    EGunsAddonStatus gl_status = GetGLStatus(wpn);
    if ((gl_status == addonStatusDisabled) || ((gl_status = addonStatusAttachable) && !(IsGLAttached(wpn))) ||
        (index >= GetAmmoInGLCount(wpn)))
    {
        return nullptr;
    }

    if (IsGrenadeMode(wpn))
    {
        return &(wpn->m_magazine[index]);
    }
    else
    {
        return &(wpn->m_magazine2[index]);
    }
}

u8 GetAmmoTypeIndex(CWeapon* wpn, bool second) { return EngineFriendWrapper::GetAmmoTypeIndex(wpn, second); }

u8 GetCartridgeType(CCartridge* c) { return EngineFriendWrapper::GetCartridgeTypeIdx(c); }
} // namespace GunslingerMod
