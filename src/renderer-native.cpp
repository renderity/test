#include <cstddef> // size_t
#include <thread>

#include "renderity/math/src/quat/quat.h"
#include "renderity/math/src/mat4/mat4.h"
#include "renderity/renderers/src/base/renderer.h"
#include "renderity/wrappers/src/object/object.h"



extern "C" void constructStage1 (void);
extern "C" void updateObjectsData (void);
extern "C" void constructStage2 (void);
extern "C" void generateBoxes (RDTY::WRAPPERS::Object* object);
extern "C" void constructStage3 (void);

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
	constructStage1();

	updateObjectsData();

	constructStage2();

	extern RDTY::WRAPPERS::Object* _object;
	// extern RDTY::WRAPPERS::Object* object2;
	// extern RDTY::WRAPPERS::Object* object3;
	// extern RDTY::WRAPPERS::Object* object4;

	generateBoxes(_object);
	// generateBoxes(object2);
	// generateBoxes(object3);
	// generateBoxes(object4);

	// std::thread thr1 { generateBoxes, _object };
	// std::thread thr2 { generateBoxes, object2 };
	// std::thread thr3 { generateBoxes, object3 };
	// // std::thread thr4 { generateBoxes, object4 };

	// thr1.join();
	// thr2.join();
	// thr3.join();
	// // thr4.join();

	// constructStage3();

	// initOpengl(false);
	// initVulkan(false, 0);

	// runRenderingThread("opengl");

	// terminateRenderingThread();



	// runRenderingThread("vulkan");

	// terminateRenderingThread();




	// runRenderingThread("opengl");

	// terminateRenderingThread();



	// runRenderingThread("vulkan");

	// rendering_thread_handle->join();

	// // terminateRenderingThread();



	// RDTY::MATH::Mat4 m1
	// {
	// 	1.0f, 5.0f, 9.0f,  13.0f,
	// 	2.0f, 6.0f, 10.0f, 14.0f,
	// 	3.0f, 7.0f, 11.0f, 15.0f,
	// 	4.0f, 8.0f, 12.0f, 16.0f,
	// };

	// RDTY::MATH::Quat q { 1.0f, 5.0f, 9.0f,  13.0f };

	// RDTY::MATH::Mat4 m2 { &m1 };
	// RDTY::MATH::Mat4 m3 { &m1 };

	// // m2.makeRotationFromQuat32(&q);

	// // m2.print();


	// m3.makeRotationFromQuat128(&q);

	// m3.print();


	return 0;
}
