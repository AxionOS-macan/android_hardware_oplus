/*
 * SPDX-FileCopyrightText: 2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.oplus"

#include "HighTouchPollingRate.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>

#include <cctype>

#include <OplusTouchConstants.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kGameSwitchEnablePath = "/proc/touchpanel/game_switch_enable";
constexpr const char* kTouchReportRateProperty = "sys.touch.report_rate";
constexpr int kDefaultReportRateMode = 3;

// Explicit Soong settings override automatic node detection and the rate property.
#if defined(HTPR_ENABLE_NODE) || defined(HTPR_ENABLE_VALUE)
constexpr bool kUseConfiguredHtpr = true;
#else
constexpr bool kUseConfiguredHtpr = false;
#endif

#ifndef HTPR_ENABLE_NODE
#define HTPR_ENABLE_NODE OplusTouchConstants::GAME_SWITCH_ENABLE_NODE
#endif
#ifndef HTPR_ENABLE_VALUE
#define HTPR_ENABLE_VALUE "1"
#endif

bool parseEnabled(const std::string& value) {
    if (value.empty()) {
        return false;
    }

    const std::string key = "high_frame_value:";
    size_t pos = value.find(key);
    if (pos != std::string::npos) {
        pos += key.length();
        while (pos < value.length() && std::isspace(static_cast<unsigned char>(value[pos]))) {
            pos++;
        }
        return pos < value.length() && value[pos] != '0';
    }

    return value[0] != '0';
}

std::string getLegacyReportRateValue(int mode) {
    switch (mode) {
        case 3:
            return "258";
        case 4:
            return "78";
        case 10:
            return "e";
        case 11:
            return "f";
        case 12:
            return "10";
        case 13:
            return "11";
        default:
            return "0";
    }
}

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

HighTouchPollingRate::HighTouchPollingRate(std::shared_ptr<IOplusTouch> oplusTouch)
    : mOplusTouch(std::move(oplusTouch)) {}

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    std::string value;

    if (mOplusTouch) {
        int node = HTPR_ENABLE_NODE;
        if (!kUseConfiguredHtpr) {
            int supported = 0;
            mOplusTouch->isTouchNodeSupport(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                            OplusTouchConstants::HIGH_FRAME_ENABLE_NODE,
                                            &supported);
            node = supported == 1 ? OplusTouchConstants::HIGH_FRAME_ENABLE_NODE
                                  : OplusTouchConstants::GAME_SWITCH_ENABLE_NODE;
        }
        ndk::ScopedAStatus status =
                mOplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID, node, &value);
        if (!status.isOk()) {
            LOG(ERROR) << "Failed to read current AIDL HighTouchPollingRate state";
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
    } else if (!ReadFileToString(kGameSwitchEnablePath, &value)) {
        LOG(ERROR) << "Failed to read current HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    *_aidl_return = parseEnabled(value);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enable) {
    if (mOplusTouch) {
        int node = HTPR_ENABLE_NODE;
        std::string value = enable ? HTPR_ENABLE_VALUE : "0";
        if (!kUseConfiguredHtpr) {
            int supported = 0;
            const int mode = enable ? android::base::GetIntProperty(kTouchReportRateProperty,
                                                                    kDefaultReportRateMode)
                                    : 0;
            mOplusTouch->isTouchNodeSupport(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                            OplusTouchConstants::REPORT_RATE_MODE_NODE, &supported);
            if (supported == 1) {
                node = OplusTouchConstants::REPORT_RATE_MODE_NODE;
                value = std::to_string(mode);
            } else {
                mOplusTouch->isTouchNodeSupport(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                                OplusTouchConstants::HIGH_FRAME_ENABLE_NODE,
                                                &supported);
                node = supported == 1 ? OplusTouchConstants::HIGH_FRAME_ENABLE_NODE
                                      : OplusTouchConstants::GAME_SWITCH_ENABLE_NODE;
                value = supported == 1 ? (enable ? "1" : "0") : getLegacyReportRateValue(mode);
            }
        }

        int aidl_return = 0;
        ndk::ScopedAStatus status = mOplusTouch->touchWriteNodeFile(
                OplusTouchConstants::DEFAULT_TP_IC_ID, node, value, &aidl_return);
        if (!status.isOk() || aidl_return < 0) {
            LOG(ERROR) << "Failed to write AIDL HighTouchPollingRate state";
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
    } else if (!WriteStringToFile(enable ? "1" : "0", kGameSwitchEnablePath, true)) {
        LOG(ERROR) << "Failed to write HighTouchPollingRate state";
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
