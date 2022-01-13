#include <cstring>

#include "renderity/wrappers/src/renderer/renderer.h"
#include "renderity/wrappers/src/uniform/uniform.h"
#include "renderity/wrappers/src/uniform-block/uniform-block.h"
#include "renderity/wrappers/src/descriptor-set/descriptor-set.h"
#include "renderity/wrappers/src/material/material.h"
#include "renderity/wrappers/src/object/object.h"
#include "renderity/wrappers/src/scene/scene.h"

#include "renderity/renderers/src/base/renderer.h"
#include "renderity/renderers/src/vulkan/vulkan.h"



#include <iostream>
using std::cout;
using std::endl;



extern void constructRenderityWrappers (void);



// Declare global to access from electron.
extern RDTY::RENDERERS::Renderer* renderer_native;
// RDTY::VULKAN::RendererOffscreen* renderer_vulkan {};



VkBuffer vertex_buffer {};



void initVulkan (void)
{
	constructRenderityWrappers();

	extern RDTY::WRAPPERS::Renderer* renderer;
	extern RDTY::WRAPPERS::Scene* scene;
	extern RDTY::WRAPPERS::Material* material;
	extern RDTY::WRAPPERS::Material* material2;
	extern RDTY::WRAPPERS::DescriptorSet* desc_set1;
	extern RDTY::WRAPPERS::DescriptorSet* desc_set2;
	extern RDTY::WRAPPERS::Object* _object;
	extern RDTY::WRAPPERS::Object* object2;



	RDTY::VULKAN::Renderer* renderer_vulkan = new RDTY::VULKAN::Renderer { renderer };
	// renderer_vulkan = new RDTY::VULKAN::RendererOffscreen { renderer };

	renderer_native = renderer_vulkan;

	{
		VkDescriptorPoolSize descr_pool_sizes [1]
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },
		};

		renderer_vulkan->descriptor_pool = renderer_vulkan->device.DescrPool(2, 1, descr_pool_sizes);
	}

	RDTY::VULKAN::Scene* _scene { RS_VULKAN_GET_INSTANCE(Scene, renderer_vulkan, scene) };
	RDTY::VULKAN::Material* _material { RS_VULKAN_GET_INSTANCE(Material, renderer_vulkan, material) };
	RDTY::VULKAN::Material* _material2 { RS_VULKAN_GET_INSTANCE(Material, renderer_vulkan, material2) };
	RDTY::VULKAN::DescriptorSet* _desc_set1 { RS_VULKAN_GET_INSTANCE(DescriptorSet, renderer_vulkan, desc_set1) };
	RDTY::VULKAN::DescriptorSet* _desc_set2 { RS_VULKAN_GET_INSTANCE(DescriptorSet, renderer_vulkan, desc_set2) };
	RDTY::VULKAN::Object* __object { RS_VULKAN_GET_INSTANCE(Object, renderer_vulkan, _object) };
	RDTY::VULKAN::Object* _object2 { RS_VULKAN_GET_INSTANCE(Object, renderer_vulkan, object2) };



	vertex_buffer = renderer_vulkan->device.Buffer(_scene->wrapper->vertex_data.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

	VkMemoryRequirements vertex_buffer_mem_reqs { renderer_vulkan->device.MemReqs(vertex_buffer) };

	uint64_t vertex_buffer_mem_index { renderer_vulkan->device.getMemTypeIndex(&vertex_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

	VkDeviceMemory vertex_buffer_mem { renderer_vulkan->device.Mem(vertex_buffer_mem_reqs.size, vertex_buffer_mem_index) };

	renderer_vulkan->device.bindMem(vertex_buffer, vertex_buffer_mem);

	void* vertex_buffer_mem_addr { renderer_vulkan->device.mapMem(vertex_buffer_mem, 0, _scene->wrapper->vertex_data.size() * sizeof(float), 0) };

	memcpy(vertex_buffer_mem_addr, _scene->wrapper->vertex_data.data(), _scene->wrapper->vertex_data.size() * sizeof(float));



	while (1)
	{
		// memcpy(uniform_buffer_mem_addr + 64, ((void*) &orbit) + 64, 64);

		vkAcquireNextImageKHR(renderer_vulkan->device.handle, renderer_vulkan->swapchain, 0xFFFFFFFF, renderer_vulkan->image_available_semaphores[renderer_vulkan->curr_image], VK_NULL_HANDLE, &renderer_vulkan->image_indices[renderer_vulkan->curr_image]);

		// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

		static const VkDeviceSize vertex_buffer_offset {};

		vkWaitForFences(renderer_vulkan->device.handle, 1, &renderer_vulkan->submission_completed_fences[renderer_vulkan->curr_image], VK_TRUE, 0xFFFFFFFF);

		vkBeginCommandBuffer(renderer_vulkan->cmd_buffers[renderer_vulkan->curr_image], &command_buffer_bi);



		_desc_set1->use(_material);

		vkCmdBindVertexBuffers(renderer_vulkan->cmd_buffers[renderer_vulkan->curr_image], 0, 1, &vertex_buffer, &vertex_buffer_offset);
		vkCmdBeginRenderPass(renderer_vulkan->cmd_buffers[renderer_vulkan->curr_image], &renderer_vulkan->render_pass_bi[renderer_vulkan->curr_image], VK_SUBPASS_CONTENTS_INLINE);

		_material->use();

		__object->draw();

		_desc_set2->use(_material2);

		_material2->use();

		_object2->draw();



		vkCmdEndRenderPass(renderer_vulkan->cmd_buffers[renderer_vulkan->curr_image]);
		vkEndCommandBuffer(renderer_vulkan->cmd_buffers[renderer_vulkan->curr_image]);

		vkResetFences(renderer_vulkan->device.handle, 1, &renderer_vulkan->submission_completed_fences[renderer_vulkan->curr_image]);

		vkQueueSubmit(renderer_vulkan->graphics_queue, 1, &renderer_vulkan->submit_i[renderer_vulkan->curr_image], renderer_vulkan->submission_completed_fences[renderer_vulkan->curr_image]);

		renderer_vulkan->present_i[renderer_vulkan->curr_image].pImageIndices = &renderer_vulkan->image_indices[renderer_vulkan->curr_image];

		vkQueuePresentKHR(renderer_vulkan->present_queue, &renderer_vulkan->present_i[renderer_vulkan->curr_image]);



		const static uint64_t max_swapchain_image_index { renderer_vulkan->swapchain_image_count - 1 };

		if (++renderer_vulkan->curr_image > max_swapchain_image_index)
		{
			renderer_vulkan->curr_image = 0;
		}
	}

	// while (1)
	// {
	// 	static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

	// 	static const VkDeviceSize vertex_buffer_offset {};

	// 	vkBeginCommandBuffer(renderer_vulkan->cmd_buffers[0], &command_buffer_bi);



	// 	_desc_set1->use(_material);

	// 	vkCmdBindVertexBuffers(renderer_vulkan->cmd_buffers[0], 0, 1, &vertex_buffer, &vertex_buffer_offset);
	// 	vkCmdBeginRenderPass(renderer_vulkan->cmd_buffers[0], &renderer_vulkan->render_pass_bi[0], VK_SUBPASS_CONTENTS_INLINE);

	// 	_material->use();

	// 	__object->draw();

	// 	_desc_set2->use(_material2);

	// 	_material2->use();

	// 	_object2->draw();



	// 	vkCmdEndRenderPass(renderer_vulkan->cmd_buffers[0]);

	// 	VkImageSubresourceLayers image_subresource_layers
	// 	{
	// 		VK_IMAGE_ASPECT_COLOR_BIT,
	// 		0, 0, 1
	// 	};

	// 	VkBufferImageCopy buffer_image_copy
	// 	{
	// 		0, 800, 600,
	// 		image_subresource_layers,
	// 		{ 0, 0, 0 },
	// 		{ 800, 600, 1 },
	// 	};

	// 	vkCmdCopyImageToBuffer
	// 	(
	// 		renderer_vulkan->cmd_buffers[0],
	// 		renderer_vulkan->render_images[0],
	// 		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
	// 		renderer_vulkan->pixel_buffer,
	// 		1, &buffer_image_copy
	// 	);

	// 	vkEndCommandBuffer(renderer_vulkan->cmd_buffers[0]);

	// 	vkQueueSubmit(renderer_vulkan->graphics_queue, 1, &renderer_vulkan->submit_i[0], nullptr);

	// 	vkDeviceWaitIdle(renderer_vulkan->device.handle);
	// }



	renderer_vulkan->destroy();
}

int main (void)
{
	initVulkan();



	return 0;
}
