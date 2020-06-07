#pragma once

class CWeaponMagazinedWGrenade;
class CWeapon;
class CHudItemObject;
class CInifile;
class IReader;
class IWriter;

namespace GunslingerMod
{
enum EGunsAddonStatus
{
    addonStatusDisabled = ALife::eAddonDisabled,
    addonStatusPermanent = ALife::eAddonPermanent,
    addonStatusAttachable = ALife::eAddonAttachable,
};

enum EGunsAddonType
{
    addonTypeScope = CSE_ALifeItemWeapon::eWeaponAddonScope,
    addonTypeGL = CSE_ALifeItemWeapon::eWeaponAddonGrenadeLauncher,
    addonTypeSilencer = CSE_ALifeItemWeapon::eWeaponAddonSilencer,
};

class EngineFriendWrapper
{
public:
    // for global stuff
    static u32 GetGameTickCount();
    static float random();
    static void LogText(pcstr text);
    static u32 GetCurrentALifeVersion();

    // for IReader
    static void ReadFromReader(IReader* r, void* buf, size_t bytes_count);
    static size_t ReaderLength(IReader* r);
    static size_t ReaderElapsed(IReader* r);

    // for IWriter
    static void WriteU32ToWriter(IWriter* w, u32 value);
    static void WriteStringZToWriter(IWriter* w, pcstr value);

    // for CLocatorAPI
    static u64 AuthGet();

    // for IniFile
    static const CInifile* GetGameIni();
    static bool ini_line_exist(const CInifile* ini, pcstr section, pcstr key);
    static bool ini_r_bool(const CInifile* ini, pcstr section, pcstr key);
    static float ini_r_float(const CInifile* ini, pcstr section, pcstr key);
    static pcstr ini_r_string(const CInifile* ini, pcstr section, pcstr key);

    // for CInventoryItem
    static pcstr GetItemSection(CInventoryItem* itm);
    static size_t GetInstalledUpgradesCount(CInventoryItem* itm);
    static pcstr GetInstalledUpgradeSection(CInventoryItem* itm, size_t index);

    // for CWeapon
    static void* GetBuffer(CWeapon* wpn);
    static void SetBuffer(CWeapon* wpn, void* buf);
    static xr_vector<shared_str>& GetScopesVector(CWeapon* wpn);
    static u8 GetCurScopeId(CWeapon* wpn);
    static bool IsAddonAttached(CWeapon* wpn, EGunsAddonType addon_type);
    static EGunsAddonStatus GetAddonStatus(CWeapon* wpn, EGunsAddonType addon_type);
    static bool IsZoomed(CWeapon* wpn);
    static void SetWeaponMisfireStatus(CWeapon* wpn, bool status);

    // for CWeaponMagazinedWGrenade
    static bool GetGrenadeMode(CWeaponMagazinedWGrenade* wpn);

    // for CSavedGameWrapper
    static s32 valid_saved_game_int(pcstr saved_game_name);
};
} // namespace GunslingerMod
