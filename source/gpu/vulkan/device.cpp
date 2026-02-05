#include "device.hpp"
#include "structchain.hpp"

#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

namespace vulkron::gpu::vulkan {

    Device::Device() 
        : _instance(nullptr),
        _device(nullptr), 
        _gpu(nullptr)
    {
        VkApplicationInfo application_info = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName   = "Vulkron",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName        = "Vulkron",
            .engineVersion      = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion         = VK_API_VERSION_1_4
        };

        VkInstanceCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo        = &application_info,
            .enabledLayerCount       = 0,
            .ppEnabledLayerNames     = nullptr,
            .enabledExtensionCount   = 0,
            .ppEnabledExtensionNames = nullptr
        };

        if (vkCreateInstance(&create_info, nullptr, &_instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    Device::~Device()  {
        if (_device != nullptr) {
            vkDestroyDevice(_device, nullptr);
            _device = nullptr;
        }

        if (_instance != nullptr) {
            vkDestroyInstance(_instance, nullptr);
            _instance = nullptr;
        }
    }

    Device::Device(Device&& other) noexcept 
    : _instance(other._instance),
        _device(other._device),
        _gpu(other._gpu),
        _queue_create_infos(std::move(other._queue_create_infos))
    {
        other._instance = nullptr;
        other._device   = nullptr;
        other._gpu      = nullptr;
    }

    auto Device::operator=(Device&& other) noexcept -> Device& {
        if (this != &other) {
            if (_device != nullptr) {
                vkDestroyDevice(_device, nullptr);
                _device = nullptr;
            }

            if (_instance != nullptr) {
                vkDestroyInstance(_instance, nullptr);
                _instance = nullptr;
            }

            _queue_create_infos.clear();

            _instance = other._instance;
            _device = other._device;
            _gpu = other._gpu;
            _queue_create_infos = std::move(other._queue_create_infos);

            other._instance = nullptr;
            other._device   = nullptr;
            other._gpu      = nullptr;
        }
        return *this;
    }

    auto Device::instance_handle() const -> VkInstance {
        return _instance;
    }

    auto Device::device_handle() const -> VkDevice {
        return _device;
    }

    auto Device::physical_device_handle() const -> VkPhysicalDevice {
        return _gpu;
    }

    auto Device::select_gpu(GpuUsage usage) -> void {
        uint32_t gpu_count = 0;

        vkEnumeratePhysicalDevices(_instance, &gpu_count, nullptr);
        if (gpu_count == 0) {
            throw std::runtime_error("No Vulkan GPUs found");
        }

        std::vector<VkPhysicalDevice> gpu_list(gpu_count);
        vkEnumeratePhysicalDevices(_instance, &gpu_count, gpu_list.data());

        VkPhysicalDeviceVulkan11Properties vk11{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES};
        VkPhysicalDeviceVulkan12Properties vk12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES};
        VkPhysicalDeviceVulkan13Properties vk13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES};
        VkPhysicalDeviceVulkan14Properties vk14{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES};

        PChainNext chained_properties;
        switch (usage) {
            case GpuUsage::Graphics:
                chained_properties = vk14 | vk13 | vk12 | vk11;
                break;
            case GpuUsage::Compute:
                chained_properties = vk12 | vk11;
                break;
            case GpuUsage::Transfer:
                chained_properties = vk11;
                break;
        }

        // chained_properties.print();

        std::priority_queue<std::pair<size_t, VkPhysicalDevice>> max_heap;

        for (const VkPhysicalDevice& gpu : gpu_list) {
            VkPhysicalDeviceProperties2 device_properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
                .pNext = chained_properties.get()};

            vkGetPhysicalDeviceProperties2(gpu, &device_properties);
            std::cout << "Found GPU: " << device_properties.properties.deviceName << "\n";

            size_t gpu_score = 0;
            gpu_score += device_properties.properties.apiVersion;

            // Prefer discrete GPUs
            if (device_properties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                gpu_score += 1000;
            }

            // Prefer more memory heaps
            // VkPhysicalDeviceMemoryProperties2 mem_props;
            // vkGetPhysicalDeviceMemoryProperties2(gpu, &mem_props);
            // gpu_score += mem_props.memoryHeapCount;

            // VkPhysicalDeviceFeatures2 features;
            // vkGetPhysicalDeviceFeatures2(gpy, &features);

            max_heap.push({gpu_score, gpu});
        }

        if (max_heap.empty()) {
            throw std::runtime_error("No suitable gpu found");
        }

        auto [score, best_gpu] = max_heap.top();
        std::cout << "Best GPU score: " << score << "\n";
        _gpu = best_gpu;
    }

    auto Device::request_queue_family(const std::vector<float>& priorities, uint32_t queue_count, VkQueueFlags required_flags, VkQueueFlags excluded_flags) -> uint32_t {
        if (priorities.size() != queue_count) {
            throw std::runtime_error("Priorities vector size must match queue_count");
        }

        for (float priority : priorities) {
            if (priority < 0.0f || priority > 1.0f) {
                throw std::runtime_error("Queue priorities must be in range [0.0, 1.0]");
            }
        }

        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &queue_family_count, queue_families.data());

        uint32_t selected_family_index = UINT32_MAX;

        // Find the first suitable queue family
        for (uint32_t i = 0; i < queue_family_count; ++i) {
            const VkQueueFamilyProperties& family = queue_families[i];

            // Check if this family meets the flag requirements
            bool has_required_flags = (family.queueFlags & required_flags) == required_flags;
            bool has_excluded_flags = (family.queueFlags & excluded_flags) != 0;

            if (!has_required_flags || has_excluded_flags) {
                continue;
            }

            // Check if we have enough queues available
            if (queue_count <= family.queueCount) {
                selected_family_index = i;
                break;
            }
        }

        if (selected_family_index == UINT32_MAX) {
            throw std::runtime_error("No suitable queue family found");
        }

        // Create queue creation info - caller must keep vector alive!
        _queue_create_infos.emplace_back(VkDeviceQueueCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = selected_family_index,
            .queueCount = queue_count,
            .pQueuePriorities = priorities.data()
        });

        return selected_family_index;
    }

    auto Device::request_queue(uint32_t queue_family_index, uint32_t queue_index) -> VkQueue {
        VkQueue queue;
        vkGetDeviceQueue(_device, queue_family_index, queue_index, &queue);
        return queue;
    }

    auto Device::create_device() -> VkDevice {
        // This is pretty basic, but we should expand on this
        VkPhysicalDeviceFeatures2 features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
        PChainNext                chained_features = features;

        // chained_features.print();

        VkDeviceCreateInfo device_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = chained_features.get(),
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(_queue_create_infos.size()),
            .pQueueCreateInfos = _queue_create_infos.data(),
            .enabledLayerCount = 0,       // deprecated
            .ppEnabledLayerNames = nullptr, // deprecated
            .enabledExtensionCount = 0,       // no extensions yet
            .ppEnabledExtensionNames = nullptr,
            .pEnabledFeatures = nullptr // Using VkPhysicalDeviceFeatures2
        };

        if (vkCreateDevice(_gpu, &device_create_info, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        return _device;
    }

} // namespace vulkron::gpu::vulkan