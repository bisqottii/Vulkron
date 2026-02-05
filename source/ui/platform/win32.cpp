#include "win32.hpp"

namespace vulkron::ui {

    Win32::Win32() 
    : _instance(GetModuleHandle(nullptr))
    {
        std::call_once(_class_registered, []() {
            WNDCLASSW wc = {};
            wc.lpfnWndProc = window_proc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = nullptr;
            wc.lpszClassName = CLASS_NAME;
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            RegisterClassW(&wc);
        });
    }

    // Constructor for child windows
    // Win32::Win32(HWND parent_window)
    // : Win32()
    // {
    //     _parent_window = parent_window;
    // }

    Win32::~Win32() {
        destroy_impl();
    }

    auto Win32::create_window_impl(const std::string_view title, uint32_t width, uint32_t height) -> bool {
        // Convert UTF-8 title to UTF-16
        int          title_len = MultiByteToWideChar(CP_UTF8, 0, title.data(), -1, nullptr, 0);
        std::wstring wtitle(title_len, 0);
        MultiByteToWideChar(CP_UTF8, 0, title.data(), -1, wtitle.data(), title_len);

        // Determine window style based on whether we have a parent
        DWORD style = _parent_window ? WS_CHILD | WS_VISIBLE : WS_OVERLAPPEDWINDOW;

        // Create window
        _window = CreateWindowExW(0,
                                  CLASS_NAME,
                                  wtitle.c_str(),
                                  style,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  static_cast<int>(width),
                                  static_cast<int>(height),
                                  _parent_window,
                                  nullptr,
                                  _instance,
                                  this);

        if (!_window)
            return false;

        if (!_parent_window) {
            ShowWindow(_window, SW_SHOW);
            UpdateWindow(_window);
        }

        return true;
    }

    auto Win32::destroy_impl() -> void
    {
        if (_window && !_parent_window) {
            DestroyWindow(_window);
        }
        _window       = nullptr;
        _should_close = true;
    }

    auto Win32::poll_events_impl() -> void
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                _should_close = true;
            }
        }
    }

    auto Win32::get_framebuffer_size_impl(uint32_t& width, uint32_t& height) -> void
    {
        if (!_window) {
            width  = 0;
            height = 0;
            return;
        }

        RECT rect;
        if (GetClientRect(_window, &rect)) {
            width  = static_cast<uint32_t>(rect.right - rect.left);
            height = static_cast<uint32_t>(rect.bottom - rect.top);
        }
        else {
            width  = 0;
            height = 0;
        }
    }

    auto Win32::get_window_handle_impl() -> native_handle
    {
        return reinterpret_cast<void*>(_window);
    }

    auto Win32::get_window_id_impl() -> native_id
    {
        return 0; // For Linux only
    }

    auto Win32::should_close_window_impl() -> bool
    {
        return _should_close;
    }


    auto CALLBACK Win32::window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        Win32* window = nullptr;

        // Get the window instance from user data
        if (msg == WM_NCCREATE) {
            // During window creation, the instance is passed in lpCreateParams
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lparam);
            window           = static_cast<Win32*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

            // Store the HWND in the instance
            window->_window = hwnd;
        }
        else {
            // For subsequent messages, retrieve from user data
            window = reinterpret_cast<Win32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        // If we have an instance, call its handler
        if (window) {
            return window->handle_window(hwnd, msg, wparam, lparam);
        }

        // For messages before we have the instance (or if it's null)
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    // Static private methods
    auto Win32::handle_window(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        switch (msg) {
            case WM_DESTROY: {
                if (!_parent_window) {
                    PostQuitMessage(0);
                }
                _should_close = true;
                return 0;
            }

            case WM_SIZE: {
                // Handle window resizing for Vulkan
                uint32_t new_width  = LOWORD(lparam);
                uint32_t new_height = HIWORD(lparam);

                // Only handle if window is not minimized
                if (wparam != SIZE_MINIMIZED) {
                    // Here you would typically:
                    // 1. Signal that swapchain needs recreation
                    // 2. Set a flag or call a callback

                    // Example: If you had a resize callback:
                    // if (_resize_callback) {
                    //     _resize_callback(new_width, new_height);
                    // }

                    // Or set a flag:
                    // _window_resized = true;
                    // _new_width = new_width;
                    // _new_height = new_height;
                }
                return 0;
            }

            case WM_EXITSIZEMOVE: {
                // This message is sent when user stops dragging/resizing window
                // Good place to trigger swapchain recreation after resizing is complete
                // _swapchain_needs_recreation = true;
                break;
            }

            case WM_CLOSE: {
                if (!_parent_window) {
                    DestroyWindow(_window);
                }
                return 0;
            }

            case WM_ERASEBKGND:
                return 1; // Don't erase background for Vulkan

            case WM_PAINT: {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);
                EndPaint(hwnd, &ps);
                return 0;
            }

            case WM_GETMINMAXINFO: {
                MINMAXINFO* min_max_info       = reinterpret_cast<MINMAXINFO*>(lparam);
                min_max_info->ptMinTrackSize.x = 320; // Minimum width
                min_max_info->ptMinTrackSize.y = 240; // Minimum height
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg, wparam, lparam);
    }


} // namespace vulkron::ui