#include "StdAfx.h"
#include "PatchingInterface.h"
#include "WeaponDataSaveLoad.h"
#include "WeaponEvents.h"
#include "UiUtils.h"
#include "Misc.h"

namespace GunslingerMod
{
void PatchingInterface::CWeapon_class::load(CWeapon* wpn, IReader* packet)
{
    LoadAdditionalDataFromReader(wpn, packet);
}

void PatchingInterface::CWeapon_class::net_Spawn_middle(CWeapon* wpn) { CreateWpnBufIfNeeded(wpn); }

bool PatchingInterface::ScriptExported::level::is_ui_shown() { return IsUIShown(); }
bool PatchingInterface::ScriptExported::level::indicators_shown() { return IndicatorsShown(); }
bool PatchingInterface::ScriptExported::level::electronics_break() { return ElectronicsProblemsInc(); }
bool PatchingInterface::ScriptExported::level::electronics_restore() { return ElectronicsProblemsDec(); }
bool PatchingInterface::ScriptExported::level::electronics_reset()
{
    ResetElectronicsProblems();
    return true;
}
bool PatchingInterface::ScriptExported::level::electronics_apply()
{
    ElectronicsProblemsImmediateApply();
    return true;
}
} // namespace GunslingerMod
