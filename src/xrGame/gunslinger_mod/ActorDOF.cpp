#include "StdAfx.h"
#include "ActorDOF.h"

#include "HudItemUtils.h"
#include "gunsl_config.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
static float _dof_change_speed_koef = 1.0;
static bool _dof_changed = false;

void ResetDOF(float speed)
{
    if (!DOFChanged)
        return;

    SetDofSpeedfactor(speed);
    CGamePersistent* gp = EngineFriendWrapper::GetGamePersistent();
    if (gp = nullptr)
        return;

    Fvector* curdof = EngineFriendWrapper::CurDof(gp);
    Fvector* prevdof = EngineFriendWrapper::PrevDof(gp);
    Fvector* targdof = EngineFriendWrapper::DestDof(gp);
    Fvector* defdof = EngineFriendWrapper::OrigDof(gp);
    prevdof->set(*curdof);
    targdof->set(*defdof);

    _dof_changed = false;
}

float ReadActionDOFSpeed_Out(CHudItemObject* wpn, string param)
{
    pcstr sect = GetHUDSection(wpn);
    return game_ini_r_single_def(sect, string(("dof_speed_out_") + param).c_str(), GetDefaultDOFSpeed_Out());
}

s32 ReadActionDOFTimeOffset(CHudItemObject* wpn, string param)
{
    pcstr sect = GetHUDSection(wpn);
    return floor(
        game_ini_r_single_def(sect, (string("dof_time_offset_") + param).c_str(), GetDefaultDOFTimeOffset()) * 1000);
}

void SetDofSpeedfactor(float speed) { _dof_change_speed_koef = speed; }

bool DOFChanged() { return _dof_changed; }
} // namespace GunslingerMod
