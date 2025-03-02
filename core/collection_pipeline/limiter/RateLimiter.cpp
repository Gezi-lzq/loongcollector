// Copyright 2024 iLogtail Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "collection_pipeline/limiter/RateLimiter.h"

#include "logger/Logger.h"
// TODO: temporarily used
#include "app_config/AppConfig.h"
#include "common/TimeUtil.h"

using namespace std;

namespace logtail {

bool RateLimiter::IsValidToPop() {
    if (time(nullptr) != mLastSendTimeSecond) {
        mLastSecondTotalBytes = 0;
        mLastSendTimeSecond = time(nullptr);
    }
    if (mLastSecondTotalBytes > mMaxSendBytesPerSecond) {
        return false;
    }
    return true;
}

void RateLimiter::PostPop(size_t size) {
    mLastSecondTotalBytes += size;
}

void RateLimiter::FlowControl(int32_t dataSize, int64_t& lastSendTime, int32_t& lastSendByte, bool isRealTime) {
    int64_t curTime = GetCurrentTimeInMicroSeconds();
    int32_t bps = isRealTime ? AppConfig::GetInstance()->GetMaxBytePerSec() : AppConfig::GetInstance()->GetBytePerSec();
    if (curTime < lastSendTime || curTime - lastSendTime >= 1000 * 1000) {
        lastSendTime = curTime;
        lastSendByte = dataSize;
    } else {
        if (lastSendByte > bps) {
            usleep(1000 * 1000 - (curTime - lastSendTime));
            lastSendTime = GetCurrentTimeInMicroSeconds();
            lastSendByte = dataSize;
        } else {
            lastSendByte += dataSize;
        }
    }
}

} // namespace logtail
