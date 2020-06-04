#include "StdAfx.h"
#include "collimator.h"
#include "HudItemUtils.h"
#include "gunsl_config.h"
#include "WeaponAdditionalBuffer.h"

namespace GunslingerMod
{
bool IsCollimatorInstalled(CWeapon* wpn)
{
    if (!IsScopeAttached(wpn))
        return false;
    pcstr scope = GetCurrentScopeSection(wpn);
    scope = game_ini_read_string(scope, "scope_name");
    return game_ini_r_bool_def(scope, "collimator", false);
}

bool IsLensedScopeInstalled(CWeapon* wpn)
{
    bool result = false;
    EGunsAddonStatus scopestatus = GetScopeStatus(wpn);
    if (scopestatus == addonStatusAttachable)
    {
        if (IsScopeAttached(wpn))
        {
            pcstr scope = GetCurrentScopeSection(wpn);
            scope = game_ini_read_string(scope, "scope_name");
            result = game_ini_r_bool_def(scope, "need_lens_frame", false);
        }
    }
    else if (scopestatus == addonStatusPermanent)
    {
        result = game_ini_r_bool_def(GetSection(wpn), "need_lens_frame", false);
        result = FindBoolValueInUpgradesDef(wpn, "need_lens_frame", result, true);
    }
    return result;
}

bool IsHudModelForceUnhide(CWeapon* wpn)
{
    WpnBuf* buf = GetBuffer(wpn);
    return IsCollimatorInstalled(wpn) || (IsLensedScopeInstalled(wpn) && IsLensEnabled()) ||
        ((buf != nullptr) && buf->IsAlterZoomMode());
}

bool IsUIForceHiding(CWeapon* wpn)
{
    bool res = false;
    if (IsBino(wpn) && IsAimNow(wpn))
    {
        res = game_ini_r_bool_def(GetSection(wpn), "zoom_hide_ui", false);
    }
    else if ((GetScopeStatus(wpn) == addonStatusPermanent) && IsAimNow(wpn))
    {
        res = game_ini_r_bool_def(GetSection(wpn), "zoom_hide_ui", false);
    }
    else if ((GetScopeStatus(wpn) == addonStatusAttachable) && IsScopeAttached(wpn) && IsAimNow(wpn))
    {
        pcstr scope_name = game_ini_read_string(GetCurrentScopeSection(wpn), "scope_name");
        res = game_ini_r_bool_def(scope_name, "zoom_hide_ui", false);
    }
    return res;
}

bool IsUIForceUnhiding(CWeapon* wpn)
{
    bool result = IsHudModelForceUnhide(wpn);

    //Дальнейшие проверки имеют смысл только если худовая модель оружия не скрыта
    if (result)
    {
        WpnBuf* buf = GetBuffer(wpn);
        if ((buf != nullptr) && buf->IsAlterZoomMode())
        {
            //Стрельба с резервных прицельных приспособлений. Не скрываем ничего
            result = true;
        }
        else if (GetScopeStatus(wpn) == addonStatusPermanent)
        {
            result = !game_ini_r_bool_def(GetSection(wpn), "zoom_hide_ui", false);
        }
        else if ((GetScopeStatus(wpn) == addonStatusAttachable) && IsScopeAttached(wpn))
        {
            pcstr sn = game_ini_read_string(GetCurrentScopeSection(wpn), "scope_name");
            result = !game_ini_r_bool_def(sn, "zoom_hide_ui", false);
        }
    }

    return result;
}
} // namespace GunslingerMod
