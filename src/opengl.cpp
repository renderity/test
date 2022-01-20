#include <cstddef> // size_t

#include "renderity/wrappers/src/renderer/renderer.h"
#include "renderity/wrappers/src/uniform/uniform.h"
#include "renderity/wrappers/src/uniform-block/uniform-block.h"
#include "renderity/wrappers/src/material/material.h"
#include "renderity/wrappers/src/object/object.h"
#include "renderity/wrappers/src/scene/scene.h"

#include "renderity/renderers/src/base/renderer.h"
#include "renderity/renderers/src/opengl/opengl.h"



#include <iostream>
using std::cout;
using std::endl;




extern RDTY::RENDERERS::Renderer* renderer_native;

extern size_t render_flag;



void initOpengl (const bool& offscreen = false)
{
	extern RDTY::WRAPPERS::Renderer* renderer;
	extern RDTY::WRAPPERS::Scene* scene;
	extern RDTY::WRAPPERS::Material* material;
	extern RDTY::WRAPPERS::Material* material2;
	extern RDTY::WRAPPERS::UniformBlock* uniform_block0;
	extern RDTY::WRAPPERS::Object* _object;
	extern RDTY::WRAPPERS::Object* object2;

	RDTY::OPENGL::RendererBase* _renderer {};

	if (offscreen)
	{
		_renderer = new RDTY::OPENGL::RendererOffscreen { renderer };
	}
	else
	{
		_renderer = new RDTY::OPENGL::Renderer { renderer };
	}

	renderer_native = _renderer;

	RDTY::OPENGL::Scene* _scene { RDTY_OPENGL_GET_INSTANCE(Scene, _renderer, scene) };
	RDTY::OPENGL::Material* _material { RDTY_OPENGL_GET_INSTANCE(Material, _renderer, material) };
	RDTY::OPENGL::Material* _material2 { RDTY_OPENGL_GET_INSTANCE(Material, _renderer, material2) };
	RDTY::OPENGL::UniformBlock* _uniform_block0 { RDTY_OPENGL_GET_INSTANCE(UniformBlock, _renderer, uniform_block0) };
	RDTY::OPENGL::Object* __object { RDTY_OPENGL_GET_INSTANCE(Object, _renderer, _object) };
	RDTY::OPENGL::Object* _object2 { RDTY_OPENGL_GET_INSTANCE(Object, _renderer, object2) };

	GLuint vertex_array {};
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glEnableVertexAttribArray(0);

	GLuint scene_vertex_buffer {};
	glGenBuffers(1, &scene_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, scene_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, _scene->wrapper->vertex_data.size() * 4, _scene->wrapper->vertex_data.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glClearColor(0.25f, 0.25f, 0.0f, 1.0f);

	while (render_flag)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_uniform_block0->use();

		_material->use();

		__object->draw();

		_material2->use();

		_object2->draw();

		_renderer->endLoop();
	}



	_renderer->destroy();

	delete _renderer;
}
