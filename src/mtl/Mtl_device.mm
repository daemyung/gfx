//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#include "std_lib.h"
#include "mtl_lib.h"
#include "Mtl_device.h"
#include "Mtl_buffer.h"
#include "Mtl_image.h"
#include "Mtl_sampler.h"
#include "Mtl_shader.h"
#include "Mtl_pipeline.h"
#include "Mtl_swap_chain.h"
#include "Mtl_cmd_buffer.h"
#include "Mtl_fence.h"

using namespace std;

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

Mtl_device::Mtl_device() :
    device_ { nil },
    command_queue_ { nil },
    used_command_buffers_ { [NSMutableSet new] },
    queue_mutex_ {}
{
    init_device_();
    init_command_queue_();
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Buffer> Mtl_device::create(const Buffer_desc& desc)
{
    return make_unique<Mtl_buffer>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Image> Mtl_device::create(const Image_desc& desc)
{
    return make_unique<Mtl_image>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Sampler> Mtl_device::create(const Sampler_desc& desc)
{
    return make_unique<Mtl_sampler>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Shader> Mtl_device::create(const Shader_desc& desc)
{
    return make_unique<Mtl_shader>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Pipeline> Mtl_device::create(const Pipeline_desc& desc)
{
    return make_unique<Mtl_pipeline>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Swap_chain> Mtl_device::create(const Swap_chain_desc& desc)
{
    return make_unique<Mtl_swap_chain>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Cmd_buffer> Mtl_device::create(const Cmd_buffer_desc& desc)
{
    return make_unique<Mtl_cmd_buffer>(this);
}

//----------------------------------------------------------------------------------------------------------------------

std::unique_ptr<Fence> Mtl_device::create(const Fence_desc& desc)
{
    return make_unique<Mtl_fence>(desc, this);
}

//----------------------------------------------------------------------------------------------------------------------

void Mtl_device::submit(Cmd_buffer* cmd_buffer, Fence* fence)
{
    auto mtl_cmd_buffer = static_cast<Mtl_cmd_buffer*>(cmd_buffer);
    __block auto mtl_fence = static_cast<Mtl_fence*>(fence);

    [mtl_cmd_buffer->command_buffer() addCompletedHandler:^(id<MTLCommandBuffer> command_buffer) {
        if (queue_mutex_.try_lock()) {
            [used_command_buffers_ removeObject:command_buffer];
            queue_mutex_.unlock();
        }

        if (mtl_fence)
            dispatch_semaphore_signal(mtl_fence->semaphore());
    }];
    [mtl_cmd_buffer->command_buffer() commit];

    lock_guard<mutex> lock { queue_mutex_ };

    [used_command_buffers_ addObject:mtl_cmd_buffer->command_buffer()];
}

//----------------------------------------------------------------------------------------------------------------------

void Mtl_device::wait_idle()
{
    lock_guard<mutex> lock { queue_mutex_ };

    for (id<MTLCommandBuffer> command_buffer in used_command_buffers_)
        [command_buffer waitUntilCompleted];

    [used_command_buffers_ removeAllObjects];
}

//----------------------------------------------------------------------------------------------------------------------

void Mtl_device::init_device_()
{
    device_ = MTLCreateSystemDefaultDevice();

    if (!device_)
        throw runtime_error("fail to create device");
}

//----------------------------------------------------------------------------------------------------------------------

void Mtl_device::init_command_queue_()
{
    command_queue_ = [device_ newCommandQueue];

    if (!command_queue_)
        throw runtime_error("fail to create device");
}

//----------------------------------------------------------------------------------------------------------------------
    
} // of namespace Gfx_lib
