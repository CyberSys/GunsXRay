#include "StdAfx.h"
#include "HudItemUtils.h"
#include "WeaponMagazinedWGrenade.h"
#include "WeaponBinoculars.h"

namespace GunslingerMod
{
pcstr GetSection(CHudItemObject* wpn) { return wpn->m_section_id.c_str(); }
pcstr GetCurrentScopeSection(CWeapon* wpn)
{
    u8 id = wpn->Gunsl_GetCurScopeId();
    auto scopes = wpn->Gunsl_GetScopesVector();
    R_ASSERT((scopes.size() > 0) && (scopes.size() > id));
    return scopes[id].c_str();
}

bool IsBino(CHudItemObject* wpn) { return (dynamic_cast<CWeaponBinoculars*>(wpn) != nullptr); }

EGunsAddonStatus GetScopeStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = static_cast<EGunsAddonStatus>(wpn_casted->get_ScopeStatus());
    }
    return result;
}

EGunsAddonStatus GetSilencerStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = static_cast<EGunsAddonStatus>(wpn_casted->get_SilencerStatus());
    }
    return result;
}

EGunsAddonStatus GetGLStatus(CHudItemObject* wpn)
{
    EGunsAddonStatus result = addonStatusDisabled;
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    if (wpn_casted != nullptr)
    {
        result = static_cast<EGunsAddonStatus>(wpn_casted->get_GrenadeLauncherStatus());
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

u32 get_addons_state(CWeapon* wpn) { return wpn->GetAddonsState(); }

bool IsSilencerAttached(CWeapon* wpn)
{
    u32 state = get_addons_state(wpn);
    return (state & CSE_ALifeItemWeapon::eWeaponAddonSilencer != 0);
}
bool IsScopeAttached(CWeapon* wpn)
{
    u32 state = get_addons_state(wpn);
    return (state & CSE_ALifeItemWeapon::eWeaponAddonScope != 0);
}
bool IsGLAttached(CWeapon* wpn)
{
    u32 state = get_addons_state(wpn);
    return (state & CSE_ALifeItemWeapon::eWeaponAddonGrenadeLauncher != 0);
}

bool IsGLEnabled(CWeapon* wpn)
{
    bool res = false;
    CWeaponMagazinedWGrenade* wpn_g = dynamic_cast<CWeaponMagazinedWGrenade*>(wpn);
    if (wpn_g != nullptr)
    {
        res = wpn_g->Gunsl_GetGrenadeMode();
    }
    return res;
}

bool IsAimNow(CHudItemObject* wpn)
{
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    return wpn_casted->IsZoomed();
}
} // namespace GunslingerMod
