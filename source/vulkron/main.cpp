
#include "device.hpp"

auto main(int argc, char const *argv[]) -> int
{
    vulkron::vulkan::Device device;
    device.usage(vulkron::vulkan::GpuUsage::Graphics);
    
    return 0;
}
