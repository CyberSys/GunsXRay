#pragma once

#include "xrCore/_vector3d.h"

class CWeapon;
class CWeaponMagazined;
class CWeaponMagazinedWGrenade;
class CRocketLauncher;
class CHudItemObject;
class CInifile;
class IReader;
class IWriter;
class CSE_Abstract;
class CCartridge;
class CCustomRocket;
class CALifeSimulator;
class CInventoryOwner;
class CCustomDetector;
class CActor;
class CGamePersistent;
class CScriptGameObject;
class CGameObject;
struct attachable_hud_item;
class IKinematics;
class IRenderVisual;

namespace GunslingerMod
{
enum EGunsAddonStatus
{
    addonStatusDisabled = ALife::eAddonDisabled,
    addonStatusPermanent = ALife::eAddonPermanent,
    addonStatusAttachable = ALife::eAddonAttachable,
};

enum EGunsKeyAction
{
    kActPress = 1,
    kActRelease = 2,
};

enum EGunsAddonType
{
    addonTypeScope = CSE_ALifeItemWeapon::eWeaponAddonScope,
    addonTypeGL = CSE_ALifeItemWeapon::eWeaponAddonGrenadeLauncher,
    addonTypeSilencer = CSE_ALifeItemWeapon::eWeaponAddonSilencer,
};

enum EGunsWeaponParticles
{
    CWEAPON_SHELL_PARTICLES,
    CWEAPON_FLAME_PARTICLES_CURRENT,
    CWEAPON_FLAME_PARTICLES,
    CWEAPON_SMOKE_PARTICLES_CURRENT,
    CWEAPON_SMOKE_PARTICLES,
};

class EngineFriendWrapper
{
public:
    // for global stuff
    static u32 GetGameTickCount();
    static float random();
    static void LogText(pcstr text);
    static u32 GetCurrentALifeVersion();
    static CALifeSimulator* alife();
    static void alife_release(CSE_Abstract* srv_obj);
    static void alife_create(pcstr section, const Fvector3* position, u32 lvid, u16 gvid, u16 parent_id);
    static CGamePersistent* GetGamePersistent();

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
    static s32 ini_r_int(const CInifile* ini, pcstr section, pcstr key);

    // for CInventoryItem
    static pcstr GetItemSection(CInventoryItem* itm);
    static size_t GetInstalledUpgradesCount(CInventoryItem* itm);
    static pcstr GetInstalledUpgradeSection(CInventoryItem* itm, size_t index);
    static float GetCondition(CInventoryItem* itm);

    // for CInventoryOwner
    static u16 GetActiveSlot(CInventoryOwner* io);
    static u16 GetTargetSlot(CInventoryOwner* io);
    static u16 GetPreviousSlot(CInventoryOwner* io);
    static CInventoryItem* ItemFromSlot(CInventoryOwner* io, u16 slot);
    static void SetTargetSlot(CInventoryOwner* io, u16 slot);

    // for IGameObject
    static u16 GetID(IGameObject* o);
    static IGameObject* GetParent(IGameObject* o);
    static const Fmatrix* GetXForm(IGameObject* o);
    static IRenderVisual* GetVisual(IGameObject* o);

    // for CGameObject
    static CScriptGameObject* GetScriptGameObject(CGameObject* o);
    static void SetNameVisual(IGameObject* o, pcstr vis_name);

    // for CHudItem
    static pcstr GetHUDSection(CHudItem* itm);
    static void SetHUDSection(CHudItem* itm, pcstr sect);
    static pcstr GetActualCurrentAnim(CHudItem* itm);
    static u32 GetCurrentState(CHudItem* itm);
    static void virtual_CHudItem_SwitchState(CHudItem* itm, u32 state);
    static u32 GetMotionStartTime(CHudItem* itm);
    static u32 GetMotionCurrTime(CHudItem* itm);
    static u32 GetMotionEndTime(CHudItem* itm);
    static struct attachable_hud_item* GetHudItemData(CHudItem* itm);
    static void PlayAnimIdle(CHudItem* itm);

    // for CHudItemObject
    static bool virtual_Action(CHudItemObject* wpn, u16 cmd, u32 flags);

