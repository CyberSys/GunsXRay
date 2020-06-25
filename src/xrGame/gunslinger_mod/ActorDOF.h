#pragma once

#include "BaseDefs.h"

namespace GunslingerMod
{
void ResetDOF(float speed);

float ReadActionDOFSpeed_Out(CHudItemObject* wpn, string param);
s32 ReadActionDOFTimeOffset(CHudItemObject* wpn, string param);

void SetDofSpeedfactor(float speed);

bool DOFChanged();
} // namespace GunslingerMod
