#pragma once

namespace vulkron::support {

    /**
     * @brief Base class to prevent both copying and moving
     *
     * Inherit privately to make a class completely non-copyable and non-movable.
     * Useful for singleton-like resources or objects with unique identity.
     *
     * @code
     * class Singleton : private Immovable {
     *     // Cannot be copied or moved
     * };
     * @endcode
     */
    struct Immovable {
        Immovable() noexcept  = default;
        ~Immovable() noexcept = default;

        Immovable(const Immovable&) noexcept                    = delete;
        auto operator=(const Immovable&) noexcept -> Immovable& = delete;

        Immovable(Immovable&&) noexcept                    = delete;
        auto operator=(Immovable&&) noexcept -> Immovable& = delete;
    };

} // namespace vulkron::support