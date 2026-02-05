#pragma once

#include <cstdint>
#include <functional>
#include <string_view>

namespace vulkron::ui {

    class Platform {
      public:
        // using EventCallback = std::function<void(const Event&)>;
        using native_handle = void*;
        using native_id = uintptr_t;

      protected:
        // EventCallback event_callback;

      public:
        Platform() = default;
        ~Platform() = default;

        Platform(const Platform&) = delete;
        Platform& operator=(const Platform&) = delete;

        Platform(Platform&&) = delete;
        Platform& operator=(Platform&&) = delete;

        template <typename Self>
        auto create_window(this Self&& self, const std::string_view title, uint32_t width, uint32_t height) -> bool {
            return self.create_window_impl(title, width, height);
        }

        template <typename Self>
        auto destroy(this Self&& self) -> void {
            self.destroy_impl();
        }

        template <typename Self, typename EventType>
        auto handle_event(this Self&& self, EventType&& event) -> bool {
            return self.handle_event_impl(std::forward<EventType>(event));
        }

        template <typename Self>
        auto poll_events(this Self&& self) -> void {
            self.poll_events_impl();
        }

        template <typename Self>
        auto get_framebuffer_size(this Self&& self, uint32_t& width, uint32_t& height) -> void {
            self.get_framebuffer_size_impl(width, height);
        }

        template <typename Self>
        auto get_window_handle(this Self&& self) -> native_handle {
            return self.get_window_handle_impl();
        }

        template <typename Self>
        auto get_window_id(this Self&& self) -> native_id {
            return self.get_window_id_impl();
        }

        template <typename Self>
        auto should_close_window(this Self&& self) -> bool {
            return self.should_close_window_impl();
        }
    };

} // namespace vulkron::ui