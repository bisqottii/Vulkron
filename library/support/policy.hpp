#pragma once

namespace vulkron {

    struct NonMovable {
        NonMovable() noexcept = default;
        NonMovable(const NonMovable&) noexcept = default;
        auto operator=(const NonMovable&) noexcept -> NonMovable& = default;

        NonMovable(NonMovable&&) = delete;
        auto operator=(NonMovable&&) -> NonMovable& = delete;
    };

    struct NonCopyable {
        NonCopyable() noexcept = default;
        NonCopyable(NonCopyable&&) noexcept = default;
        auto operator=(NonCopyable&&) noexcept -> NonCopyable& = default;

        NonCopyable(const NonCopyable&) = delete;
        auto operator=(const NonCopyable&) -> NonCopyable& = delete;
    };

} // vulkron