#pragma once

namespace GunslingerMod
{
bool ElectronicsProblemsInc();
bool ElectronicsProblemsDec();
void ResetElectronicsProblems();
bool ElectronicsProblemsImmediateApply();

void ReadFromReader(IReader* r, void* buf, size_t bytes_count);
u32 ReaderLength(IReader* r);
u32 ReaderElapsed(IReader* r);
} // namespace GunslingerMod
