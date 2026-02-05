#include "ui/window.hpp"

#if _WIN32
#include "win32.hpp"
#endif

#include <memory>
#include <new>

namespace vulkron::ui {

    struct Window::Impl {
#if _WIN32
        Win32 _platform;
#endif

        Impl() {}
        ~Impl() {}
    };

    Window::Window()
    {
        static_assert(sizeof(_storage) >= sizeof(Impl), "Window storage too small for Impl");
        new (&_storage) Impl();
    }

    Window::~Window()
    {
        impl()->~Impl();
    }

    auto Window::impl() noexcept -> impl_pointer
    {
        return std::launder(reinterpret_cast<impl_pointer>(&_storage));
    }

    auto Window::impl() const noexcept -> const_impl_pointer
    {
        return std::launder(reinterpret_cast<const_impl_pointer>(&_storage));
    }

    auto Window::create_native_window(const Config& config) -> bool
    {
        if (config.width == 0 || config.height == 0) {
            return false;
        }

        return impl()->_platform.create_window(config.title, config.width, config.height);
    }

    auto Window::poll_events() -> void
    {
        impl()->_platform.poll_events();
    }

    auto Window::should_close() -> bool
    {
        return impl()->_platform.should_close_window();
    }

    auto Window::get_handle() -> native_handle
    {
        return impl()->_platform.get_window_handle();
    }

    auto Window::get_id() -> native_id
    {
        return impl()->_platform.get_window_id();
    }

} // namespace vulkron::ui
