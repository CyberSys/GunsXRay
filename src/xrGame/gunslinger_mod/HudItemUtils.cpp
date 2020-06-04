#include "StdAfx.h"
#include "HudItemUtils.h"
#include "gunsl_config.h"

#include "WeaponMagazinedWGrenade.h"
#include "WeaponBinoculars.h"

namespace GunslingerMod
{
pcstr GetSection(CHudItemObject* wpn) { return EngineFriendWrapper::GetItemSection(wpn); }

pcstr GetCurrentScopeSection(CWeapon* wpn)
{
    u8 id = EngineFriendWrapper::GetCurScopeId(wpn);
    auto scopes = EngineFriendWrapper::GetScopesVector(wpn);
    R_ASSERT((scopes.size() > 0) && (scopes.size() > id));
    return scopes[id].c_str();
}

void SetWeaponMisfireStatus(CWeapon* wpn, bool status) { EngineFriendWrapper::SetWeaponMisfireStatus(wpn, status); }

bool WpnCanShoot(CHudItemObject* wpn)
{
    return (dynamic_cast<CWeaponMagazined*>(wpn) != nullptr) && (dynamic_cast<CWeaponBinoculars*>(wpn) == nullptr);
}

bool IsBino(CHudItemObject* wpn) { return (dynamic_cast<CWeaponBinoculars*>(wpn) != nullptr); }

EGunsAddonStatus GetScopeStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = EngineFriendWrapper::GetAddonStatus(wpn_casted, addonTypeScope);
    }
    return result;
}

EGunsAddonStatus GetSilencerStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = EngineFriendWrapper::GetAddonStatus(wpn_casted, addonTypeSilencer);
    }
    return result;
}

bool IsGrenadeMode(CWeapon* wpn)
{
    EGunsAddonStatus status = GetGLStatus(wpn);
    bool has_gl = (status == addonStatusPermanent) || ((status == addonStatusAttachable) && IsGLAttached(wpn));
    bool res = has_gl && IsGLEnabled(wpn);
    return res;
}

bool IsSilencerAttached(CWeapon* wpn) { return EngineFriendWrapper::GetAddonStatus(wpn, addonTypeSilencer); }

bool IsScopeAttached(CWeapon* wpn) { return EngineFriendWrapper::GetAddonStatus(wpn, addonTypeScope); }

bool IsGLAttached(CWeapon* wpn) { return EngineFriendWrapper::GetAddonStatus(wpn, addonTypeGL); }

EGunsAddonStatus GetGLStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = EngineFriendWrapper::GetAddonStatus(wpn_casted, addonTypeGL);
    }
    return result;
}
bool IsGLEnabled(CWeapon* wpn)
{
    bool res = false;
    CWeaponMagazinedWGrenade* wpn_g = dynamic_cast<CWeaponMagazinedWGrenade*>(wpn);
    if (wpn_g != nullptr)
    {
        res = EngineFriendWrapper::GetGrenadeMode(wpn_g);
    }
    return res;
}

bool IsAimNow(CHudItemObject* wpn)
{
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    return wpn_casted->IsZoomed();
}

bool FindBoolValueInUpgradesDef(CInventoryItem* itm, pcstr key, bool def, bool scan_after_nodefault)
{
    bool result = def;
    for (size_t i = 0; i < EngineFriendWrapper::GetInstalledUpgradesCount(itm); ++i)
    {
        pcstr str = EngineFriendWrapper::GetInstalledUpgradeSection(itm, i);
        str = game_ini_read_string(str, "section");
        if (game_ini_line_exist(str, key))
        {
            result = game_ini_r_bool(str, key);
            if (!scan_after_nodefault && (result != def))
                break;
        }
    }
    return result;
}
} // namespace GunslingerMod
