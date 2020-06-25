#include "StdAfx.h"
#include "EngineFriends.h"
#include "savedgames.h"

#include "xrEngine/device.h"
#include "xrEngine/xr_object.h"
#include "xrCore/xr_ini.h"
#include "xrCore/FS.h"
#include "xrCore/_random.h"
#include "xrCore/log.h"
#include "WeaponMagazinedWGrenade.h"
#include "saved_game_wrapper.h"
#include "Level.h"
#include "RocketLauncher.h"
#include "WeaponAmmo.h"
#include "CustomRocket.h"
#include "alife_simulator.h"
#include "CustomDetector.h"
#include "Actor.h"
#include "GamePersistent.h"
#include "Include/xrRender/Kinematics.h"

CALifeSimulator* alife();
void CALifeSimulator__release(CALifeSimulator*, CSE_Abstract*, bool);

CSE_Abstract* CALifeSimulator__spawn_item2(CALifeSimulator* self, LPCSTR section, const Fvector& position,
    u32 level_vertex_id, GameGraph::_GRAPH_ID game_vertex_id, ALife::_OBJECT_ID id_parent);

static pcstr safe_pcstr(const shared_str& str)
{
    pcstr s = str.c_str();
    return (s != nullptr) ? s : "";
}

namespace GunslingerMod
{
// for global stuff
u32 EngineFriendWrapper::GetGameTickCount() { return Device.dwTimeGlobal; }
float EngineFriendWrapper::random() { return ::Random.randF(0.f, 1.f); }
void EngineFriendWrapper::LogText(pcstr text) { Log(text); }
u32 EngineFriendWrapper::GetCurrentALifeVersion() { return ALIFE_VERSION; }

CALifeSimulator* EngineFriendWrapper::alife() { return ::alife(); }
void EngineFriendWrapper::alife_release(CSE_Abstract* srv_obj)
{
    if (srv_obj == nullptr)
        return;
    CALifeSimulator* alife_object = alife();
    if (alife_object == nullptr)
        return;
    CALifeSimulator__release(alife_object, srv_obj, true);
}

void EngineFriendWrapper::alife_create(pcstr section, const Fvector3* position, u32 lvid, u16 gvid, u16 parent_id)
{
    CALifeSimulator* alife_object = alife();
    if (alife_object == nullptr)
        return;

    CALifeSimulator__spawn_item2(alife_object, section, *position, lvid, gvid, parent_id);
}

CGamePersistent* EngineFriendWrapper::GetGamePersistent() { return &GamePersistent(); }

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
    pcstr s = ini->r_string(section, key);
    return (s != nullptr) ? s : "";
}

s32 EngineFriendWrapper::ini_r_int(const CInifile* ini, pcstr section, pcstr key) { return ini->r_s32(section, key); }

// for CInventoryItem
pcstr EngineFriendWrapper::GetItemSection(CInventoryItem* itm) { return safe_pcstr(itm->m_section_id); }
size_t EngineFriendWrapper::GetInstalledUpgradesCount(CInventoryItem* itm) { return itm->m_upgrades.size(); }
pcstr EngineFriendWrapper::GetInstalledUpgradeSection(CInventoryItem* itm, size_t index)
{
    return safe_pcstr(itm->m_upgrades[index]);
}
float EngineFriendWrapper::GetCondition(CInventoryItem* itm) { return itm->GetCondition(); }

// for CInventoryOwner
u16 EngineFriendWrapper::GetActiveSlot(CInventoryOwner* io) { return io->inventory().GetActiveSlot(); }
u16 EngineFriendWrapper::GetTargetSlot(CInventoryOwner* io) { return io->inventory().GetNextActiveSlot(); }
u16 EngineFriendWrapper::GetPreviousSlot(CInventoryOwner* io) { return io->inventory().GetPrevActiveSlot(); }
CInventoryItem* EngineFriendWrapper::ItemFromSlot(CInventoryOwner* io, u16 slot)
{
    return io->inventory().ItemFromSlot(slot);
}
void EngineFriendWrapper::SetTargetSlot(CInventoryOwner* io, u16 slot) { io->inventory().m_iNextActiveSlot = slot; }

// for IGameObject
u16 EngineFriendWrapper::GetID(IGameObject* o) { return o->ID(); }
IGameObject* EngineFriendWrapper::GetParent(IGameObject* o) { return o->H_Parent(); }
const Fmatrix* EngineFriendWrapper::GetXForm(IGameObject* o) { return &o->XFORM(); }
IRenderVisual* EngineFriendWrapper::GetVisual(IGameObject* o) { return o->Visual(); }
void EngineFriendWrapper::SetNameVisual(IGameObject* o, pcstr vis_name) { o->cNameVisual_set(vis_name); }

// for CGameObject
CScriptGameObject* EngineFriendWrapper::GetScriptGameObject(CGameObject* o) { return o->lua_game_object(); }

