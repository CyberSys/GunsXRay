#include "StdAfx.h"
#include "Misc.h"
#include "BaseDefs.h"

namespace GunslingerMod
{
static u32 previous_electronics_problems_counter = 0;
static u32 target_electronics_problems_counter = 0;
static u32 current_electronics_problems_counter = 0;
static bool last_problems_update_was_decrease = false;

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
} // namespace GunslingerMod
