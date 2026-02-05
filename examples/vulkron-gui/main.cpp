#include "ui/window.hpp"

auto main(int argc, char const *argv[]) -> int {
    vulkron::ui::Window window;

    vulkron::ui::Window::Config config = {
        .title = "Vulkron Win32",
        .width = 1080,
        .height = 720
    };

    if (!window.create_native_window(config)) {
        return 1;
    }

    while (!window.should_close()) {
        window.poll_events();
    }

    return 0;
}