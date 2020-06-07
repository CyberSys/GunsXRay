#include "StdAfx.h"
#include "EngineFriends.h"
#include "savedgames.h"

#include "xrEngine/device.h"
#include "xrCore/xr_ini.h"
#include "xrCore/FS.h"
#include "xrCore/_random.h"
#include "xrCore/log.h"
#include "WeaponMagazinedWGrenade.h"
#include "saved_game_wrapper.h"

namespace GunslingerMod
{
// for global stuff
u32 EngineFriendWrapper::GetGameTickCount() { return Device.dwTimeGlobal; }
float EngineFriendWrapper::random() { return ::Random.randF(0.f, 1.f); }
void EngineFriendWrapper::LogText(pcstr text) { Log(text); }
u32 EngineFriendWrapper::GetCurrentALifeVersion() { return ALIFE_VERSION; }

// for IReader
void EngineFriendWrapper::ReadFromReader(IReader* r, void* buf, size_t bytes_count) { r->r(buf, bytes_count); }
size_t EngineFriendWrapper::ReaderLength(IReader* r) { return r->length(); }
size_t EngineFriendWrapper::ReaderElapsed(IReader* r) { return r->elapsed(); }

// for IWriter
void EngineFriendWrapper::WriteU32ToWriter(IWriter* w, u32 value) { w->w_u32(value); }
void EngineFriendWrapper::WriteStringZToWriter(IWriter* w, pcstr value) { w->w_stringZ(value); }

// for CLocatorAPI
u64 EngineFriendWrapper::AuthGet() { return FS.auth_get(); }

// for IniFile
const CInifile* EngineFriendWrapper::GetGameIni() { return pSettings; }

bool EngineFriendWrapper::ini_line_exist(const CInifile* ini, pcstr section, pcstr key)
{
    return ini->line_exist(section, key);
}

bool EngineFriendWrapper::ini_r_bool(const CInifile* ini, pcstr section, pcstr key)
{
    return ini->r_bool(section, key);
}

float EngineFriendWrapper::ini_r_float(const CInifile* ini, pcstr section, pcstr key)
{
    return ini->r_float(section, key);
}

pcstr EngineFriendWrapper::ini_r_string(const CInifile* ini, pcstr section, pcstr key)
{
    return ini->r_string(section, key);
}

// for CInventoryItem
pcstr EngineFriendWrapper::GetItemSection(CInventoryItem* itm) { return itm->m_section_id.c_str(); }
size_t EngineFriendWrapper::GetInstalledUpgradesCount(CInventoryItem* itm) { return itm->m_upgrades.size(); }
pcstr EngineFriendWrapper::GetInstalledUpgradeSection(CInventoryItem* itm, size_t index)
{
    return itm->m_upgrades[index].c_str();
}

// for CWeapon
void* EngineFriendWrapper::GetBuffer(CWeapon* wpn) { return wpn->m_gunslingermod_buffer; }
void EngineFriendWrapper::SetBuffer(CWeapon* wpn, void* buf) { wpn->m_gunslingermod_buffer = buf; }

xr_vector<shared_str>& EngineFriendWrapper::GetScopesVector(CWeapon* wpn) { return wpn->m_scopes; }

u8 EngineFriendWrapper::GetCurScopeId(CWeapon* wpn) { return wpn->m_cur_scope; }

bool EngineFriendWrapper::IsAddonAttached(CWeapon* wpn, EGunsAddonType addon_type)
{
    u8 state = wpn->GetAddonsState();
    switch (addon_type)
    {
    case addonTypeScope: return (state & CSE_ALifeItemWeapon::eWeaponAddonScope != 0);
    case addonTypeGL: return (state & CSE_ALifeItemWeapon::eWeaponAddonGrenadeLauncher != 0);
    case addonTypeSilencer: return (state & CSE_ALifeItemWeapon::eWeaponAddonSilencer != 0);
    };
    return false;
}

EGunsAddonStatus EngineFriendWrapper::GetAddonStatus(CWeapon* wpn, EGunsAddonType addon_type)
{
    switch (addon_type)
    {
    case addonTypeScope: return static_cast<EGunsAddonStatus>(wpn->get_ScopeStatus());
    case addonTypeGL: return static_cast<EGunsAddonStatus>(wpn->get_GrenadeLauncherStatus());
    case addonTypeSilencer: return static_cast<EGunsAddonStatus>(wpn->get_SilencerStatus());
    };
    return addonStatusDisabled;
}

bool EngineFriendWrapper::IsZoomed(CWeapon* wpn) { return wpn->IsZoomed(); }

void EngineFriendWrapper::SetWeaponMisfireStatus(CWeapon* wpn, bool status) { wpn->bMisfire = status; }

// for CWeaponMagazinedWGrenade
bool EngineFriendWrapper::GetGrenadeMode(CWeaponMagazinedWGrenade* wpn) { return wpn->m_bGrenadeMode; }

// for CSavedGameWrapper
s32 EngineFriendWrapper::valid_saved_game_int(pcstr saved_game_name)
{
    string_path file_name;
    if (!FS.exist(CSavedGameWrapper::saved_game_full_name(saved_game_name, file_name, SAVE_EXTENSION)))
        if (!FS.exist(CSavedGameWrapper::saved_game_full_name(saved_game_name, file_name, SAVE_EXTENSION_LEGACY)))
            return false;

    IReader* stream = FS.r_open(file_name);
    s32 result = static_cast<s32>(CSavedGameWrapper__valid_saved_game_override(stream));
    FS.r_close(stream);
    return (result);
}
} // namespace GunslingerMod
