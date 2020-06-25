#include "StdAfx.h"
#include "WeaponUpdate.h"

#include "Misc.h"
#include "HudItemUtils.h"
#include "xr_RocketLauncher.h"
#include "xr_Cartridge.h"
#include "ActorUtils.h"
#include "gunsl_config.h"
#include "BaseGameData.h"
#include "ControllerMonster.h"
#include "WeaponAdditionalBuffer.h"
#include "ActorDOF.h"
#include "WeaponEvents.h"
#include "xr_BoneUtils.h"

#include "Weapon.h"
#include "WeaponMagazinedWGrenade.h"
#include "CustomRocket.h"

#include "xrCore/_vector3d.h"

namespace GunslingerMod
{
void HideOneUpgradeLevel(CWeapon* wpn, pcstr up_gr_section)
{
    string all_subelements = game_ini_read_string(up_gr_section, "elements");

    string element;
    while (GetNextSubStr(all_subelements, element, ','))
    {
        //Обработаем ветки, которые открывает данный апгрейд
        if (game_ini_line_exist(element.c_str(), "effects"))
        {
            string up_group = game_ini_read_string(element.c_str(), "effects");
            string tmp;
            while (GetNextSubStr(up_group, tmp, ','))
            {
                HideOneUpgradeLevel(wpn, tmp.c_str());
            }
        }

        //Теперь посмотрим, какие кости надо отображать, когда данный апгрейд установлен
        pcstr up_sect = game_ini_read_string(element.c_str(), "section");
        if (game_ini_line_exist(up_sect, "show_bones"))
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(up_sect, "show_bones"), false);
    }
}

