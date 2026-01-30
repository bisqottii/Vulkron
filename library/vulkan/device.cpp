#include "device.hpp"
#include "structchain.hpp"

#include <stdexcept>
#include <queue>
#include <vector>
#include <iostream>

namespace vulkron::vulkan {

    Device::Device()
    : _instance(nullptr),
        _device(nullptr),
        _gpu(nullptr),
        _queue(nullptr),
        _queue_family(0)
    {
        VkApplicationInfo application_info = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Vulkron",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "Vulkron",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = VK_API_VERSION_1_4
        };

        VkInstanceCreateInfo create_info = {
            create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            create_info.pNext = nullptr,
            create_info.flags = 0,
            create_info.pApplicationInfo = &application_info,
            create_info.enabledLayerCount = 0,
            create_info.ppEnabledLayerNames = nullptr,
            create_info.enabledExtensionCount = 0,
            create_info.ppEnabledExtensionNames = nullptr
        };

        if (vkCreateInstance(&create_info, nullptr, &_instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }


    Device::~Device() {
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
    : _instance(other._instance)
        , _device(other._device)
        , _gpu(other._gpu)
        , _queue(other._queue)
        , _queue_family(other._queue_family)
    {
        other._instance = nullptr;
        other._device = nullptr;
        other._gpu = nullptr;
        other._queue = nullptr;
        other._queue_family = 0;
    }

    auto Device::operator=(Device&& other) noexcept -> Device& {
        if (this != &other) {
            if (_device != nullptr) {
                vkDestroyDevice(_device, nullptr);
            }

            if (_instance != nullptr) {
                vkDestroyInstance(_instance, nullptr);
            }

            _instance = other._instance;
            _device = other._device;
            _gpu = other._gpu;
            _queue = other._queue;
            _queue_family = other._queue_family;

            other._instance = nullptr;
            other._device = nullptr;
            other._gpu = nullptr;
            other._queue = nullptr;
            other._queue_family = 0;
        }
        return *this;
    }

    auto Device::instance_handle() const -> VkInstance {
        return _instance;
    }

    auto Device::device_handle() const -> VkDevice {
        return _device;
    }

    auto Device::queue_family() const -> uint32_t {
        return _queue_family;
    }


    // TODO: Add in properties and filter the gpu's based on capabilities and usage
    auto Device::usage(GpuUsage usage) -> void {
        uint32_t gpu_count = 0;
        vkEnumeratePhysicalDevices(_instance, &gpu_count, nullptr);
        if (gpu_count == 0) {
            throw std::runtime_error("No Vulkan GPUs found");
        }

        std::vector<VkPhysicalDevice> gpu_list(gpu_count);
        vkEnumeratePhysicalDevices(_instance, &gpu_count, gpu_list.data());

        VkPhysicalDeviceVulkan11Properties vk11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES };
        VkPhysicalDeviceVulkan12Properties vk12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES };
        VkPhysicalDeviceVulkan13Properties vk13{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES };
        VkPhysicalDeviceVulkan14Properties vk14{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES };

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

        chained_properties.print();

        std::priority_queue<std::pair<size_t, VkPhysicalDevice>> max_heap;

        for (const VkPhysicalDevice& gpu : gpu_list) {
            VkPhysicalDeviceProperties2 device_properties = {
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
                .pNext = chained_properties.get()
            };

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

        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties2(_gpu, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties2> queue_families(queue_family_count);
        for (VkQueueFamilyProperties2& queue : queue_families) {
            queue.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
            queue.pNext = nullptr; // no extensions for now
        }

        vkGetPhysicalDeviceQueueFamilyProperties2(_gpu, &queue_family_count, queue_families.data());

        // Pick the first graphics queue family but we should expand on this
        for (uint32_t i = 0; i < queue_family_count; ++i) {
            if (queue_families[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                _queue_family = i;
                break;
            }
        }

        // This is pretty basic, but we should expand on this
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queue_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = _queue_family,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority
        };

        // This is pretty basic, but we should expand on this
        VkPhysicalDeviceFeatures2 features { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
        PChainNext chained_features = features;

        VkDeviceCreateInfo device_create_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = chained_features.get(),
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queue_create_info,
            .enabledLayerCount = 0,          // deprecated
            .ppEnabledLayerNames = nullptr,  // deprecated
            .enabledExtensionCount = 0,      // no extensions yet
            .ppEnabledExtensionNames = nullptr,
            .pEnabledFeatures = nullptr      // Using VkPhysicalDeviceFeatures2
        };

        if (vkCreateDevice(_gpu, &device_create_info, nullptr, &_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(_device, _queue_family, 0, &_queue);
    }

} // vulkron::vulkan