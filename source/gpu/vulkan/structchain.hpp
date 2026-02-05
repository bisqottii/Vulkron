#pragma once

#include <concepts>
#include <iostream>
#include <vulkan/vulkan.h>

namespace vulkron::gpu::vulkan {

    template <typename T>
    concept VulkanStruct = requires(T t) {
        { t.sType } -> std::convertible_to<VkStructureType>;
        { t.pNext } -> std::convertible_to<void*>;
    };

    class PChainNext {
        VkBaseOutStructure* head = nullptr;

      public:
        constexpr PChainNext() noexcept = default;

        template <VulkanStruct T>
        constexpr PChainNext(T& first) noexcept 
        : head(reinterpret_cast<VkBaseOutStructure*>(&first))
        {
            first.pNext = nullptr; // ensure the chain terminates
        }

        [[nodiscard]]
        constexpr auto get() const noexcept -> void* {
            return head;
        }

        auto print() const -> void {
            if (!head) {
                std::cout << "<empty chain>\n";
                return;
            }

            const VkBaseOutStructure* current = head;
            while (current) {
                std::cout << "struct @ " << current << " sType = " << current->sType
                          << " pNext = " << current->pNext << "\n";
                current = current->pNext;
            }
        }

        template <VulkanStruct T>
        auto append(T& nextStruct) noexcept -> PChainNext& {
            nextStruct.pNext = nullptr;

            if (!head) {
                head = reinterpret_cast<VkBaseOutStructure*>(&nextStruct);
                return *this;
            }

            VkBaseOutStructure* last = head;
            while (last->pNext) {
                last = last->pNext;
            }

            last->pNext = reinterpret_cast<VkBaseOutStructure*>(&nextStruct);
            return *this;
        }
    };

    // Pipe operator for chaining
    template <VulkanStruct T>
    auto operator|(PChainNext chain, T& nextStruct) noexcept -> PChainNext {
        chain.append(nextStruct);
        return chain;
    }

} // namespace vulkron::gpu::vulkan