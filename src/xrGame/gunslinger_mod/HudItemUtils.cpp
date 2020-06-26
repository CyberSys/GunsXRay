#include "StdAfx.h"
#include "HudItemUtils.h"
#include "gunsl_config.h"
#include "ActorUtils.h"

#include "WeaponMagazinedWGrenade.h"
#include "WeaponBinoculars.h"

namespace GunslingerMod
{
void ChangeParticles(CWeapon* wpn, pcstr name, EGunsWeaponParticles particle_type)
{
    EngineFriendWrapper::SetWeaponParticle(wpn, name, particle_type);
}

void PlayAnimIdle(CHudItemObject* wpn) { EngineFriendWrapper::PlayAnimIdle(wpn); }

bool IsScopeAttached(CWeapon* wpn) { return EngineFriendWrapper::IsAddonAttached(wpn, addonTypeScope); }

bool IsSilencerAttached(CWeapon* wpn) { return EngineFriendWrapper::IsAddonAttached(wpn, addonTypeSilencer); }

bool IsGLAttached(CWeapon* wpn) { return EngineFriendWrapper::IsAddonAttached(wpn, addonTypeGL); }

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

bool IsWeaponJammed(CWeapon* wpn) { return EngineFriendWrapper::IsWeaponJammed(wpn); }

u32 GetInstalledUpgradesCount(CInventoryItem* itm) { return EngineFriendWrapper::GetInstalledUpgradesCount(itm); }
pcstr GetInstalledUpgradeSection(CInventoryItem* itm, u32 index)
{
    return EngineFriendWrapper::GetInstalledUpgradeSection(itm, index);
}

bool FindBoolValueInUpgradesDef(CInventoryItem* itm, pcstr key, bool def, bool scan_after_nodefault)
{
    bool result = def;
    for (u32 i = 0; i < GetInstalledUpgradesCount(itm); ++i)
    {
        pcstr str = GetInstalledUpgradeSection(itm, i);
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

float ModifyFloatUpgradedValue(CInventoryItem* itm, pcstr key, float def)
{
    float result = def;
    for (u32 i = 0; i < GetInstalledUpgradesCount(itm); ++i)
    {
        pcstr str = GetInstalledUpgradeSection(itm, i);
        str = game_ini_read_string(str, "section");
        if (game_ini_line_exist(str, key))
            result = result + game_ini_r_single_def(str, key, 0);
    }
    return result;
}

pcstr GetSection(CHudItemObject* wpn) { return EngineFriendWrapper::GetItemSection(wpn); }

u16 GetID(CHudItemObject* wpn) { return EngineFriendWrapper::GetID(wpn); }

pcstr GetHUDSection(CHudItemObject* wpn) { return EngineFriendWrapper::GetHUDSection(wpn); }

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

pcstr GetCurrentScopeSection(CWeapon* wpn) { return GetScopeSection(wpn, GetCurrentScopeIndex(wpn)); }
u32 GetScopesCount(CWeapon* wpn) { return EngineFriendWrapper::GetScopesVector(wpn).size(); }
s32 GetCurrentScopeIndex(CWeapon* wpn) { return EngineFriendWrapper::GetCurScopeId(wpn); }
pcstr GetScopeSection(CWeapon* wpn, u32 index)
{
    auto scopes = EngineFriendWrapper::GetScopesVector(wpn);
    R_ASSERT((scopes.size() > 0) && (scopes.size() > index));
    return scopes[index].c_str();
}

void SetWpnVisual(CHudItemObject* wpn, pcstr name) { EngineFriendWrapper::SetNameVisual(wpn, name); }
void SetObjectVisual(CGameObject* o, pcstr name) { EngineFriendWrapper::SetNameVisual(o, name); }

void SetHUDSection(CHudItemObject* wpn, pcstr new_hud_section)
{
    EngineFriendWrapper::SetHUDSection(wpn, new_hud_section);
}

u32 GetAmmoInMagCount(CWeapon* wpn)
{
    u32 result = 0;
    if (wpn == nullptr)
        return result;

    if (IsGrenadeMode(wpn))
    {
        result = EngineFriendWrapper::GetAmmoInSecondVectorCount(static_cast<CWeaponMagazinedWGrenade*>(wpn));
    }
    else
    {
        result = EngineFriendWrapper::GetAmmoInVectorCount(wpn);
    }

    return result;
}

u32 GetAmmoInGLCount(CWeapon* wpn)
{
    u32 result = 0;
    if (wpn == nullptr)
        return result;

    EGunsAddonStatus gl_status = GetGLStatus(wpn);
    if ((gl_status == addonStatusDisabled) || ((gl_status == addonStatusAttachable) && !IsGLAttached(wpn)))
        return result;

    if (IsGrenadeMode(wpn))
    {
        result = EngineFriendWrapper::GetAmmoInVectorCount(wpn);
    }
    else
    {
        result = EngineFriendWrapper::GetAmmoInSecondVectorCount(static_cast<CWeaponMagazinedWGrenade*>(wpn));
    }

    return result;
}

IGameObject* GetOwner(CHudItemObject* wpn) { return EngineFriendWrapper::GetParent(wpn); }

bool IsAimNow(CHudItemObject* wpn)
{
    CWeapon* wpn_casted = dynamic_cast<CWeapon*>(wpn);
    return wpn_casted->IsZoomed();
}

float GetAimFactor(CWeapon* wpn) { return EngineFriendWrapper::GetZoomFactor(wpn); }

bool IsTriStateReload(CWeapon* wpn) { return EngineFriendWrapper::IsTriStateReload(wpn); }

EWeaponHudState GetCurrentState(CHudItemObject* wpn)
{
    return static_cast<EWeaponHudState>(EngineFriendWrapper::GetCurrentState(wpn));
}

pcstr GetSilencerSection(CWeapon* wpn) { return EngineFriendWrapper::GetSilencerName(wpn); }

void DetachGLRight(CWeaponMagazinedWGrenade* wpn)
{
    pcstr addon_name = GetGLSection(wpn);
    if (addon_name == nullptr)
    {
        LogText("WpnUtils.DetachGLRight: Weapon has no GL!", true);
        return;
    }
    CreateObjectToActor(addon_name);
    EngineFriendWrapper::SetAddonAttachedStatus(wpn, addonTypeGL, false);

    //[bug] баг в CWeaponMagazinedWGrenade::Detach - если оружие при отсоединении ПГ находится не в состоянии стрельбы
    //гренами, то подствол отсоединится без разряжания грены
    if (!IsGLEnabled(wpn))
    {
        PerformSwitchGL(wpn);
    }

    virtual_CWeaponMagazined__UnloadMagazine(static_cast<CWeaponMagazined*>(wpn), true);
    PerformSwitchGL(wpn);

    EngineFriendWrapper::UpdateAddonsVisibility(wpn);

    if (GetCurrentState(wpn) == EHudStates__eIdle)
    {
        PlayAnimIdle(wpn);
    }
}

void DetachAddon(CWeapon* wpn, EGunsAddonType addon_type)
{
    pcstr addon_name = nullptr;

    switch (addon_type)
    {
    case addonTypeScope:
    {
        addon_name = game_ini_read_string(GetCurrentScopeSection(wpn), "scope_name");
        break;
    }
    case addonTypeSilencer:
    {
        addon_name = GetSilencerSection(wpn);
        break;
    }
    case addonTypeGL:
    {
        DetachGLRight(static_cast<CWeaponMagazinedWGrenade*>(wpn));
        return;
    }
    default:
    {
        LogText("WpnUtils.DetachAddon: Invalid addon type!", true);
        return;
    }
    }

    if (addon_name == nullptr)
        return;

    CreateObjectToActor(addon_name);
    EngineFriendWrapper::UpdateAddonsVisibility(wpn);
    EngineFriendWrapper::InitAddons(wpn);
    if (addon_type == addonTypeScope)
    {
        EngineFriendWrapper::SetCurScopeId(wpn, 0);
    }
}

pcstr GetGLSection(CWeaponMagazinedWGrenade* wpn) { return EngineFriendWrapper::GetGLSection(wpn); }

float GetCurrentCondition(CHudItemObject* wpn) { return EngineFriendWrapper::GetCondition(wpn); }
const Fvector3* GetPosition(CHudItemObject* wpn) { return &(EngineFriendWrapper::GetXForm(wpn)->c); }
const Fvector3* GetOrientation(CHudItemObject* wpn) { return &(EngineFriendWrapper::GetXForm(wpn)->k); }
pcstr GetActualCurrentAnim(CHudItemObject* wpn) { return EngineFriendWrapper::GetActualCurrentAnim(wpn); }

void SetWeaponMisfireStatus(CWeapon* wpn, bool status) { EngineFriendWrapper::SetWeaponMisfireStatus(wpn, status); }

bool WpnCanShoot(CHudItemObject* wpn)
{
    return (dynamic_cast<CWeaponMagazined*>(wpn) != nullptr) && (dynamic_cast<CWeaponBinoculars*>(wpn) == nullptr);
}

bool IsBino(CHudItemObject* wpn) { return (dynamic_cast<CWeaponBinoculars*>(wpn) != nullptr); }

TimeTicks GetAnimTimeState(CHudItemObject* wpn, EAnmTimeOffsetType what)
{
    switch (what)
    {
    case ANM_TIME_CUR: return EngineFriendWrapper::GetMotionCurrTime(wpn);
    case ANM_TIME_START: return EngineFriendWrapper::GetMotionStartTime(wpn);
    case ANM_TIME_END: return EngineFriendWrapper::GetMotionEndTime(wpn);
    }
    return 0;
}

bool virtual_Action(CHudItemObject* wpn, u32 cmd, u32 flags)
{
    return EngineFriendWrapper::virtual_Action(wpn, cmd, flags);
}

void virtual_CHudItem_SwitchState(CHudItem* itm, EWeaponHudState state)
{
    EngineFriendWrapper::virtual_CHudItem_SwitchState(itm, state);
}

void virtual_CWeaponMagazined__UnloadMagazine(CWeaponMagazined* wpn, bool spawn_ammo)
{
    EngineFriendWrapper::virtual_CWeaponMagazined__UnloadMagazine(wpn, spawn_ammo);
}

attachable_hud_item* CHudItem__HudItemData(CHudItem* hi) { return EngineFriendWrapper::GetHudItemData(hi); }

bool IsGrenadeMode(CWeapon* wpn)
{
    EGunsAddonStatus status = GetGLStatus(wpn);
    bool has_gl = (status == addonStatusPermanent) || ((status == addonStatusAttachable) && IsGLAttached(wpn));
    bool res = has_gl && IsGLEnabled(wpn);
    return res;
}

void PerformSwitchGL(CWeaponMagazinedWGrenade* wpn) { EngineFriendWrapper::PerformSwitchGL(wpn); }
} // namespace GunslingerMod
