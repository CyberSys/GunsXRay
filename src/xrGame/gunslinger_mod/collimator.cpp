#include "StdAfx.h"
#include "collimator.h"
#include "HudItemUtils.h"
#include "gunsl_config.h"

namespace GunslingerMod
{
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
    return false; // TODO: port
}
} // namespace GunslingerMod
