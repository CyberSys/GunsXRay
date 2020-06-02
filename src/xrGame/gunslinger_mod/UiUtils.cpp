#include "StdAfx.h"
#include "UiUtils.h"
#include "ActorUtils.h"
#include "collimator.h"
#include "HudItemUtils.h"

namespace GunslingerMod
{
bool IsUIShown()
{
    bool res = false;
    auto ui = CurrentGameUI();
    if (ui != nullptr)
    {
        res = ui->GameIndicatorsShown();
    }
    return res;
}

bool IndicatorsShown()
{
    bool res = IsUIShown();
    if (res)
    {
        CWeapon* wpn = dynamic_cast<CWeapon*>(GetActorActiveItem());
        if (wpn != nullptr)
        {
            res = true;
        }
        else if (IsUIForceHiding(wpn))
        {
            res = false;
        }
        else if (IsUIForceUnhiding(wpn))
        {
            res = true;
        }
        else if (IsGrenadeMode(wpn))
        {
            res = true;
        }
        else if (IsAimNow(wpn) &&
            ((GetScopeStatus(wpn) == addonStatusPermanent) ||
                (GetScopeStatus(wpn) == addonStatusAttachable) && IsScopeAttached(wpn)))
        {
            res = false;
        }
        else
        {
            res = true;
        }
    }
    return res;
}
} // namespace GunslingerMod
