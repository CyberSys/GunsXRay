#include "StdAfx.h"
#include "xr_BoneUtils.h"
#include "EngineFriends.h"
#include "BaseDefs.h"
#include "BaseGameData.h"
#include "ActorUtils.h"
#include "DetectorUtils.h"
#include "HudItemUtils.h"

#include "CustomDetector.h"
#include "player_hud.h"

namespace GunslingerMod
{
void SetWeaponMultipleBonesStatus(CHudItemObject* wpn, pcstr bones, bool status)
{
    string bones_string = bones;
    string bone;
    while (GetNextSubStr(bones_string, bone, ','))
    {
        SetWeaponModelBoneStatus(wpn, bone.c_str(), status);
    }
}

void SetWorldModelMultipleBonesStatus(CHudItemObject* wpn, pcstr bones, bool status)
{
    string bones_string = bones;
    string bone;
    while (GetNextSubStr(bones_string, bone, ','))
    {
        SetWorldModelBoneStatus(wpn, bone.c_str(), status);
    }
}

void SetWorldModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status)
{
    IRenderVisual* v = EngineFriendWrapper::GetVisual(wpn);
    IKinematics* ik = v->dcast_PKinematics();
    if (ik == nullptr)
        return;

    u16 bid = IKinematics__LL_BoneID(ik, bone_name);
    if (bid != (u16)(-1))
    {
        EngineFriendWrapper::SetBoneVisible(ik, bid, status, false);
    }
}
void SetHudModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status)
{
    attachable_hud_item* hid = CHudItem__HudItemData(wpn);
    if (hid != nullptr)
    {
        u16 bid = IKinematics__LL_BoneID(hid->m_model, bone_name);
        if (bid != (u16)(-1))
        {
            EngineFriendWrapper::SetBoneVisible(hid->m_model, bid, status, false);
        }
    }
}

void SetWeaponModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status)
{
    CActor* act = GetActor();
    if ((act != nullptr) &&
        ((GetActorActiveItem() == wpn) || (static_cast<CHudItemObject*>(GetActiveDetector(act)) == wpn)))
    {
        SetHudModelBoneStatus(wpn, bone_name, status);
    }
    SetWorldModelBoneStatus(wpn, bone_name, status);
}

u16 IKinematics__LL_BoneID(IKinematics* ik, pcstr name) { return EngineFriendWrapper::GetLLBoneId(ik, name); }
} // namespace GunslingerMod