void ProcessUpgrade(CWeapon* wpn)
{
    pcstr section = GetSection(wpn);
    WpnBuf* buf = GetBuffer(wpn);

    EGunsAddonStatus gl_status = GetGLStatus(wpn);
    bool gl_enabled = (gl_status != addonStatusDisabled) && IsGrenadeMode(wpn);

    //Скроем все кости, которые надо скрыть, исходя из данных секции оружия
    if (game_ini_line_exist(section, "def_hide_bones"))
        SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "def_hide_bones"), false);

    if (game_ini_line_exist(section, "def_show_bones"))
        SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "def_show_bones"), true);

    if (gl_enabled && game_ini_line_exist(section, "def_hide_bones_grenade") &&
        (leftstr(GetActualCurrentAnim(wpn), length("anm_switch")) != "anm_switch"))
    {
        SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "def_hide_bones_grenade"), false);
    }

    //Прочитаем секции всех доступных апов из конфига
    if (game_ini_line_exist(section, "upgrades"))
    {
        string all_upgrades = game_ini_read_string(section, "upgrades");

        //Переберем их все
        string up_gr_sect;
        while (GetNextSubStr(all_upgrades, up_gr_sect, ','))
        {
            HideOneUpgradeLevel(wpn, up_gr_sect.c_str());
        }

        //Посмотрим, какие апгрейды уже установлены, и отобразим их
        for (size_t i = 0; i < GetInstalledUpgradesCount(wpn); ++i)
        {
            section = GetInstalledUpgradeSection(wpn, i);
            section = game_ini_read_string(section, "section");
            if (game_ini_line_exist(section, "hide_bones"))
                SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "hide_bones"), false);

            if (game_ini_line_exist(section, "hud"))
            {
                if (game_ini_r_bool_def(section, "hud_when_silencer_is_attached", false) && IsSilencerAttached(wpn))
                    SetHUDSection(wpn, game_ini_read_string(section, "hud_silencer"));
                else if (game_ini_r_bool_def(section, "hud_when_gl_is_attached", false) && IsGLAttached(wpn))
                    SetHUDSection(wpn, game_ini_read_string(section, "hud_gl"));
                else if (game_ini_r_bool_def(section, "hud_when_scope_is_attached", false) && IsScopeAttached(wpn))
                    SetHUDSection(wpn, game_ini_read_string(section, "hud_scope"));
                else
                    SetHUDSection(wpn, game_ini_read_string(section, "hud"));
            }
            if (game_ini_line_exist(section, "visual"))
            {
                SetWpnVisual(wpn, game_ini_read_string(section, "visual"));
            }

            if ((buf != nullptr) && !buf->IsLaserInstalled() && game_ini_r_bool_def(section, "laser_installed", false))
            {
                buf->InstallLaser(section);
            }

            if ((buf != nullptr) && !buf->IsTorchInstalled() && game_ini_r_bool_def(section, "torch_installed", false))
            {
                buf->InstallTorch(section);
            }

            if ((buf != nullptr) && !buf->NeedPermanentLensRendering() &&
                game_ini_r_bool_def(section, "permanent_lens_render", false))
            {
                buf->SetPermanentLensRenderingStatus(true);
            }

            if (buf != nullptr)
            {
                lens_zoom_params lens_params = buf->GetLensParams();
                float t_dt = game_ini_r_single_def(section, "lens_factor_levels_count", 0);

                if (t_dt != 0)
                {
                    lens_params.delta = 1.0 / t_dt;
                }
                lens_params.factor_min = game_ini_r_single_def(section, "min_lens_factor", lens_params.factor_min);
                lens_params.factor_max = game_ini_r_single_def(section, "max_lens_factor", lens_params.factor_max);
                lens_params.speed = game_ini_r_single_def(section, "lens_speed", lens_params.speed);
                lens_params.gyro_period =
                    game_ini_r_single_def(section, "lens_gyro_sound_period", lens_params.gyro_period);
                buf->SetLensParams(lens_params);
            }

            if (game_ini_line_exist(section, "flame_particles"))
            {
                ChangeParticles(wpn, game_ini_read_string(section, "flame_particles"), CWEAPON_FLAME_PARTICLES);
                if (!IsSilencerAttached(wpn))
                {
                    ChangeParticles(
                        wpn, game_ini_read_string(section, "flame_particles"), CWEAPON_FLAME_PARTICLES_CURRENT);
                }
            }
            if (game_ini_line_exist(section, "smoke_particles"))
            {
                ChangeParticles(wpn, game_ini_read_string(section, "smoke_particles"), CWEAPON_SMOKE_PARTICLES);
                if (!IsSilencerAttached(wpn))
                {
                    ChangeParticles(
                        wpn, game_ini_read_string(section, "smoke_particles"), CWEAPON_SMOKE_PARTICLES_CURRENT);
                }
            }
            if (game_ini_line_exist(section, "shell_particles"))
            {
                ChangeParticles(wpn, game_ini_read_string(section, "shell_particles"), CWEAPON_SHELL_PARTICLES);
            }

            if (game_ini_line_exist(section, "actor_camera_speed_factor"))
            {
                buf->SetCameraSpeed(GetCamSpeedDef() * game_ini_r_single(section, "actor_camera_speed_factor"));
            }
        }

        for (u32 i = 0; i < GetInstalledUpgradesCount(wpn); ++i)
        {
            section = GetInstalledUpgradeSection(wpn, i);
            section = game_ini_read_string(section, "section");
            if (game_ini_line_exist(section, "show_bones"))
                SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "show_bones"), true);

            //для сокрытия костей, отвечающих за предыдущие апы ветки
            if (game_ini_line_exist(section, "hide_bones_override"))
                SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(section, "hide_bones_override"), false);

            if (IsSilencerAttached(wpn))
            {
                if (game_ini_line_exist(section, "hide_bones_override_when_silencer_attached"))
                {
                }
                SetWeaponMultipleBonesStatus(
                    wpn, game_ini_read_string(section, "hide_bones_override_when_silencer_attached"), false);
            }

            if (IsScopeAttached(wpn))
            {
                if (game_ini_line_exist(section, "hide_bones_override_when_scope_attached"))
                {
                    SetWeaponMultipleBonesStatus(
                        wpn, game_ini_read_string(section, "hide_bones_override_when_scope_attached"), false);
                }
            }

            if ((gl_status == addonStatusPermanent) || ((gl_status == addonStatusAttachable) && IsGLAttached(wpn)))
            {
                if (game_ini_line_exist(section, "hide_bones_override_when_gl_attached"))
                    SetWeaponMultipleBonesStatus(
                        wpn, game_ini_read_string(section, "hide_bones_override_when_gl_attached"), false);
            }

            if (gl_enabled && (leftstr(GetActualCurrentAnim(wpn), length("anm_switch")) != "anm_switch"))
            {
                if (game_ini_line_exist(section, "hide_bones_override_grenade"))
                {
                    SetWeaponMultipleBonesStatus(
                        wpn, game_ini_read_string(section, "hide_bones_override_grenade"), false);
                }
            }
        }
    }

    section = GetSection(wpn);
    if ((gl_status == addonStatusPermanent) || ((gl_status == addonStatusAttachable) && IsGLAttached(wpn)))
    {
        if (game_ini_line_exist(section, "def_hide_bones_override_when_gl_attached"))
        {
            SetWeaponMultipleBonesStatus(
                wpn, game_ini_read_string(section, "def_hide_bones_override_when_gl_attached"), false);
        }
    }
}

