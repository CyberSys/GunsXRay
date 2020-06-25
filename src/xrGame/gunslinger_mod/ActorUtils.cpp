#include "StdAfx.h"
#include "ActorUtils.h"
#include "DetectorUtils.h"
#include "HudItemUtils.h"

#include "EngineFriends.h"
#include "hud_item_object.h"
#include "inventory_item.h"
#include "CustomDetector.h"

namespace GunslingerMod
{
static TAnimationEffector _action_animator_callback = nullptr;
static bool _action_animator_autoshow = false;

CActor* GetActor()
{
    CActor* res = nullptr;
    if (g_pGameLevel != nullptr)
    {
        IGameObject* o = EngineFriendWrapper::GetCurrentEntity();
        if (o != nullptr)
            res = dynamic_cast<CActor*>(o);
    }
    return res;
}

s32 GetActorTargetSlot()
{
    int res = -1;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        res = EngineFriendWrapper::GetTargetSlot(act);
    }
    return res;
}

s32 GetActorActiveSlot()
{
    int res = -1;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        res = EngineFriendWrapper::GetActiveSlot(act);
    }
    return res;
}

s32 GetActorPreviousSlot()
{
    int res = -1;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        res = EngineFriendWrapper::GetPreviousSlot(act);
    }
    return res;
}

void RestoreLastActorDetector()
{
    CActor* act = GetActor();
    if (!WasLastDetectorHiddenManually())
    {
        CInventoryItem* itm = ItemInSlot(act, 9);
        CCustomDetector* detector = dynamic_cast<CCustomDetector*>(itm);
        if ((detector != nullptr) && (!GetDetectorActiveStatus(detector)))
        {
            SetDetectorForceUnhide(detector, true);
            SetActorActionState(act, actShowDetectorNow, true);
        }
    }
}

bool GetActorActionState(CActor* act, u32 mask, EActionStateType state)
{
    return (GetActorActionStateInt(act, mask, state) != 0);
}

u32 GetActorActionStateInt(CActor* act, u32 mask, EActionStateType state)
{
    u32 val = 0;
    switch (state)
    {
    case mState_WISHFUL:
    {
        val = EngineFriendWrapper ::GetWishfulMState(act);
        break;
    }
    case mState_REAL:
    {
        val = EngineFriendWrapper ::GetRealMState(act);
        break;
    }
    case mState_OLD:
    {
        val = EngineFriendWrapper ::GetOldMState(act);
        break;
    }
    }

    u32 res = val & mask;
    return res;
}

void CreateObjectToActor(pcstr section)
{
    CActor* act = GetActor();
    if (act == nullptr)
        return;
    EngineFriendWrapper::alife_create(section, GetEntityPosition(act), 0, 0, 0);
}

bool IsHolderInAimState(CHudItemObject* wpn)
{
    IGameObject* holder = GetOwner(wpn);
    CActor* actor = GetActor();
    if ((actor != nullptr) && (static_cast<IGameObject*>(actor) == holder) &&
        (GetActorActionState(actor, actAimStarted)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SetActorActionState(CActor* act, u32 mask, bool set_value, EActionStateType state)
{
    u32 val = GetActorActionStateInt(act, actTotalActions, state);

    if (set_value)
    {
        val |= mask;
    }
    else
    {
        val &= ~mask;
    }

    switch (state)
    {
    case mState_WISHFUL:
    {
        EngineFriendWrapper ::SetWishfulMState(act, val);
        break;
    }
    case mState_REAL:
    {
        EngineFriendWrapper ::SetRealMState(act, val);
        break;
    }
    case mState_OLD:
    {
        EngineFriendWrapper ::SetOldMState(act, val);
        break;
    }
    }
}

CHudItemObject* GetActorActiveItem()
{
    CHudItemObject* res = nullptr;
    CActor* act = GetActor();
    if (act != nullptr)
    {
        CInventoryItem* itm = ItemInSlot(act, GetActorActiveSlot());
        if (itm != nullptr)
        {
            res = static_cast<CHudItemObject*>(itm);
        }
    }
    return res;
}

CInventoryItem* ItemInSlot(CActor* act, int slot) { return EngineFriendWrapper::ItemFromSlot(act, slot); }

void ActivateActorSlot(s32 slot)
{
    CActor* act = GetActor();
    if (act != nullptr)
    {
        EngineFriendWrapper::SetTargetSlot(act, slot);
    }
}

TAnimationEffector GetActorActionCallback() { return _action_animator_callback; }

bool IsActorActionAnimatorAutoshow() { return _action_animator_autoshow; }

const Fvector3* GetEntityDirection(IGameObject* o) { return &(EngineFriendWrapper::GetXForm(o)->k); }
const Fvector3* GetEntityPosition(IGameObject* o) { return &(EngineFriendWrapper::GetXForm(o)->c); }
} // namespace GunslingerMod
