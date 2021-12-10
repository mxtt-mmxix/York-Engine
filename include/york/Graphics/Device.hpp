//
// Created by Matthew McCall on 12/3/21.
//

#ifndef YORK_GRAPHICS_DEVICE_HPP
#define YORK_GRAPHICS_DEVICE_HPP

#include <utility>

#include <vulkan/vulkan.hpp>

#include "Handle.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "RequestableItem.h"

namespace york::graphics {

using IndexQueuePair = std::pair<std::uint32_t, vk::Queue>;

using DeviceExtension = RequestableItem;

class Device : public Handle<vk::Device> {
public:
    explicit Device(Instance& instance);

    void requestExtension(const DeviceExtension& extension);
    [[nodiscard]] vk::PhysicalDevice getPhysicalDevice() const;

    ~Device() override;

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    Instance& m_instance;
    PhysicalDevice m_physicalDevice;
    std::vector<DeviceExtension> m_requestedExtensions;

    IndexQueuePair graphicsQueue;

};

}

#endif // YORK_GRAPHICS_DEVICE_HPP