// for CHudItem
pcstr EngineFriendWrapper::GetHUDSection(CHudItem* itm) { return safe_pcstr(itm->HudSection()); }
void EngineFriendWrapper::SetHUDSection(CHudItem* itm, pcstr sect) { itm->hud_sect = sect; }
pcstr EngineFriendWrapper::GetActualCurrentAnim(CHudItem* itm) { return safe_pcstr(itm->m_current_motion); }
u32 EngineFriendWrapper::GetCurrentState(CHudItem* itm) { return itm->GetState(); }
void EngineFriendWrapper::virtual_CHudItem_SwitchState(CHudItem* itm, u32 state) { itm->SwitchState(state); }
u32 EngineFriendWrapper::GetMotionStartTime(CHudItem* itm) { return itm->m_dwMotionStartTm; }
u32 EngineFriendWrapper::GetMotionCurrTime(CHudItem* itm) { return itm->m_dwMotionCurrTm; }
u32 EngineFriendWrapper::GetMotionEndTime(CHudItem* itm) { return itm->m_dwMotionEndTm; }
struct attachable_hud_item* EngineFriendWrapper::GetHudItemData(CHudItem* itm) { return itm->HudItemData(); }
void EngineFriendWrapper::PlayAnimIdle(CHudItem* itm) { itm->PlayAnimIdle(); }

// for CHudItemObject
bool EngineFriendWrapper::virtual_Action(CHudItemObject* wpn, u16 cmd, u32 flags) { return wpn->Action(cmd, flags); }

// for CWeapon
void* EngineFriendWrapper::GetBuffer(CWeapon* wpn) { return wpn->m_gunslingermod_buffer; }
void EngineFriendWrapper::SetBuffer(CWeapon* wpn, void* buf) { wpn->m_gunslingermod_buffer = buf; }

xr_vector<shared_str>& EngineFriendWrapper::GetScopesVector(CWeapon* wpn) { return wpn->m_scopes; }

u8 EngineFriendWrapper::GetCurScopeId(CWeapon* wpn) { return wpn->m_cur_scope; }
void EngineFriendWrapper::SetCurScopeId(CWeapon* wpn, u8 id) { wpn->m_cur_scope = id; }

bool EngineFriendWrapper::IsAddonAttached(CWeapon* wpn, EGunsAddonType addon_type)
{
    u8 state = wpn->GetAddonsState();
    return (state & addon_type != 0);
}

