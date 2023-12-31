/*
* Copyright (c) 2017, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <utils/utils.h>
#include <private/hw_info_interface.h>

#ifndef TARGET_HEADLESS
#include "hw_info_drm.h"
#endif

#define __CLASS__ "HWInfoInterface"

namespace sdm {

int32_t HWInfoInterface::ref_count_ = 0;
HWInfoInterface* HWInfoInterface::intf_ = nullptr;

DisplayError HWInfoInterface::Create(HWInfoInterface **intf) {
#ifndef TARGET_HEADLESS
  if (ref_count_ > 0 && intf_) {
    ref_count_++;
    *intf = intf_;
    return kErrorNone;
  }

  *intf = new HWInfoDRM();
#else
  *intf = nullptr;
#endif

  DisplayError error = kErrorNone;
  if (*intf) {
    error = (*intf)->Init();
    if (error != kErrorNone) {
      delete *intf;
      *intf = nullptr;
    }
  } else {
    error = kErrorCriticalResource;
  }

  if (*intf) {
    intf_ = *intf;
    ref_count_++;
  }

  return error;
}

DisplayError HWInfoInterface::Destroy(HWInfoInterface *intf) {
  ref_count_ = ref_count_ - 1 >= 0 ? --ref_count_ : 0;
  DLOGV("refcount: %lu", ref_count_);

  if (!ref_count_ && intf) {
    delete intf;
  }

  intf_ = nullptr;
  return kErrorNone;
}

}  // namespace sdm
