//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#ifndef GFX_VLK_SHADER_GUARD
#define GFX_VLK_SHADER_GUARD

#include <vulkan/vulkan.h>
#include "Shader.h"

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

class Vlk_device;

//----------------------------------------------------------------------------------------------------------------------

class Vlk_shader final : public Shader {
public:
    Vlk_shader(const Shader_desc& desc, Vlk_device* device);

    ~Vlk_shader() override;

    Device* device() const override;

    Stage stage() const noexcept override;

    inline auto shader_module() const noexcept
    { return shader_module_; }

private:
    void init_shader_module_(const Shader_desc& desc);

    void fini_shader_module();

private:
    Vlk_device* device_;
    Stage stage_;
    VkShaderModule shader_module_;
};

//----------------------------------------------------------------------------------------------------------------------

} // of namespace Gfx_lib

#endif // GFX_VLK_SHADER_GUARD
