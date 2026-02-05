#pragma once

namespace vulkron::support {

    /**
     * @brief Base class to prevent moving of derived types
     *
     * Inherit privately to make a class non-movable but copyable:
     * @code
     * class MyClass : private NonMovable {
     *     // Can be copied, cannot be moved
     * };
     * @endcode
     */
    struct NonMovable {
        NonMovable() noexcept  = default;
        ~NonMovable() noexcept = default;

        // Allow copying
        NonMovable(const NonMovable&) noexcept                    = default;
        auto operator=(const NonMovable&) noexcept -> NonMovable& = default;

        // Delete moving
        NonMovable(NonMovable&&) noexcept                    = delete;
        auto operator=(NonMovable&&) noexcept -> NonMovable& = delete;
    };

} // namespace vulkron::support