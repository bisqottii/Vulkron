#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace vulkron::gpu::vulkan {

  class Device {
    VkInstance       _instance;
    VkDevice         _device;
    VkPhysicalDevice _gpu;

    std::vector<VkDeviceQueueCreateInfo> _queue_create_infos;

  public:
    enum class GpuUsage {
        Graphics, // Rendering pipelines and swapchain
        Compute,  // General-purpose compute workloads
        Transfer  // Memory transfer / copy operations
    };

  public:
    Device();
    ~Device();

    Device(const Device&) = delete;
    auto operator=(const Device&) -> Device& = delete;

    Device(Device&& other) noexcept;
    auto operator=(Device&& other) noexcept -> Device&;

    [[nodiscard]] auto instance_handle() const -> VkInstance;
    [[nodiscard]] auto device_handle() const -> VkDevice;
    [[nodiscard]] auto physical_device_handle() const -> VkPhysicalDevice;

    auto select_gpu(GpuUsage usage) -> void;

    [[nodiscard]] auto request_queue_family(const std::vector<float>& priorities, uint32_t queue_count, VkQueueFlags required_flags, VkQueueFlags excluded_flags) -> uint32_t;
    [[nodiscard]] auto request_queue(uint32_t queue_family_index, uint32_t queue_index) -> VkQueue;
    auto create_device() -> VkDevice;
  };

} // namespace vulkron::gpu::vulkan