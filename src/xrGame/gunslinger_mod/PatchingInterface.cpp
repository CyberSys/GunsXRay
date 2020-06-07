#include "StdAfx.h"
#include "PatchingInterface.h"
#include "EngineFriends.h"
#include "WeaponDataSaveLoad.h"
#include "WeaponEvents.h"
#include "UiUtils.h"
#include "Misc.h"
#include "savedgames.h"
#include "gunsl_config.h"

namespace GunslingerMod
{
void PatchingInterface::CWeapon_class::load(CWeapon* wpn, IReader* packet)
{
    LoadAdditionalDataFromReader(wpn, packet);
}

void PatchingInterface::CWeapon_class::net_Spawn_middle(CWeapon* wpn) { CreateWpnBufIfNeeded(wpn); }

bool PatchingInterface::CLevel_class::Connect2Server_ForceAuthCalc() { return true; }

bool PatchingInterface::CSavedGameWrapper_class::valid_saved_game_override(IReader* reader)
{
    return (CSavedGameWrapper__valid_saved_game_override(reader) != GUNS_UNCOMPATIBLE_SAVE);
}

void PatchingInterface::CALifeStorageManager_class::save(IWriter* w) { ConstructGunsHeader(w); }

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

s32 PatchingInterface::ScriptExported::level::valid_saved_game_int(u16 unused, pcstr save_name)
{
    return EngineFriendWrapper::valid_saved_game_int(save_name);
}

void PatchingInterface::global_on_game_started_init() { gunsl_config_Init(); }
} // namespace GunslingerMod
