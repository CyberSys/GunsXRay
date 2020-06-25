#include "StdAfx.h"
#include "Misc.h"
#include "BaseDefs.h"
#include "BaseGameData.h"

namespace GunslingerMod
{
static u32 previous_electronics_problems_counter = 0;
static u32 target_electronics_problems_counter = 0;
static u32 current_electronics_problems_counter = 0;
static bool last_problems_update_was_decrease = false;

CScriptGameObject* game_object_GetScriptGameObject(CGameObject* obj)
{
    return EngineFriendWrapper::GetScriptGameObject(obj);
}

CSE_Abstract* get_server_object_by_id(u16 id) { return EngineFriendWrapper::ID_to_entity(id); }

void alife_release(CSE_Abstract* srv_obj) { EngineFriendWrapper::alife_release(srv_obj); }

void ResetElectronicsProblems() { target_electronics_problems_counter = 0; }

void ResetElectronicsProblems_Full()
{
    ResetElectronicsProblems();
    current_electronics_problems_counter = 0;
    previous_electronics_problems_counter = 0;
    last_problems_update_was_decrease = false;
}

float PreviousElectronicsProblemsCnt() { return static_cast<float>(previous_electronics_problems_counter); }

bool ElectronicsProblemsImmediateApply()
{
    current_electronics_problems_counter = target_electronics_problems_counter;
    return true;
}

bool ElectronicsProblemsInc()
{
    target_electronics_problems_counter = target_electronics_problems_counter + 1;
    return true;
}

float TargetElectronicsProblemsCnt() { return target_electronics_problems_counter; }

float CurrentElectronicsProblemsCnt() { return current_electronics_problems_counter; }

bool ElectronicsProblemsDec()
{
    if (target_electronics_problems_counter > 0)
    {
        target_electronics_problems_counter = target_electronics_problems_counter - 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool IsElectronicsProblemsDecreasing() { return last_problems_update_was_decrease; }

void UpdateElectronicsProblemsCnt(TimeTicks dt)
{
    previous_electronics_problems_counter = current_electronics_problems_counter;
    if (target_electronics_problems_counter = current_electronics_problems_counter)
        return;

    float max_delta = static_cast<float>(dt) / 2000.0;
    float delta = static_cast<float>(target_electronics_problems_counter) -
        static_cast<float>(current_electronics_problems_counter);

    if (abs(delta) <= abs(max_delta))
    {
        current_electronics_problems_counter = target_electronics_problems_counter;
    }
    else
    {
        s32 sign = (delta > 0) ? 1 : -1;
        current_electronics_problems_counter = current_electronics_problems_counter + sign * max_delta;
        last_problems_update_was_decrease = (delta < 0);
    }
}

void ReadFromReader(IReader* r, void* buf, size_t bytes_count)
{
    return EngineFriendWrapper::ReadFromReader(r, buf, bytes_count);
}
u32 ReaderLength(IReader* r) { return EngineFriendWrapper::ReaderLength(r); }
u32 ReaderElapsed(IReader* r) { return EngineFriendWrapper::ReaderElapsed(r); }

void IWriter__w_u32(IWriter* w, u32 value) { EngineFriendWrapper::WriteU32ToWriter(w, value); }
void IWriter__w_stringZ(IWriter* w, pcstr value) { EngineFriendWrapper::WriteStringZToWriter(w, value); }

u16 GetCObjectID(IGameObject* o) { return EngineFriendWrapper::GetID(o); }
} // namespace GunslingerMod
