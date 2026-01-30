#pragma once

#include "policy.hpp"

#include <cstddef>
namespace vulkron::gpu {

    // enum class GpuUsage {
    //     Graphics,   // Rendering pipelines and swapchain
    //     Compute,    // General-purpose compute workloads
    //     Transfer    // Memory transfer / copy operations
    // };
    
    class Context : public vulkron::NonCopyable {
        struct Impl;

        using impl_pointer        = Impl*;
        using const_impl_pointer  = const Impl*;

        alignas(std::max_align_t) std::byte _storage[8];

    public:
        Context();
        ~Context();
        Context(Context&& other) noexcept;
        auto operator=(Context&& other) noexcept -> Context&;

    private:
        [[nodiscard]] auto impl() noexcept -> impl_pointer;
        [[nodiscard]] auto impl() const noexcept -> const_impl_pointer;
    };

} // namespace vulkron::gp 
