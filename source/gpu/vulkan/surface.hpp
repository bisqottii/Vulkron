#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

namespace vulkron::gpu::vulkan {

    class Surface {
      public:
        using native_handle = void*;
        using native_id = uintptr_t;

      private:
        VkSurfaceKHR _surface;

      public:
        enum class Platform { Win32 };

        struct Config {
            VkInstance    instance;
            Platform      platform;
            native_handle handle;
            native_id     id;
        };

      public:
        explicit Surface(const Config& config);
        Surface() = default;
        ~Surface() = default;

        Surface(Surface&& other) noexcept;
        auto operator=(Surface&& other) noexcept -> Surface&;

        Surface(const Surface& other) = delete;
        auto operator=(const Surface& other) -> Surface& = delete;

        auto destroy() -> void;
        auto handle() const -> VkSurfaceKHR;
    };

} // namespace vulkron::gpu::vulkan