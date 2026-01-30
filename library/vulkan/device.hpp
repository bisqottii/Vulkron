#pragma once

#include "policy.hpp"
#include <vulkan/vulkan.h>

namespace vulkron::vulkan {

    enum class GpuUsage {
        Graphics,   // Rendering pipelines and swapchain
        Compute,    // General-purpose compute workloads
        Transfer    // Memory transfer / copy operations
    };
    

    class Device : public vulkron::NonCopyable {
        VkInstance _instance;
        VkDevice _device;
        VkPhysicalDevice _gpu;
        VkQueue _queue;
        uint32_t _queue_family;

    public:

    public:
        Device();
        ~Device();

        Device(Device&& other) noexcept;
        auto operator=(Device&&) noexcept -> Device&;

        auto instance_handle() const -> VkInstance;
        auto device_handle() const -> VkDevice;
        auto queue_family() const -> uint32_t;

        auto usage(GpuUsage usage) -> void;
    };

} // vulkron::vulkan