void EngineFriendWrapper::SetAddonAttachedStatus(CWeapon* wpn, EGunsAddonType addon_type, bool attached)
{
    u8 state = wpn->GetAddonsState();
    if (attached)
    {
        state |= addon_type;
    }
    else
    {
        state &= ~addon_type;
    }
    wpn->SetAddonsState(state);
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

pcstr EngineFriendWrapper::GetSilencerName(CWeapon* wpn) { return safe_pcstr(wpn->GetSilencerName()); }

bool EngineFriendWrapper::IsZoomed(CWeapon* wpn) { return wpn->IsZoomed(); }

void EngineFriendWrapper::SetWeaponMisfireStatus(CWeapon* wpn, bool status) { wpn->bMisfire = status; }

size_t EngineFriendWrapper::GetAmmoInVectorCount(CWeapon* wpn) { return wpn->m_magazine.size(); }

u8 EngineFriendWrapper::GetAmmoTypeIndex(CWeapon* wpn, bool second)
{
    if (second)
    {
        return static_cast<CWeaponMagazinedWGrenade*>(wpn)->m_ammoType2;
    }
    else
    {
        return wpn->m_ammoType;
    }
}

float EngineFriendWrapper::GetZoomFactor(CWeapon* wpn) { return wpn->m_zoom_params.m_fCurrentZoomFactor; }

void EngineFriendWrapper::UpdateAddonsVisibility(CWeapon* wpn) { wpn->UpdateAddonsVisibility(); }
void EngineFriendWrapper::InitAddons(CWeapon* wpn) { wpn->InitAddons(); }
void EngineFriendWrapper::SetWeaponParticle(CWeapon* wpn, pcstr particle_name, EGunsWeaponParticles particle_type)
{
    switch (particle_type)
    {
    case CWEAPON_SHELL_PARTICLES: wpn->m_sShellParticles = particle_name; break;
    case CWEAPON_FLAME_PARTICLES_CURRENT: wpn->m_sFlameParticlesCurrent = particle_name; break;
    case CWEAPON_FLAME_PARTICLES: wpn->m_sFlameParticles = particle_name; break;
    case CWEAPON_SMOKE_PARTICLES_CURRENT: wpn->m_sSmokeParticlesCurrent = particle_name; break;
    case CWEAPON_SMOKE_PARTICLES: wpn->m_sSmokeParticles = particle_name; break;
    }
}

// for CWeaponMagazined
void EngineFriendWrapper::virtual_CWeaponMagazined__UnloadMagazine(CWeaponMagazined* wpn, bool spawn_ammo)
{
    wpn->UnloadMagazine(spawn_ammo);
}

// for CWeaponMagazinedWGrenade
bool EngineFriendWrapper::GetGrenadeMode(CWeaponMagazinedWGrenade* wpn) { return wpn->m_bGrenadeMode; }

void EngineFriendWrapper::LaunchGrenade(CWeaponMagazinedWGrenade* wpn) { wpn->LaunchGrenade(); }

size_t EngineFriendWrapper::GetAmmoInSecondVectorCount(CWeaponMagazinedWGrenade* wpn)
{
    return wpn->m_magazine2.size();
}

void EngineFriendWrapper::PerformSwitchGL(CWeaponMagazinedWGrenade* wpn) { wpn->PerformSwitchGL(); }

pcstr EngineFriendWrapper::GetGLSection(CWeaponMagazinedWGrenade* wpn)
{
    return safe_pcstr(wpn->GetGrenadeLauncherName());
}

// for CCustomDetector
bool EngineFriendWrapper::IsDetectorActive(CCustomDetector* det) { return det->IsWorking(); }
void EngineFriendWrapper::SetDetectorForceUnhide(CCustomDetector* det, bool status) { det->m_bNeedActivation = status; }

// for CRocketLauncher
u32 EngineFriendWrapper::GetRocketsCount(CRocketLauncher* rl) { return rl->getRocketCount(); }
CCustomRocket* EngineFriendWrapper::GetRocket(CRocketLauncher* rl, size_t index) { return rl->m_rockets[index]; }
void EngineFriendWrapper::DetachRocket(CRocketLauncher* rl, u16 id, bool bLaunch) { rl->DetachRocket(id, bLaunch); }
void EngineFriendWrapper::DropRockets(CRocketLauncher* rl)
{
    while (GetRocketsCount(rl) > 0)
        rl->dropCurrentRocket();
}

// for CCustomRocket
void EngineFriendWrapper::virtual_CCustomRocket__Contact(CCustomRocket* r, Fvector3* pos, Fvector3* normal)
{
    r->Contact(*pos, *normal);
}

// for CLevel
CSE_Abstract* EngineFriendWrapper::ID_to_entity(u16 id) { return Level().Server->ID_to_entity(id); }
IGameObject* EngineFriendWrapper::GetCurrentEntity() { return Level().CurrentEntity(); }

// for CCartridge
u8 EngineFriendWrapper::GetCartridgeTypeIdx(CCartridge* c) { return c->m_LocalAmmoType; }

// for CActor
u32 EngineFriendWrapper::GetOldMState(CActor* act) { return act->mstate_old; }
u32 EngineFriendWrapper::GetRealMState(CActor* act) { return act->mstate_real; }
u32 EngineFriendWrapper::GetWishfulMState(CActor* act) { return act->mstate_wishful; }
void EngineFriendWrapper::SetOldMState(CActor* act, u32 value) { act->mstate_old = value; }
void EngineFriendWrapper::SetRealMState(CActor* act, u32 value) { act->mstate_real = value; }
void EngineFriendWrapper::SetWishfulMState(CActor* act, u32 value) { act->mstate_wishful = value; }
CCustomDetector* EngineFriendWrapper::GetActiveDetector(CActor* act)
{
    CScriptGameObject* o = EngineFriendWrapper::GetScriptGameObject(act);
    if (o == nullptr)
        return nullptr;

    CScriptGameObject* det = o->active_detector();
    if (det == nullptr)
        return nullptr;

    return static_cast<CCustomDetector*>(&det->object());
}

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

// for CSE_ALifeItemWeapon
void EngineFriendWrapper::UpdateElapsedGrenadesCount(CSE_ALifeItemWeapon* wpn, u8 count, u8 type)
{
    wpn->a_elapsed_grenades.grenades_count = count;
    wpn->a_elapsed_grenades.grenades_type = type;
}

// for CGamePersistent
Fvector* EngineFriendWrapper::DestDof(CGamePersistent* gp) { return &(gp->m_dof[0]); }
Fvector* EngineFriendWrapper::CurDof(CGamePersistent* gp) { return &gp->m_dof[1]; }
Fvector* EngineFriendWrapper::PrevDof(CGamePersistent* gp) { return &gp->m_dof[2]; }
Fvector* EngineFriendWrapper::OrigDof(CGamePersistent* gp) { return &gp->m_dof[3]; }

// for IKinematics
u16 EngineFriendWrapper::GetLLBoneId(IKinematics* ik, pcstr name) { return ik->LL_BoneID(name); }

void EngineFriendWrapper::SetBoneVisible(IKinematics* ik, u16 id, bool status, bool modify_children)
{
    ik->LL_SetBoneVisible(id, status, modify_children);
}
} // namespace GunslingerMod
