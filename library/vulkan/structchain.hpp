#include <vulkan/vulkan.h>
#include <concepts>
#include <iostream>

template<typename T>
concept VulkanStruct = requires(T t) {
    { t.sType } -> std::convertible_to<VkStructureType>;
    { t.pNext } -> std::convertible_to<void*>;
};

class PChainNext {
    VkBaseOutStructure* head = nullptr;

public:
    constexpr PChainNext() noexcept = default;

    template<VulkanStruct T>
    constexpr PChainNext(T& first) noexcept
        : head(reinterpret_cast<VkBaseOutStructure*>(&first)) 
    {
        first.pNext = nullptr; // ensure the chain terminates
    }

    constexpr void* get() const noexcept { 
        return head; 
    }

    auto print() const -> void {
        if (!head) { 
            std::cout << "<empty chain>\n"; return; 
        }

        const VkBaseOutStructure* current = head;
        while (current) {
            std::cout << "struct @ " << current
                      << " sType = " << current->sType
                      << " pNext = " << current->pNext
                      << "\n";
            current = current->pNext;
        }
    }

    template<VulkanStruct T>
    PChainNext& append(T& nextStruct) noexcept {
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
template<VulkanStruct T>
PChainNext operator|(PChainNext chain, T& nextStruct) noexcept {
    chain.append(nextStruct);
    return chain;
}


