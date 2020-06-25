#pragma once

class CHudItemObject;
class IKinematics;

namespace GunslingerMod
{
void SetWorldModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status);
void SetHudModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status);
void SetWeaponModelBoneStatus(CHudItemObject* wpn, pcstr bone_name, bool status);
void SetWeaponMultipleBonesStatus(CHudItemObject* wpn, pcstr bones, bool status);
void SetWorldModelMultipleBonesStatus(CHudItemObject* wpn, pcstr bones, bool status);
u16 IKinematics__LL_BoneID(IKinematics* ik, pcstr name);
} // namespace GunslingerMod