void ProcessScope(CWeapon* wpn)
{
    s32 cur_index;
    if (IsScopeAttached(wpn) && (GetScopeStatus(wpn) == addonStatusAttachable))
        cur_index = GetCurrentScopeIndex(wpn);
    else
        cur_index = -1;

    u32 total_scope_cnt = GetScopesCount(wpn);

    for (u32 i = 0; i < total_scope_cnt; ++i)
    {
        pcstr tmp = GetScopeSection(wpn, i);
        bool status = (i == cur_index);

        if (game_ini_line_exist(tmp, "bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "bones"), status);
        }

        if (game_ini_line_exist(tmp, "hide_bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "hide_bones"), !status);
        }
    }

    if (cur_index >= 0)
    {
        pcstr tmp = GetScopeSection(wpn, cur_index);
        if (game_ini_line_exist(tmp, "overriding_hide_bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "overriding_hide_bones"), false);
        }

        if (game_ini_line_exist(tmp, "overriding_show_bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "overriding_show_bones"), true);
        }
    }
    else
    {
        pcstr tmp = GetSection(wpn);
        if (game_ini_line_exist(tmp, "no_scope_overriding_hide_bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "no_scope_overriding_hide_bones"), false);
        }
        if (game_ini_line_exist(tmp, "no_scope_overriding_show_bones"))
        {
            SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(tmp, "no_scope_overriding_show_bones"), true);
        }
    }
}

void CheckRLHasActiveRocket(CWeapon* wpn, CRocketLauncher* rl)
{
    IGameObject* act = static_cast<IGameObject*>(GetActor());
    bool is_act_own = (act != nullptr) && (act == GetOwner(wpn));

    if (GetRocketsCount(rl) > 0)
    {
        if ((!IsActionProcessing(wpn) || (is_act_own && IsActorControlled())) && (GetAmmoInMagCount(wpn) > 0))
        {
            pcstr sect = GetSection(wpn);

            float start_tr = game_ini_r_single_def(sect, "rocket_misfunc_start_condition", 0);
            float end_tr = game_ini_r_single_def(sect, "rocket_misfunc_end_condition", 0);
            float start_prob = game_ini_r_single_def(sect, "rocket_misfunc_start_probability", 0);
            float end_prob = game_ini_r_single_def(sect, "rocket_misfunc_end_probability", 0);
            float cond = GetCurrentCondition(wpn);

            bool is_expl;
            if ((GetOwner(wpn) != act) || (cond > start_tr) || (start_tr = end_tr))
            {
                is_expl = false;
            }
            else if (cond < end_tr)
            {
                is_expl = random() < end_prob;
            }
            else
            {
                is_expl = random() < start_prob + (end_prob - start_prob) * (start_tr - cond) / (start_tr - end_tr);
            }

            if ((IsActorControlled() && is_act_own) || is_expl)
            {
                Fvector3 p = *GetPosition(wpn);
                Fvector3 n;
                n.set(0, 1, 0);

                CCustomRocket* r = GetRocket(rl, 0);
                DetachRocket(rl, GetCObjectID(r), true);
                virtual_CCustomRocket__Contact(r, &p, &n);

                UnloadRockets(rl);
                virtual_CWeaponMagazined__UnloadMagazine(static_cast<CWeaponMagazined*>(wpn), false);
            }
            else
            {
                virtual_Action(wpn, kWPN_FIRE, kActPress);
            }
        }
        else
        {
            UnloadRockets(rl);
        }
    }
}

void CWeapon__ModUpdate(CWeapon* wpn)
{
    // TODO: избавиться от необходимости подсматривать в серверный объект
    CSE_Abstract* so = get_server_object_by_id(GetID(wpn));

    if (so == nullptr)
        return;
    pcstr sect = GetSection(wpn);

    u32 gl_ammotype = 0;
    CWeaponMagazinedWGrenade* rl = dynamic_cast<CWeaponMagazinedWGrenade*>(wpn);
    if (rl != nullptr)
    {
        if (GetRocketsCount(rl) > 0)
        {
            CWeaponMagazinedWGrenade__LaunchGrenade(rl);
        }
        //[bug] баг - отсутствует выставление a_elapsed_grenades в апдейт-пакете, из-за чего грены прогружаются
        //некорректно. По-хорошему, надо править не так топорно, а модифицированием методов экспорта и импорта
        //нетпакетов
        // TODO: избавиться для МП
        u32 gl_ammocnt = GetAmmoInGLCount(wpn);
        if (gl_ammocnt > 0)
        {
            gl_ammotype = GetCartridgeType(GetGrenadeCartridgeFromGLVector(rl, gl_ammocnt - 1));
        }
        else
        {
            gl_ammotype = GetAmmoTypeIndex(wpn, !IsGrenadeMode(wpn));
        }
        EngineFriendWrapper::UpdateElapsedGrenadesCount(static_cast<CSE_ALifeItemWeapon*>(so), gl_ammocnt, gl_ammotype);
    }
    else
    {
        CRocketLauncher* rlauncher = dynamic_cast<CRocketLauncher*>(wpn);
        if (rlauncher != nullptr)
        {
            CheckRLHasActiveRocket(wpn, rlauncher);
        }
    }

    if (game_ini_r_bool_def(sect, "action_animator", false))
    {
        if (static_cast<CHudItemObject*>(wpn) == GetActorActiveItem())
        {
            if (GetCurrentState(wpn) == EHudStates__eShowing)
            {
                if ((GetActorTargetSlot() == GetActorActiveSlot()) &&
                    !game_ini_r_bool_def(sect, "disable_autochange_slot", false))
                {
                    s32 prevslot = GetActorPreviousSlot();

                    if ((prevslot >= 0) && (prevslot != GetActorActiveSlot()) &&
                        (ItemInSlot(GetActor(), prevslot) != nullptr))
                    {
                        ActivateActorSlot(prevslot);
                    }
                    else
                    {
                        ActivateActorSlot(0);
                    }
                }
            }
            else
            {
                if ((GetActorTargetSlot() == GetActorActiveSlot()) && IsActorActionAnimatorAutoshow() &&
                    ((GetActorActionCallback() != nullptr) ||
                        !game_ini_r_bool_def(sect, "disable_autochange_slot", false)))
                {
                    virtual_CHudItem_SwitchState(wpn, EHudStates__eShowing);
                };
            }
        }
        else
        {
            if ((GetActor == nullptr) || (GetOwner(wpn) != static_cast<IGameObject*>(GetActor())))
            {
                alife_release(get_server_object_by_id(GetID(wpn)));
            }
            else
            {
                s32 slot = game_ini_r_int_def(sect, "slot", 0) + 1;
                if (GetActorTargetSlot() != slot)
                {
                    alife_release(get_server_object_by_id(GetID(wpn)));
                    RestoreLastActorDetector();
                }
            }
        }
    }

    if ((GetActorActiveItem() == static_cast<CHudItemObject*>(wpn)) && DOFChanged() && (!IsAimNow(wpn)) &&
        !IsHolderInAimState(wpn) && (GetAnimTimeState(wpn, ANM_TIME_CUR) > 0))
    {
        s32 offset = ReadActionDOFTimeOffset(wpn, GetActualCurrentAnim(wpn));
        if (offset > 0)
        {
            if (GetTimeDeltaSafe(GetAnimTimeState(wpn, ANM_TIME_START), GetAnimTimeState(wpn, ANM_TIME_CUR)) > offset)
            {
                ResetDOF(ReadActionDOFSpeed_Out(wpn, GetActualCurrentAnim(wpn)));
            }
        }
        else if (offset < 0)
        {
            if (GetTimeDeltaSafe(GetAnimTimeState(wpn, ANM_TIME_CUR), GetAnimTimeState(wpn, ANM_TIME_END)) <
                u32(-1 * offset))
            {
                ResetDOF(ReadActionDOFSpeed_Out(wpn, GetActualCurrentAnim(wpn)));
            }
        }
    }

    WpnBuf* buf = GetBuffer(wpn);
    if (buf != nullptr)
    {
        if (!buf->Update())
        {
            LogText(string("Failed to update wpn: ") + ptrtohex(wpn));
        }
        if (!buf->IsLaserInstalled() && game_ini_r_bool_def(sect, "laser_installed", false))
        {
            buf->InstallLaser(sect);
        }

        if (!buf->IsTorchInstalled() && game_ini_r_bool_def(sect, "torch_installed", false))
        {
            buf->InstallTorch(sect);
        }

        if (game_ini_line_exist(sect, "collimator_sights_bones"))
        {
            conditional_breaking_params bp = buf->GetCollimatorBreakingParams();
            if (((GetAimFactor(wpn) > 0) &&
                    (buf->IsLastZoomAlter() || (buf->GetAlterZoomDirectSwitchMixupFactor() > EPS)) &&
                    game_ini_r_bool_def(sect, "hide_collimator_sights_in_alter_zoom", true)) ||
                (GetCurrentCondition(wpn) < bp.end_condition))
            {
                SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(sect, "collimator_sights_bones"), false);
            }
            else if ((GetCurrentCondition(wpn) < bp.start_condition) || (CurrentElectronicsProblemsCnt() > 0))
            {
                float probability = 1;
                if (bp.start_condition = bp.end_condition)
                {
                    probability = bp.end_condition;
                }
                else
                {
                    probability = bp.start_probability +
                        (bp.start_condition - GetCurrentCondition(wpn)) * (1 - bp.start_probability) /
                            (bp.start_condition - bp.end_condition);
                }

                float collim_problems_cnt = buf->GetCollimatorProblemsLevel();
                if ((CurrentElectronicsProblemsCnt() > 0) && (collim_problems_cnt > 0))
                {
                    if (CurrentElectronicsProblemsCnt() >= collim_problems_cnt)
                    {
                        probability = 1;
                    }
                    else
                    {
                        float probability2 = CurrentElectronicsProblemsCnt() / collim_problems_cnt;
                        if (probability2 > probability)
                        {
                            probability = probability2;
                        }
                    }
                }
                SetWeaponMultipleBonesStatus(
                    wpn, game_ini_read_string(sect, "collimator_sights_bones"), !(random() < probability));
            }
            else
            {
                SetWeaponMultipleBonesStatus(wpn, game_ini_read_string(sect, "collimator_sights_bones"), true);
            }
        }

        if (((GetActor() == nullptr) || (GetOwner(wpn) != static_cast<IGameObject*>(GetActor()))) ||
            (GetActorActiveItem() != static_cast<CHudItemObject*>(wpn)))
        {
            if (IsExplosed(wpn))
                OnWeaponExplode_AfterAnim(wpn, 0);

            if (leftstr(GetCurAnim(wpn), length("anm_attach_scope_")) == "anm_attach_scope_")
                DetachAddon(wpn, addonTypeScope);

            if (leftstr(GetCurAnim(wpn), length("anm_attach_gl")) == "anm_attach_gl")
                DetachAddon(wpn, addonTypeGL);

            if (leftstr(GetCurAnim(wpn), length("anm_attach_sil")) == "anm_attach_sil")
                DetachAddon(wpn, addonTypeSilencer);
        }
    }

    //Обработаем установленные апгрейды
    ProcessUpgrade(wpn);
    //Теперь отобразим установленный прицел
    ProcessScope(wpn);
    //Разберемся с визуализацией патронов
    /*ProcessAmmo(wpn);
    ProcessAmmoGL(wpn);
    //анимы от 3-го лица
    ReassignWorldAnims(wpn);
    //Визуализация режима огня
    ProcessFiremode(wpn);

    if (buf != nullptr)
    {
        ProcessLaserDot(wpn);
    }

    if (buf != nullptr)
    {
        buf->UpdateTorch();
    }*/
}
} // namespace GunslingerMod
