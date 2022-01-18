#include <cstddef>
#include <thread>

#include "renderity/renderers/src/base/renderer.h"



extern "C" void constructRenderityWrappers (void);

extern void initOpengl (const bool& = false);
extern void initVulkan (const bool& = false, const size_t& = 0);



RDTY::RENDERERS::Renderer* renderer_native {};

size_t render_flag { 1 };



// std::thread* rendering_thread_handle {};

// void terminateRenderingThread (void)
// {
// 	if (rendering_thread_handle)
// 	{
// 		render_flag = 0;

// 		rendering_thread_handle->join();

// 		render_flag = 1;
// 	}

// 	delete rendering_thread_handle;

// 	rendering_thread_handle = nullptr;

// 	// renderer_native->pixel_data = nullptr;

// 	renderer_native = nullptr;
// }

// void runRenderingThread (const std::string& api, const size_t& vulkan_physical_device_index = 0)
// {
// 	// terminateRenderingThread();

// 	if (api == "opengl")
// 	{
// 		rendering_thread_handle = new std::thread { initOpengl, false };
// 	}
// 	else if (api == "vulkan")
// 	{
// 		rendering_thread_handle = new std::thread { initVulkan, false, vulkan_physical_device_index };
// 	}
// }



int main (void)
{
	constructRenderityWrappers();

	// initOpengl(false);
	initVulkan(false, 0);

	// runRenderingThread("opengl");

	// terminateRenderingThread();



	// runRenderingThread("vulkan");

	// terminateRenderingThread();




	// runRenderingThread("opengl");

	// terminateRenderingThread();



	// runRenderingThread("vulkan");

	// rendering_thread_handle->join();

	// // terminateRenderingThread();



	return 0;
}
