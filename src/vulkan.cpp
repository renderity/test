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




extern RDTY::RENDERERS::Renderer* renderer_native;

extern size_t render_flag;



// void initVulkan (const VkPhysicalDevice& physical_device = VK_NULL_HANDLE)
void initVulkan (const size_t& physical_device_index = 0)
{
	extern RDTY::WRAPPERS::Renderer* renderer;
	extern RDTY::WRAPPERS::Scene* scene;
	extern RDTY::WRAPPERS::Material* material;
	extern RDTY::WRAPPERS::Material* material2;
	extern RDTY::WRAPPERS::DescriptorSet* desc_set1;
	extern RDTY::WRAPPERS::DescriptorSet* desc_set2;
	extern RDTY::WRAPPERS::Object* _object;
	extern RDTY::WRAPPERS::Object* object2;



	// RDTY::VULKAN::Renderer* _renderer { new RDTY::VULKAN::Renderer { renderer, physical_device_index } };
	RDTY::VULKAN::RendererOffscreen* _renderer { new RDTY::VULKAN::RendererOffscreen { renderer, physical_device_index } };

	renderer_native = _renderer;

	{
		VkDescriptorPoolSize descr_pool_sizes [1]
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },
		};

		_renderer->descriptor_pool = _renderer->device.DescrPool(2, 1, descr_pool_sizes);
	}

	RDTY::VULKAN::Scene* _scene { RDTY_VULKAN_GET_INSTANCE(Scene, _renderer, scene) };
	RDTY::VULKAN::Material* _material { RDTY_VULKAN_GET_INSTANCE(Material, _renderer, material) };
	RDTY::VULKAN::Material* _material2 { RDTY_VULKAN_GET_INSTANCE(Material, _renderer, material2) };
	RDTY::VULKAN::DescriptorSet* _desc_set1 { RDTY_VULKAN_GET_INSTANCE(DescriptorSet, _renderer, desc_set1) };
	RDTY::VULKAN::DescriptorSet* _desc_set2 { RDTY_VULKAN_GET_INSTANCE(DescriptorSet, _renderer, desc_set2) };
	RDTY::VULKAN::Object* __object { RDTY_VULKAN_GET_INSTANCE(Object, _renderer, _object) };
	RDTY::VULKAN::Object* _object2 { RDTY_VULKAN_GET_INSTANCE(Object, _renderer, object2) };



	VkBuffer vertex_buffer = _renderer->device.Buffer(_scene->wrapper->vertex_data.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

	VkMemoryRequirements vertex_buffer_mem_reqs { _renderer->device.MemReqs(vertex_buffer) };

	uint64_t vertex_buffer_mem_index { _renderer->device.getMemTypeIndex(&vertex_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

	VkDeviceMemory vertex_buffer_mem { _renderer->device.Mem(vertex_buffer_mem_reqs.size, vertex_buffer_mem_index) };

	_renderer->device.bindMem(vertex_buffer, vertex_buffer_mem);

	void* vertex_buffer_mem_addr { _renderer->device.mapMem(vertex_buffer_mem, 0, _scene->wrapper->vertex_data.size() * sizeof(float), 0) };

	memcpy(vertex_buffer_mem_addr, _scene->wrapper->vertex_data.data(), _scene->wrapper->vertex_data.size() * sizeof(float));



	// while (render_flag)
	// {
	// 	// memcpy(uniform_buffer_mem_addr + 64, ((void*) &orbit) + 64, 64);

	// 	vkAcquireNextImageKHR(_renderer->device.handle, _renderer->swapchain, 0xFFFFFFFF, _renderer->image_available_semaphores[_renderer->curr_image], VK_NULL_HANDLE, &_renderer->image_indices[_renderer->curr_image]);

	// 	// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	// 	static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

	// 	static const VkDeviceSize vertex_buffer_offset {};

	// 	vkWaitForFences(_renderer->device.handle, 1, &_renderer->submission_completed_fences[_renderer->curr_image], VK_TRUE, 0xFFFFFFFF);

	// 	vkBeginCommandBuffer(_renderer->cmd_buffers[_renderer->curr_image], &command_buffer_bi);



	// 	_desc_set1->use(_material);

	// 	vkCmdBindVertexBuffers(_renderer->cmd_buffers[_renderer->curr_image], 0, 1, &vertex_buffer, &vertex_buffer_offset);
	// 	vkCmdBeginRenderPass(_renderer->cmd_buffers[_renderer->curr_image], &_renderer->render_pass_bi[_renderer->curr_image], VK_SUBPASS_CONTENTS_INLINE);

	// 	_material->use();

	// 	__object->draw();

	// 	_desc_set2->use(_material2);

	// 	_material2->use();

	// 	_object2->draw();



	// 	vkCmdEndRenderPass(_renderer->cmd_buffers[_renderer->curr_image]);
	// 	vkEndCommandBuffer(_renderer->cmd_buffers[_renderer->curr_image]);

	// 	vkResetFences(_renderer->device.handle, 1, &_renderer->submission_completed_fences[_renderer->curr_image]);

	// 	vkQueueSubmit(_renderer->graphics_queue, 1, &_renderer->submit_i[_renderer->curr_image], _renderer->submission_completed_fences[_renderer->curr_image]);

	// 	_renderer->present_i[_renderer->curr_image].pImageIndices = &_renderer->image_indices[_renderer->curr_image];

	// 	vkQueuePresentKHR(_renderer->present_queue, &_renderer->present_i[_renderer->curr_image]);



	// 	const static uint64_t max_swapchain_image_index { _renderer->swapchain_image_count - 1 };

	// 	if (++_renderer->curr_image > max_swapchain_image_index)
	// 	{
	// 		_renderer->curr_image = 0;
	// 	}
	// }

	while (render_flag)
	{
		static const VkCommandBufferBeginInfo command_buffer_bi { CmdBufferBeginI(nullptr, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

		static const VkDeviceSize vertex_buffer_offset {};

		vkBeginCommandBuffer(_renderer->cmd_buffers[0], &command_buffer_bi);



		_desc_set1->use(_material);

		vkCmdBindVertexBuffers(_renderer->cmd_buffers[0], 0, 1, &vertex_buffer, &vertex_buffer_offset);
		vkCmdBeginRenderPass(_renderer->cmd_buffers[0], &_renderer->render_pass_bi[0], VK_SUBPASS_CONTENTS_INLINE);

		_material->use();

		__object->draw();

		_desc_set2->use(_material2);

		_material2->use();

		_object2->draw();



		vkCmdEndRenderPass(_renderer->cmd_buffers[0]);

		VkImageSubresourceLayers image_subresource_layers
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0, 0, 1
		};

		VkBufferImageCopy buffer_image_copy
		{
			0, 800, 600,
			image_subresource_layers,
			{ 0, 0, 0 },
			{ 800, 600, 1 },
		};

		vkCmdCopyImageToBuffer
		(
			_renderer->cmd_buffers[0],
			_renderer->render_images[0],
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			_renderer->pixel_buffer,
			1, &buffer_image_copy
		);

		vkEndCommandBuffer(_renderer->cmd_buffers[0]);

		vkQueueSubmit(_renderer->graphics_queue, 1, &_renderer->submit_i[0], nullptr);

		vkDeviceWaitIdle(_renderer->device.handle);
	}



	_renderer->destroy();

	delete _renderer;
}
