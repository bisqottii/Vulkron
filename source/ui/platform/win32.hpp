#pragma once

#include "platform.hpp"

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mutex>
#include <string>

namespace vulkron::ui {

    class Win32 : public Platform {
        friend class Platform;

        static constexpr WCHAR       CLASS_NAME[] = L"VulkronWin32";
        static inline std::once_flag _class_registered;

        HINSTANCE _instance;
        HWND      _window        = nullptr;
        HWND      _parent_window = nullptr; // for later use

        bool _should_close = false;

      public:
        Win32();
        ~Win32();

      private:
        auto create_window_impl(const std::string_view title, uint32_t width, uint32_t height)
            -> bool;
        auto destroy_impl() -> void;

        template <typename EventType>
        auto handle_event_impl(EventType&& event) -> bool;

        auto poll_events_impl() -> void;
        auto get_framebuffer_size_impl(uint32_t& width, uint32_t& height) -> void;
        auto get_window_handle_impl() -> native_handle;
        auto get_window_id_impl() -> native_id;
        auto should_close_window_impl() -> bool;

      private:
        static auto CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
            -> LRESULT;
        auto handle_window(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;
    };

} // namespace vulkron::ui