#include "wrappers/src/renderer/renderer.h"
#include "wrappers/src/uniform/uniform.h"
#include "wrappers/src/uniform-block/uniform-block.h"
#include "wrappers/src/material/material.h"
#include "wrappers/src/object/object.h"
#include "wrappers/src/scene/scene.h"

#include "renderers/src/base/renderer.h"
#include "renderers/src/opengl/opengl.h"



#include <iostream>
using std::cout;
using std::endl;



extern void constructRsWrappers (void);



// Declare global to access from electron.
extern RDTY::RENDERERS::Renderer* renderer_native;
RDTY::OPENGL::RendererOffscreen* _renderer {};



void initOpengl (void)
{
	constructRsWrappers();

	extern RDTY::WRAPPERS::Renderer* renderer;
	extern RDTY::WRAPPERS::Scene* scene;
	extern RDTY::WRAPPERS::Material* material;
	extern RDTY::WRAPPERS::Material* material2;
	extern RDTY::WRAPPERS::UniformBlock* uniform_block0;
	extern RDTY::WRAPPERS::Object* _object;
	extern RDTY::WRAPPERS::Object* object2;

	// RDTY::OPENGL::Renderer* _renderer { new RDTY::OPENGL::Renderer { renderer } };
	_renderer = new RDTY::OPENGL::RendererOffscreen { renderer };

	renderer_native = _renderer;

	RDTY::OPENGL::Scene* _scene { RS_OPENGL_GET_INSTANCE(Scene, _renderer, scene) };
	RDTY::OPENGL::Material* _material { RS_OPENGL_GET_INSTANCE(Material, _renderer, material) };
	RDTY::OPENGL::Material* _material2 { RS_OPENGL_GET_INSTANCE(Material, _renderer, material2) };
	RDTY::OPENGL::UniformBlock* _uniform_block0 { RS_OPENGL_GET_INSTANCE(UniformBlock, _renderer, uniform_block0) };
	RDTY::OPENGL::Object* __object { RS_OPENGL_GET_INSTANCE(Object, _renderer, _object) };
	RDTY::OPENGL::Object* _object2 { RS_OPENGL_GET_INSTANCE(Object, _renderer, object2) };

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

	while (1)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_uniform_block0->use();

		_material->use();

		__object->draw();

		_material2->use();

		_object2->draw();

		_renderer->endLoop();
	}
}

// int main (void)
// {
// 	initOpengl();



// 	return 0;
// }
