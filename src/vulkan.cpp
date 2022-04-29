#include <cstddef> // size_t
// #include <cstring>

#include "renderity/wrappers/src/renderer/renderer.h"
// #include "renderity/wrappers/src/uniform/uniform.h"
// #include "renderity/wrappers/src/uniform-block/uniform-block.h"
#include "renderity/wrappers/src/descriptor-set/descriptor-set.h"
#include "renderity/wrappers/src/material/material.h"
#include "renderity/wrappers/src/object/object.h"
// #include "renderity/wrappers/src/scene/scene.h"

#include "renderity/renderers/src/base/renderer.h"
#include "renderity/renderers/src/vulkan/vulkan.h"



#include <iostream>
using std::cout;
using std::endl;




extern RDTY::RENDERERS::Renderer* renderer_native;

extern size_t render_flag;



// void initVulkan (const VkPhysicalDevice& physical_device = VK_NULL_HANDLE)
void initVulkan (const bool& offscreen = false, const size_t& physical_device_index = 0)
{
	extern RDTY::WRAPPERS::Renderer* renderer;
	extern RDTY::WRAPPERS::DescriptorSet* descriptor_set_scene;
	extern RDTY::WRAPPERS::Material* surface_material;
	extern RDTY::WRAPPERS::Object* surface_object;

	RDTY::VULKAN::RendererBase* _renderer {};

	if (offscreen)
	{
		_renderer = new RDTY::VULKAN::RendererOffscreen { renderer, physical_device_index };
	}
	else
	{
		_renderer = new RDTY::VULKAN::Renderer { renderer, physical_device_index };
	}

	renderer_native = _renderer;

	{
		VkDescriptorPoolSize descr_pool_sizes [2]
		{
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 5 },
		};

		_renderer->descriptor_pool = _renderer->device.DescrPool(1, 1, descr_pool_sizes);
	}

	RDTY::VULKAN::DescriptorSet* _descriptor_set_scene { RDTY_VULKAN_GET_INSTANCE(DescriptorSet, _renderer, descriptor_set_scene) };
	RDTY::VULKAN::Material* _surface_material { RDTY_VULKAN_GET_INSTANCE(Material, _renderer, surface_material) };
	RDTY::VULKAN::Object* _surface_object { RDTY_VULKAN_GET_INSTANCE(Object, _renderer, surface_object) };



	_surface_object->createBuffers();



	// VkBuffer vertex_buffer = _renderer->device.Buffer(_scene->wrapper->position_data.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

	// VkMemoryRequirements vertex_buffer_mem_reqs { _renderer->device.MemReqs(vertex_buffer) };

	// uint64_t vertex_buffer_mem_index { _renderer->device.getMemTypeIndex(&vertex_buffer_mem_reqs, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) };

	// VkDeviceMemory vertex_buffer_mem { _renderer->device.Mem(vertex_buffer_mem_reqs.size, vertex_buffer_mem_index) };

	// _renderer->device.bindMem(vertex_buffer, vertex_buffer_mem);

	// void* vertex_buffer_mem_addr { _renderer->device.mapMem(vertex_buffer_mem, 0, _scene->wrapper->position_data.size() * sizeof(float), 0) };

	// memcpy(vertex_buffer_mem_addr, _scene->wrapper->position_data.data(), _scene->wrapper->position_data.size() * sizeof(float));

	const VkDeviceSize vertex_buffer_offset {};



	while (render_flag)
	{
		_renderer->beginLoop();

		// vkCmdBindVertexBuffers(_renderer->cmd_buffers[_renderer->curr_image], 0, 1, &vertex_buffer, &vertex_buffer_offset);
		vkCmdBindVertexBuffers(_renderer->cmd_buffers[_renderer->curr_image], 0, 1, &_surface_object->position_buffer, &vertex_buffer_offset);

		vkCmdBeginRenderPass(_renderer->cmd_buffers[_renderer->curr_image], &_renderer->render_pass_bi[_renderer->curr_image], VK_SUBPASS_CONTENTS_INLINE);

		_descriptor_set_scene->use(_surface_material);

		_surface_material->use();

		_surface_object->draw2();

		_renderer->endLoop();
	}



	_renderer->destroy();

	delete _renderer;
}
