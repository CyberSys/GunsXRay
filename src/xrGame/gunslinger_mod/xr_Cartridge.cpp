#include "StdAfx.h"
#include "xr_Cartridge.h"
#include "EngineFriends.h"
#include "HudItemUtils.h"

#include "WeaponMagazinedWGrenade.h"

namespace GunslingerMod
{
CCartridge* GetCartridgeFromMagVector(CWeapon* wpn, u32 index)
{
    if ((wpn == nullptr) || (index >= GetAmmoInMagCount(wpn)))
    {
        return nullptr;
    };

    EGunsAddonStatus gl_status = GetGLStatus(wpn);
    if (((gl_status == addonStatusPermanent) || ((gl_status == addonStatusAttachable) && (IsGLAttached(wpn)))) &&
        IsGLEnabled(wpn))
    {
        CWeaponMagazinedWGrenade* wpn_gl = static_cast<CWeaponMagazinedWGrenade*>(wpn);
        return &(wpn_gl->m_magazine2[index]);
    }
    else
    {
        return &(wpn->m_magazine[index]);
    }
}

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

u8 GetAmmoTypeChangingStatus(CWeapon* wpn) { return EngineFriendWrapper::GetAmmoTypeChangingStatus(wpn); }

u8 GetAmmoTypeIndex(CWeapon* wpn, bool second) { return EngineFriendWrapper::GetAmmoTypeIndex(wpn, second); }

u8 GetAmmoTypeToReload(CWeapon* wpn)
{ //Вернет индекс для текущего активного магазина (т.е. разные в режиме подствола и обычном)
    u8 result = GetAmmoTypeChangingStatus(wpn);
    if (result == u8(-1))
        result = GetAmmoTypeIndex(wpn);
    return result;
}

u8 GetCartridgeType(CCartridge* c) { return EngineFriendWrapper::GetCartridgeTypeIdx(c); }
} // namespace GunslingerMod
