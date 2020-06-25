#pragma once

class CCustomDetector;
class CActor;

namespace GunslingerMod
{
void SetDetectorForceUnhide(CCustomDetector* det, bool status);

CCustomDetector* GetActiveDetector(CActor* act);

bool GetDetectorActiveStatus(CCustomDetector* det);
bool WasLastDetectorHiddenManually();
} // namespace GunslingerMod
