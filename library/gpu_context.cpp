#include "gpu_context.hpp"

#include <utility>
#include <memory>
#include <new>

namespace vulkron::gpu {

    struct Context::Impl {
        int a{0};
        int b{0};
    };








    Context::Context() {
        static_assert(sizeof(Impl) <= sizeof(_storage));
        std::construct_at(impl());
    }

    Context::~Context() {
        std::destroy_at(impl());
    }

    Context::Context(Context&& other) noexcept {
        std::construct_at(impl(), std::move(*other.impl()));
    }

    auto Context::operator=(Context&& other) noexcept -> Context& {
        *impl() = std::move(*other.impl());
        return *this;
    }

    auto Context::impl() noexcept -> impl_pointer {
        return std::launder(reinterpret_cast<impl_pointer>(_storage));
    }

    auto Context::impl() const noexcept -> const_impl_pointer {
        return std::launder(reinterpret_cast<const_impl_pointer>(_storage));
    }

} // namespace vulkron::gpu
