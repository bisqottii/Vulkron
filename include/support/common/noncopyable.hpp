#pragma once

namespace vulkron::support {

    /**
     * @brief Base class to prevent copying of derived types
     *
     * Inherit privately to make a class non-copyable but movable:
     * @code
     * class MyClass : private NonCopyable {
     *     // Can be moved, cannot be copied
     * };
     * @endcode
     */
    struct NonCopyable {
        NonCopyable() noexcept  = default;
        ~NonCopyable() noexcept = default;

        NonCopyable(NonCopyable&&) noexcept                    = default;
        auto operator=(NonCopyable&&) noexcept -> NonCopyable& = default;

        NonCopyable(const NonCopyable&) noexcept                    = delete;
        auto operator=(const NonCopyable&) noexcept -> NonCopyable& = delete;
    };

} // namespace vulkron::support