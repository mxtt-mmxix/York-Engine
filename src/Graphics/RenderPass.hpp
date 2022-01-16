//
// Created by Matthew McCall on 1/6/22.
//

#ifndef YORK_GRAPHICS_RENDERPASS_HPP
#define YORK_GRAPHICS_RENDERPASS_HPP

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "SwapChain.hpp"

namespace york::graphics {

class RenderPass : public Handle<vk::RenderPass> {
public:
    explicit RenderPass(SwapChain& swapChain);
    Device& getDevice();
    SwapChain& getSwapChain();

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    SwapChain& m_swapChain;
    Device& m_device;
};

}


#endif // YORK_GRAPHICS_RENDERPASS_HPP