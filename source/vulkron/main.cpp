
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

int main() {
    // --- 1. Application info (optional but recommended) ---
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkron Minimal Example";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    // --- 2. Instance creation info ---
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Optional: extensions (for example, surface extensions for windowing)
    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME
        // Add platform-specific extensions if needed, e.g., VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Optional: validation layers
    std::vector<const char*> layers = {
        "VK_LAYER_KHRONOS_validation"
    };
    createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    createInfo.ppEnabledLayerNames = layers.data();

    // --- 3. Create Vulkan instance ---
    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance: " << result << std::endl;
        return -1;
    }

    std::cout << "Vulkan instance created successfully!" << std::endl;

    // --- 4. Cleanup ---
    vkDestroyInstance(instance, nullptr);
    std::cout << "Vulkan instance destroyed. Exiting." << std::endl;

    return 0;
}