    // for CWeapon
    static void* GetBuffer(CWeapon* wpn);
    static void SetBuffer(CWeapon* wpn, void* buf);
    static xr_vector<shared_str>& GetScopesVector(CWeapon* wpn);
    static u8 GetCurScopeId(CWeapon* wpn);
    static void SetCurScopeId(CWeapon* wpn, u8 id);
    static bool IsAddonAttached(CWeapon* wpn, EGunsAddonType addon_type);
    static void SetAddonAttachedStatus(CWeapon* wpn, EGunsAddonType addon_type, bool attached);
    static EGunsAddonStatus GetAddonStatus(CWeapon* wpn, EGunsAddonType addon_type);
    static pcstr GetSilencerName(CWeapon* wpn);
    static bool IsZoomed(CWeapon* wpn);
    static void SetWeaponMisfireStatus(CWeapon* wpn, bool status);
    static size_t GetAmmoInVectorCount(CWeapon* wpn);
    static u8 GetAmmoTypeIndex(CWeapon* wpn, bool second);
    static float GetZoomFactor(CWeapon* wpn);
    static void UpdateAddonsVisibility(CWeapon* wpn);
    static void InitAddons(CWeapon* wpn);
    static void SetWeaponParticle(CWeapon* wpn, pcstr particle_name, EGunsWeaponParticles particle_type);

    // for CWeaponMagazined
    static void virtual_CWeaponMagazined__UnloadMagazine(CWeaponMagazined* wpn, bool spawn_ammo);

    // for CWeaponMagazinedWGrenade
    static bool GetGrenadeMode(CWeaponMagazinedWGrenade* wpn);
    static void LaunchGrenade(CWeaponMagazinedWGrenade* wpn);
    static size_t GetAmmoInSecondVectorCount(CWeaponMagazinedWGrenade* wpn);
    static void PerformSwitchGL(CWeaponMagazinedWGrenade* wpn);
    static pcstr GetGLSection(CWeaponMagazinedWGrenade* wpn);

    // for CCustomDetector
    static bool IsDetectorActive(CCustomDetector* det);
    static void SetDetectorForceUnhide(CCustomDetector* det, bool status);

    // for CRocketLauncher
    static u32 GetRocketsCount(CRocketLauncher* rl);
    static CCustomRocket* GetRocket(CRocketLauncher* rl, size_t index);
    static void DetachRocket(CRocketLauncher* rl, u16 id, bool bLaunch);
    static void DropRockets(CRocketLauncher* rl);

    // for CCustomRocket
    static void virtual_CCustomRocket__Contact(CCustomRocket* r, Fvector3* pos, Fvector3* normal);

    // for CLevel
    static CSE_Abstract* ID_to_entity(u16 id);
    static IGameObject* GetCurrentEntity();

    // for CCartridge
    static u8 GetCartridgeTypeIdx(CCartridge* c);

    // for CActor
    static u32 GetOldMState(CActor* act);
    static u32 GetRealMState(CActor* act);
    static u32 GetWishfulMState(CActor* act);
    static void SetOldMState(CActor* act, u32 value);
    static void SetRealMState(CActor* act, u32 value);
    static void SetWishfulMState(CActor* act, u32 value);
    static CCustomDetector* GetActiveDetector(CActor* act);

    // for CSavedGameWrapper
    static s32 valid_saved_game_int(pcstr saved_game_name);

    // for CSE_ALifeItemWeapon
    static void UpdateElapsedGrenadesCount(CSE_ALifeItemWeapon* wpn, u8 count, u8 type);

    // for CGamePersistent
    static Fvector* DestDof(CGamePersistent* gp);
    static Fvector* CurDof(CGamePersistent* gp);
    static Fvector* PrevDof(CGamePersistent* gp);
    static Fvector* OrigDof(CGamePersistent* gp);

    // for IKinematics
    static u16 GetLLBoneId(IKinematics* ik, pcstr name);
    static void SetBoneVisible(IKinematics* ik, u16 id, bool status, bool modify_children);
};
} // namespace GunslingerMod
