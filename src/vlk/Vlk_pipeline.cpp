//
// This file is part of the "gfx" project
// See "LICENSE" for license information.
//

#include "std_lib.h"
#include "vlk_lib.h"
#include "Vlk_pipeline.h"
#include "Vlk_device.h"
#include "Vlk_shader.h"
#include "Vlk_render_pass.h"

using namespace std;
using namespace Gfx_lib;

namespace {

//----------------------------------------------------------------------------------------------------------------------

inline auto to_render_pass_desc(const Multisample& multisample, const Output_merger& output_merger)
{
    // configure a render pass desc.
    Vlk_render_pass_desc render_pass_desc {};

    for (auto i = 0; i != 4; ++i) {
        render_pass_desc.colors[i].format = output_merger.color_formats[i];
        render_pass_desc.colors[i].samples = multisample.samples;
    }

    render_pass_desc.depth_stencil.format = output_merger.depth_stencil_format;
    render_pass_desc.depth_stencil.samples = multisample.samples;

    return render_pass_desc;
}

//----------------------------------------------------------------------------------------------------------------------

} // of namespace

namespace Gfx_lib {

//----------------------------------------------------------------------------------------------------------------------

Vlk_pipeline::Vlk_pipeline(const Pipeline_desc& desc, Vlk_device* device) :
    Pipeline(),
    device_ { device },
    pipeline_ { VK_NULL_HANDLE }
{
    init_render_pipeline(desc);
}

//----------------------------------------------------------------------------------------------------------------------

Device* Vlk_pipeline::device() const
{
    return device_;
}

//----------------------------------------------------------------------------------------------------------------------

void Vlk_pipeline::init_render_pipeline(const Pipeline_desc& desc)
{
    // configure a vertex and a fragment shader stages.
    array<VkPipelineShaderStageCreateInfo, 2> shader_stages {};

    {
        // cast to the implementation.
        auto shader_impl = static_cast<Vlk_shader*>(desc.vertex_shader);

        shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shader_stages[0].module = shader_impl->shader_module();
        shader_stages[0].pName = "main";
    }

    {
        // cast to the implementation.
        auto shader_impl = static_cast<Vlk_shader*>(desc.fragment_shader);

        shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shader_stages[1].module = shader_impl->shader_module();
        shader_stages[1].pName = "main";
    }

    // configure vertex input bindings.
    vector<VkVertexInputBindingDescription> vertex_input_bindings;

    {
        for (auto i = 0; i != 2; ++i) {
            auto& binding = desc.vertex_input.bindings[i];

            if (UINT32_MAX == binding.stride)
                continue;

            VkVertexInputBindingDescription vertex_input_binding {};

            vertex_input_binding.binding = i;
            vertex_input_binding.stride = binding.stride;
            vertex_input_binding.inputRate = convert<VkVertexInputRate>(binding.step_rate);

            vertex_input_bindings.push_back(vertex_input_binding);
        }
    }

    // configure vertex input attributes.
    vector<VkVertexInputAttributeDescription> vertex_input_attributes;

    {
        for (auto i = 0; i != 16; ++i) {
            auto& attribute = desc.vertex_input.attributes[i];

            if (UINT32_MAX == attribute.binding)
                continue;

            VkVertexInputAttributeDescription vertex_input_attribute {};

            vertex_input_attribute.location = i;
            vertex_input_attribute.binding = attribute.binding;
            vertex_input_attribute.format = convert<VkFormat>(attribute.format);
            vertex_input_attribute.offset = attribute.offset;

            vertex_input_attributes.push_back(vertex_input_attribute);
        }
    }

    // configure a vertex input stage.
    VkPipelineVertexInputStateCreateInfo vertex_input_state {};

    {
        vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state.vertexBindingDescriptionCount = vertex_input_bindings.size();
        vertex_input_state.pVertexBindingDescriptions = &vertex_input_bindings[0];
        vertex_input_state.vertexAttributeDescriptionCount = vertex_input_attributes.size();
        vertex_input_state.pVertexAttributeDescriptions = &vertex_input_attributes[0];
    }

    // configure an input assembly state.
    VkPipelineInputAssemblyStateCreateInfo input_assembly_state {};

    {
        auto& input_assembly = desc.input_assembly;

        input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_state.topology = convert<VkPrimitiveTopology>(input_assembly.topology);
        input_assembly_state.primitiveRestartEnable = input_assembly.restart;
    }

    // configure a viewport state create info.
    VkPipelineViewportStateCreateInfo viewport_state_create_info {};

    {
        auto& create_info = viewport_state_create_info;

        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        create_info.viewportCount = 1;
        create_info.scissorCount = 1;
    }

    // configure a rasterization state.
    VkPipelineRasterizationStateCreateInfo rasterization_state {};

    {
        auto& rasterization = desc.rasterization;

        rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state.depthClampEnable = rasterization.depth_clamp;
        rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state.cullMode = convert<VkCullModeFlags>(rasterization.cull_mode);
        rasterization_state.frontFace = convert<VkFrontFace>(rasterization.front_face);
        rasterization_state.depthBiasEnable = rasterization.depth_bias;
        rasterization_state.depthBiasConstantFactor = rasterization.depth_bias_constant_factor;
        rasterization_state.depthBiasClamp = rasterization.depth_bias_clamp;
        rasterization_state.depthBiasSlopeFactor = rasterization.depth_bias_slope_factor;
    }

    // configure a multisample state create info.
    VkPipelineMultisampleStateCreateInfo multisample_state_create_info {};

    {
        auto& create_info = multisample_state_create_info;

        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    // configure a depth stencil state.
    VkPipelineDepthStencilStateCreateInfo depth_stencil_state {};

    {
        auto& depth_stencil = desc.depth_stencil;

        depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_state.depthTestEnable = depth_stencil.depth_test;
        depth_stencil_state.depthWriteEnable = depth_stencil.write_mask;
        depth_stencil_state.depthCompareOp = convert<VkCompareOp>(depth_stencil.depth_compare_op);
        depth_stencil_state.stencilTestEnable = depth_stencil.stencil_test;

        // todo: front back stencil

        depth_stencil_state.minDepthBounds = 0.0f;
        depth_stencil_state.maxDepthBounds = 1.0f;
    }

    // configure pipeline color blend attachment states.
    std::vector<VkPipelineColorBlendAttachmentState> color_blend_attachment_states;

    {
        for (auto& attachment : desc.color_blend.attachments) {
            VkPipelineColorBlendAttachmentState state {};

            state.blendEnable = attachment.blend;
            state.srcColorBlendFactor = convert<VkBlendFactor>(attachment.src_rgb_blend_factor);
            state.dstColorBlendFactor = convert<VkBlendFactor>(attachment.dst_rgb_blend_factor);
            state.colorBlendOp = convert<VkBlendOp>(attachment.rgb_blend_op);
            state.srcAlphaBlendFactor = convert<VkBlendFactor>(attachment.src_a_blend_factor);
            state.dstAlphaBlendFactor = convert<VkBlendFactor>(attachment.dst_a_blend_factor);
            state.alphaBlendOp = convert<VkBlendOp>(attachment.a_blend_op);
            state.colorWriteMask = attachment.write_mask;

            color_blend_attachment_states.push_back(state);
        }
    }

    // configure pipeline color blend state info.
    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info {};

    {
        auto& create_info = color_blend_state_create_info;
        auto& color_blend = desc.color_blend;

        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        // create_info.attachmentCount = color_blend_attachment_states.size();
        create_info.attachmentCount = 1;
        create_info.pAttachments = &color_blend_attachment_states[0];
        create_info.blendConstants[0] = color_blend.constant[0];
        create_info.blendConstants[1] = color_blend.constant[1];
        create_info.blendConstants[2] = color_blend.constant[2];
        create_info.blendConstants[3] = color_blend.constant[3];
    }

    vector<VkDynamicState> dynamic_states {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH,
        VK_DYNAMIC_STATE_DEPTH_BIAS
    };

    // configure dynamic state create info.
    VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};

    {
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = dynamic_states.size();
        dynamic_state_create_info.pDynamicStates = &dynamic_states[0];
    }

    // todo: it's temporary.
    VkPipelineLayoutCreateInfo layout_create_info {};

    layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    // create a empty pipeline layout.
    VkPipelineLayout layout;

    vkCreatePipelineLayout(device_->device(), &layout_create_info, nullptr, &layout);

    auto render_pass = device_->render_pass(to_render_pass_desc(desc.multisample, desc.output_merger));

    // configure a graphics pipeline create info.
    VkGraphicsPipelineCreateInfo create_info {};

    create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    create_info.stageCount = shader_stages.size();
    create_info.pStages = &shader_stages[0];
    create_info.pVertexInputState = &vertex_input_state;
    create_info.pInputAssemblyState = &input_assembly_state;
    create_info.pViewportState = &viewport_state_create_info;
    create_info.pRasterizationState= &rasterization_state;
    create_info.pMultisampleState = &multisample_state_create_info;
    create_info.pDepthStencilState = &depth_stencil_state;
    create_info.pColorBlendState = &color_blend_state_create_info;
    create_info.pDynamicState = &dynamic_state_create_info;
    create_info.layout = layout;
    create_info.renderPass = render_pass->render_pass();

    // try to create a graphics pipeline.
    if (vkCreateGraphicsPipelines(device_->device(), VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline_))
        throw runtime_error("fail to create a pipeline.");
}

//----------------------------------------------------------------------------------------------------------------------

}
