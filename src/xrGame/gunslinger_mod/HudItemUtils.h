#pragma once
#include "EngineFriends.h"
#include "BaseGameData.h"

class CHudItemObject;
class CWeapon;
class CWeaponMagazined;
class attachable_hud_item;

namespace GunslingerMod
{
enum EAnmTimeOffsetType
{
    ANM_TIME_START,
    ANM_TIME_CUR,
    ANM_TIME_END,
};

enum EWeaponHudState
{
    EWeaponStates__eFire = CWeapon::eFire,
    EWeaponStates__eFire2 = CWeapon::eFire2,
    EWeaponStates__eReload = CWeapon::eReload,
    EWeaponStates__eMisfire = CWeapon::eMisfire,
    EWeaponStates__eMagEmpty = CWeapon::eMagEmpty,
    EWeaponStates__eSwitch = CWeapon::eSwitch,
    EHudStates__eIdle = CHUDState::eIdle,
    EHudStates__eShowing = CHUDState::eShowing,
    EHudStates__eHiding = CHUDState::eHiding,
    EHudStates__eHidden = CHUDState::eHidden,
    EHudStates__eBore = CHUDState::eBore,
    EHudStates__eLastBaseState = CHUDState::eLastBaseState,
    EMissileStates__eThrowStart = CMissile::eThrowStart,
    EMissileStates__eReady = CMissile::eReady,
    EMissileStates__eThrow = CMissile::eThrow,
    EMissileStates__eThrowEnd = CMissile::eThrowEnd,
};

void ChangeParticles(CWeapon* wpn, pcstr name, EGunsWeaponParticles particle_type);

void PlayAnimIdle(CHudItemObject* wpn);

bool IsScopeAttached(CWeapon* wpn);
bool IsSilencerAttached(CWeapon* wpn);
bool IsGLAttached(CWeapon* wpn);
bool IsGLEnabled(CWeapon* wpn);

bool IsWeaponJammed(CWeapon* wpn);

u32 GetInstalledUpgradesCount(CInventoryItem* itm);
pcstr GetInstalledUpgradeSection(CInventoryItem* itm, u32 index);
bool FindBoolValueInUpgradesDef(CInventoryItem* itm, pcstr key, bool def, bool scan_after_nodefault);

float ModifyFloatUpgradedValue(CInventoryItem* itm, pcstr key, float def);

pcstr GetSection(CHudItemObject* wpn);
u16 GetID(CHudItemObject* wpn);
pcstr GetHUDSection(CHudItemObject* wpn);
EGunsAddonStatus GetScopeStatus(CHudItemObject* wpn);
EGunsAddonStatus GetSilencerStatus(CHudItemObject* wpn);
EGunsAddonStatus GetGLStatus(CHudItemObject* wpn);
pcstr GetCurrentScopeSection(CWeapon* wpn);
u32 GetScopesCount(CWeapon* wpn);
s32 GetCurrentScopeIndex(CWeapon* wpn);
pcstr GetScopeSection(CWeapon* wpn, u32 index);
void SetWpnVisual(CHudItemObject* wpn, pcstr name);
void SetObjectVisual(CGameObject* o, pcstr name);
void SetHUDSection(CHudItemObject* wpn, pcstr new_hud_section);
u32 GetAmmoInMagCount(CWeapon* wpn);
u32 GetAmmoInGLCount(CWeapon* wpn);

IGameObject* GetOwner(CHudItemObject* wpn);

bool IsAimNow(CHudItemObject* wpn);
float GetAimFactor(CWeapon* wpn);

bool IsTriStateReload(CWeapon* wpn);

EWeaponHudState GetCurrentState(CHudItemObject* wpn);

pcstr GetSilencerSection(CWeapon* wpn);
void DetachAddon(CWeapon* wpn, EGunsAddonType addon_type);
pcstr GetGLSection(CWeaponMagazinedWGrenade* wpn);

float GetCurrentCondition(CHudItemObject* wpn);
const Fvector3* GetPosition(CHudItemObject* wpn);
const Fvector3* GetOrientation(CHudItemObject* wpn);
pcstr GetActualCurrentAnim(CHudItemObject* wpn);

void SetWeaponMisfireStatus(CWeapon* wpn, bool status);

bool WpnCanShoot(CHudItemObject* wpn);
bool IsBino(CHudItemObject* wpn);

TimeTicks GetAnimTimeState(CHudItemObject* wpn, EAnmTimeOffsetType what);

bool virtual_Action(CHudItemObject* wpn, u32 cmd, u32 flags);

void virtual_CHudItem_SwitchState(CHudItem* itm, EWeaponHudState state);

void virtual_CWeaponMagazined__UnloadMagazine(CWeaponMagazined* wpn, bool spawn_ammo);

attachable_hud_item* CHudItem__HudItemData(CHudItem* hi);

bool IsGrenadeMode(CWeapon* wpn);
void PerformSwitchGL(CWeaponMagazinedWGrenade* wpn);
} // namespace GunslingerMod
