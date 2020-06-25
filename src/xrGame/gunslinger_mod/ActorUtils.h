#pragma once

#include "WeaponAdditionalBuffer.h"

#include "Level.h"
#include "Actor.h"
#include "hud_item_object.h"

namespace GunslingerMod
{
const u32 actModNeedBlowoutAnim = 0x02000000;
const u32 actAimStarted = 0x04000000;
const u32 actShowDetectorNow = 0x08000000; //преддоставание проигралoсь, можно показывать детектор
const u32 actModSprintStarted = 0x10000000;
const u32 actModNeedMoveReassign = 0x20000000;
const u32 actModDetectorSprintStarted = 0x40000000;
const u32 actModDetectorAimStarted = 0x80000000;

const u32 actTotalActions = (u32)(-1);

enum EActionStateType
{
    mState_WISHFUL,
    mState_OLD,
    mState_REAL,
};

CActor* GetActor();
s32 GetActorTargetSlot();
s32 GetActorActiveSlot();
s32 GetActorPreviousSlot();
void RestoreLastActorDetector();
bool GetActorActionState(CActor* act, u32 mask, EActionStateType state = mState_REAL);
u32 GetActorActionStateInt(CActor* act, u32 mask, EActionStateType state = mState_REAL);

void CreateObjectToActor(pcstr section);

bool IsHolderInAimState(CHudItemObject* wpn);
void SetActorActionState(CActor* act, u32 mask, bool set_value, EActionStateType state = mState_REAL);
CHudItemObject* GetActorActiveItem();
CInventoryItem* ItemInSlot(CActor* act, int slot);

void ActivateActorSlot(s32 slot);

TAnimationEffector GetActorActionCallback();
bool IsActorActionAnimatorAutoshow();

const Fvector3* GetEntityDirection(IGameObject* o);
const Fvector3* GetEntityPosition(IGameObject* o);
} // namespace GunslingerMod
