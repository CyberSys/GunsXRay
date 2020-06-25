#include "StdAfx.h"
#include "DetectorUtils.h"
#include "EngineFriends.h"

namespace GunslingerMod
{
//должен быть всегда true, кроме случаев, когда идет быстрое использование какого-то предмета (юзейбла,
//грены, ножа),  не поддерживающего детектор-компаньон, а перед быстрым использованием детектор был
//активен и скрылся автоматом
static bool _was_detector_hidden_manually = true;

void SetDetectorForceUnhide(CCustomDetector* det, bool status) {}

CCustomDetector* GetActiveDetector(CActor* act) { return EngineFriendWrapper::GetActiveDetector(act); }

bool GetDetectorActiveStatus(CCustomDetector* det)
{
    if (det == nullptr)
        return false;
    return EngineFriendWrapper::IsDetectorActive(det);
}

bool WasLastDetectorHiddenManually() { return _was_detector_hidden_manually; }
} // namespace GunslingerMod
