#pragma once

#include <cstdint>
#include <string_view>

namespace vulkron::ui {

    class Window {
        struct Impl;

        using impl_pointer       = Impl*;
        using const_impl_pointer = const Impl*;

        alignas(std::max_align_t) std::byte _storage[256];

      public:
        using native_handle = void*;
        using native_id     = uintptr_t;

      public:
        struct Config {
            std::string_view title;
            uint32_t         width;
            uint32_t         height;
        };

      public:
        Window();
        ~Window();

        Window(const Window&)                    = delete;
        auto operator=(const Window&) -> Window& = delete;

        Window(Window&&)                    = delete;
        auto operator=(Window&&) -> Window& = delete;

        auto create_native_window(const Config& config) -> bool;
        auto poll_events() -> void;
        auto should_close() -> bool;

        auto get_handle() -> native_handle;
        auto get_id() -> native_id;

      private:
        [[nodiscard]] auto impl() noexcept -> impl_pointer;
        [[nodiscard]] auto impl() const noexcept -> const_impl_pointer;
    };


} // namespace vulkron::ui
