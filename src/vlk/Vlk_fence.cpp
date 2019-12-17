//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#include "std_lib_modules.h"
#include "vlk_lib_modules.h"
#include "Vlk_fence.h"
#include "Vlk_device.h"

using namespace std;

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

Vlk_fence::Vlk_fence(const Fence_desc& desc, Vlk_device* device) :
    Fence(),
    device_ { device },
    fence_ { VK_NULL_HANDLE }
{
    init_fence_(desc);
}

//----------------------------------------------------------------------------------------------------------------------

void Vlk_fence::wait_signal()
{
    vkWaitForFences(device_->device(), 1, &fence_, false, UINT64_MAX);
}

//----------------------------------------------------------------------------------------------------------------------

void Vlk_fence::reset()
{
    vkResetFences(device_->device(), 1, &fence_);
}

//----------------------------------------------------------------------------------------------------------------------

Device* Vlk_fence::device() const
{
    return device_;
}

//----------------------------------------------------------------------------------------------------------------------

bool Vlk_fence::signaled() const
{
    return !vkGetFenceStatus(device_->device(), fence_);
}

//----------------------------------------------------------------------------------------------------------------------

void Vlk_fence::init_fence_(const Fence_desc& desc)
{
    // configure a fence create info.
    VkFenceCreateInfo create_info {};

    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = desc.signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    // try to create a fence.
    if (vkCreateFence(device_->device(), &create_info, nullptr, &fence_))
        throw runtime_error("fail to create a fence");
}

//----------------------------------------------------------------------------------------------------------------------

} // of namespace Gfx_lib