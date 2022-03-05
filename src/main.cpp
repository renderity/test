#include <cstddef> // size_t
#include <cstring>
#include <vector>
#include <string>

#include "renderity/math/src/mat4/mat4.h"
#include "renderity/math/src/orbit/orbit.h"
#include "renderity/math/src/util/util.h"

#include "renderity/aux/src/transition-stack/transition-stack.h"
#include "renderity/aux/src/transition/transition.h"

#include "renderity/wrappers/src/renderer/renderer.h"
#include "renderity/wrappers/src/uniform/uniform.h"
#include "renderity/wrappers/src/uniform-block/uniform-block.h"
#include "renderity/wrappers/src/storage-block/storage-block.h"
#include "renderity/wrappers/src/descriptor-set/descriptor-set.h"
#include "renderity/wrappers/src/material/material.h"
#include "renderity/wrappers/src/object/object.h"
#include "renderity/wrappers/src/scene/scene.h"



#ifdef __wasm__
	extern "C" void console_log (size_t);
	#define LOG(x) console_log((size_t) x);

	extern "C" void console_log_f (float);
	#define LOGF(x) console_log_f((float) x);

	// extern "C" size_t getTime (void);
#else
	#include <iostream>

	#define LOG(x) std::cout << x << std::endl;
	#define LOGF(x) std::cout << x << std::endl;
#endif



RDTY::MATH::Orbit* orbit {};
RDTY::MATH::Orbit* orbit2 {};

RDTY::WRAPPERS::Renderer* renderer {};
RDTY::WRAPPERS::Scene* scene {};
RDTY::WRAPPERS::Material* material {};
RDTY::WRAPPERS::Material* material2 {};
RDTY::WRAPPERS::UniformBlock* uniform_block0 {};
RDTY::WRAPPERS::UniformBlock* uniform_block1 {};
RDTY::WRAPPERS::Object* _object {};
RDTY::WRAPPERS::Object* object2 {};
RDTY::WRAPPERS::DescriptorSet* desc_set1 {};
RDTY::WRAPPERS::DescriptorSet* desc_set2 {};

RDTY::WRAPPERS::UniformBlock* surface_uniform_block_camera {};
RDTY::WRAPPERS::StorageBlock* storage_block {};

RDTY::WRAPPERS::Material* surface_material {};
RDTY::WRAPPERS::Object* surface_object {};

float curve_values [5000];
RDTY::Transition orbit_transition;
RDTY::Transition orbit_transition2;

extern "C" void ___test (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[time_gone - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	orbit->rotation_speed_x = orbit->rotation_speed_y = temp * (time_gone - prev_time) * 0.01;

	prev_time = time_gone;

	orbit->rotate();
	orbit->update();
}

extern "C" void ___test2 (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[time_gone - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	orbit->translation_speed_y = temp * (time_gone - prev_time) * 0.001;

	prev_time = time_gone;

	orbit->transY();
	orbit->update();
}

RDTY::TransitionStack* _stack0 {};
RDTY::TransitionStack* _stack1 {};

extern "C" void initTransitionStack (void)
{
	RDTY::MATH::UTIL::makeBezierCurve3Sequence
	(
		curve_values,

		1.0f,
		1.0f,
		1.0f,
		0.0f,
		0.5f,
		0.0f,
		0.0f,
		0.0f,

		5000
	);

	_stack0 = new RDTY::TransitionStack { 64 };
	_stack1 = new RDTY::TransitionStack { 64 };
}

extern "C" void updateTransitions0 (void)
{
	_stack0->calculateFrametime();
	_stack0->update();
}

extern "C" void updateTransitions1 (void)
{
	_stack1->calculateFrametime();
	_stack1->update();
}

extern "C" void logStacks (void)
{
	LOG(_stack0->length)
	LOG(_stack1->length)
}

extern "C" void startTransition (void)
{
	// auto ___test = [&] (const size_t& time_gone)
	// {
	// 	static size_t prev_time {};

	// 	float temp { curve_values[time_gone - 1] };

	// 	if (time_gone < prev_time)
	// 	{
	// 		prev_time = 0;
	// 	}

	// 	orbit->rotation_speed_x = orbit->rotation_speed_y = temp * (time_gone - prev_time) * 0.01;

	// 	prev_time = time_gone;

	// 	orbit->rotate();
	// 	orbit->update();
	// };

	orbit_transition.start2(5000, ___test);
}

extern "C" void startTransition2 (void)
{
	orbit_transition2.start2(5000, ___test2);
}

extern "C" void constructRenderityWrappers (void)
{
	renderer = new RDTY::WRAPPERS::Renderer { .width = 800, .height = 600 };

	scene = new RDTY::WRAPPERS::Scene;

	material = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.wgsl_code_vertex =
			R"(
				struct VertexIn
				{
					[[location(0)]] pos : vec3<f32>;
					// [[builtin(vertex_index)]] vi : u32;
				};

				struct VertexOut
				{
					[[builtin(position)]] pos : vec4<f32>;
				};

				struct Camera
				{
					projection_matrix : mat4x4<f32>;
					view_matrix : mat4x4<f32>;
				};

				[[group(0), binding(0)]] var<uniform> camera : Camera;

				[[stage(vertex)]] fn main(input : VertexIn) -> VertexOut
				{
					var output : VertexOut;

					output.pos = camera.projection_matrix * camera.view_matrix * vec4<f32>(input.pos, 1.0);

					return output;
				}
			)",
	};

	material2 = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.blend_enabled = RDTY::WRAPPERS::MATERIAL::BlendEnabled::FALSE,

		.glsl100es_code_fragment =
			R"(
				precision highp int;
				precision highp float;

				void main (void)
				{
					gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
				}
			)",

		.glsl300es_code_fragment =
			R"(
				#version 300 es

				precision highp int;
				precision highp float;

				layout (location = 0) out vec4 fragment_color;

				void main (void)
				{
					fragment_color = vec4(0.0, 1.0, 0.0, 1.0);
				}
			)",

		.glsl_vulkan_code_vertex =
			R"(
				#version 460

				#extension GL_ARB_separate_shader_objects : enable

				precision highp int;
				precision highp float;

				layout (location = 0) in vec3 in_position;

				out gl_PerVertex
				{
					vec4 gl_Position;
				};

				layout (set = 0, binding = 0) uniform Camera
				{
					mat4 projection_matrix;
					mat4 view_matrix;
				} camera;

				layout (set = 0, binding = 1) uniform Camera2
				{
					mat4 view_matrix;
				} camera2;

				void main (void)
				{
					// gl_Position = camera.projection_matrix * camera.view_matrix * camera2.view_matrix * vec4(in_position, 1.0f);
					gl_Position = camera.projection_matrix * camera.view_matrix * vec4(in_position, 1.0f);
					// gl_Position = camera2.view_matrix * vec4(in_position, 1.0f);
				}
			)",

		.glsl_vulkan_code_fragment =
			R"(
				#version 460

				#extension GL_ARB_separate_shader_objects : enable

				precision highp int;
				precision highp float;

				// layout (set = 0, binding = 2) buffer _Camera2
				// {
				// 	float data [];
				// } _camera2;

				layout (std430, set = 0, binding = 2) buffer _Camera2
				{
					float _camera2 [];
				};

				// layout (set = 0, binding = 2) buffer _Camera2_UI
				// {
				// 	uint _camera2_ui [];
				// };

				layout (location = 0) out vec4 fragment_color;

				void main (void)
				{
					// fragment_color = vec4(0.25f, 0.0f, 1.0f, 1.0f);

					fragment_color = vec4(_camera2[0], 0.0f, 1.0f, 1.0f);
				}
			)",

		.wgsl_code_fragment =
			R"(
				[[stage(fragment)]] fn main() -> [[location(0)]] vec4<f32>
				{
					return vec4<f32>(1.0, 0.0, 1.0, 1.0);
				}
			)",
	};

	uniform_block0 =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 0,
			.name = "Camera",
		};

	uniform_block1 =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 1,
			.name = "Camera2",
		};

	_object = new RDTY::WRAPPERS::Object;
	object2 = new RDTY::WRAPPERS::Object;

	memcpy(object2->position_data.data(), std::vector({ -1.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f }).data(), 36);

	// scene->addObject(*_object);
	// scene->addObject(*object2);

	orbit = new RDTY::MATH::Orbit;

	orbit->object.setTransZ(50.0f);
	orbit->update();

	orbit->projection_matrix.makeProjPersp(45.0f, static_cast<float>(renderer->width) / static_cast<float>(renderer->height), 1.0f, 2000.0f, 1.0f);



	orbit2 = new RDTY::MATH::Orbit;

	orbit2->object.setTransZ(10.0f);
	orbit2->update();

	orbit2->projection_matrix.makeProjPersp(45.0f, static_cast<float>(renderer->width) / static_cast<float>(renderer->height), 1.0f, 2000.0f, 1.0f);




	RDTY::WRAPPERS::Uniform* projection_matrix_uniform
	{ new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit->projection_matrix), .name = "projection_matrix", .block_index = offsetof(RDTY::MATH::Orbit, projection_matrix), .size = sizeof(orbit->projection_matrix) } };

	RDTY::WRAPPERS::Uniform* view_matrix_uniform
	{ new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit->view_matrix), .name = "view_matrix", .block_index = offsetof(RDTY::MATH::Orbit, view_matrix), .size = sizeof(orbit->view_matrix) } };

	material->injectUniform(projection_matrix_uniform);
	material->injectUniform(view_matrix_uniform);

	material2->injectUniform(projection_matrix_uniform);
	material2->injectUniform(view_matrix_uniform);

	uniform_block0->injectUniform(projection_matrix_uniform);
	uniform_block0->injectUniform(view_matrix_uniform);

	uniform_block1->injectUniform(new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit2->view_matrix), .block_index = 0, .size = sizeof(float) });

	material->injectUniformBlock(uniform_block0);
	material2->injectUniformBlock(uniform_block0);



	desc_set1 = new RDTY::WRAPPERS::DescriptorSet;
	desc_set2 = new RDTY::WRAPPERS::DescriptorSet;

	desc_set1->bindings.push_back(uniform_block0);

	desc_set2->bindings.push_back(uniform_block0);
	desc_set2->bindings.push_back(uniform_block1);

	material->descriptor_sets.push_back(desc_set1);
	material2->descriptor_sets.push_back(desc_set2);



	surface_uniform_block_camera =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 4,
			.name = "Camera",
		};

	surface_uniform_block_camera->injectUniform
	(
		new RDTY::WRAPPERS::Uniform
		{ .object_addr = &(orbit->object.mat), .block_index = 0, .size = 64 }
	);

	surface_uniform_block_camera->injectUniform
	(
		new RDTY::WRAPPERS::Uniform
		{ .object_addr = &(orbit->projection_matrix), .block_index = 64, .size = 64 }
	);

	surface_uniform_block_camera->injectUniform
	(
		new RDTY::WRAPPERS::Uniform
		{ .object_addr = &(orbit->view_matrix), .block_index = 128, .size = 64 }
	);



	surface_object = new RDTY::WRAPPERS::Object;

	memcpy(surface_object->position_data.data(), std::vector({ -1.0f, -1.0f, 0.0f, -1.0f, 3.0f, 0.0f, 3.0f, -1.0f, 0.0f }).data(), 36);
	// memcpy(surface_object->index_data.data(), std::vector({ 0, 1, 2 }).data(), 12);



	surface_material = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.glsl_vulkan_code_vertex =
			R"(
				#version 460

				#extension GL_ARB_separate_shader_objects : enable

				precision highp int;
				precision highp float;

				layout (location = 0) in vec3 in_position;

				layout (location = 0) out VertexOut
				{
					vec3 position_ndc;
				};



				void main (void)
				{
					gl_Position = vec4(in_position, 1.0f);

					position_ndc = in_position;
				}
			)",

		.glsl_vulkan_code_fragment =
			R"(
				#version 460

				#extension GL_ARB_separate_shader_objects : enable

				precision highp int;
				precision highp float;



				struct Ray
				{
					vec3 origin;
					vec3 direction;
				};

				// struct Box
				// {
				// 	vec3 min;
				// 	vec3 max;
				// 	uint triangle_start;
				// 	uint triangle_end;
				// };

				struct Box
				{
					float min_x, min_y, min_z;
					float max_x, max_y, max_z;
					uint triangle_start;
					uint triangle_end;
				};



				layout (std430, set = 0, binding = 0) readonly buffer ScenePositionData
				{
					// float scene_position_data [792588 / 4];
					// float scene_position_data [];
					vec3 scene_position_data [];
				};

				layout (std430, set = 0, binding = 2) readonly buffer SceneIndexData
				{
					// uint scene_index_data [1566720 / 4];
					// uint scene_index_data [];
					uvec3 scene_index_data [];
				};

				layout (std430, set = 0, binding = 1) readonly buffer SceneTriData
				{
					uint triangles_data [];
				};

				layout (std430, set = 0, binding = 3) readonly buffer BoxTreeUint32
				{
					// uint box_tree_uint [2048 * 1024];
					// uint box_tree_uint [];
					Box boxes [];
				};

				layout (set = 0, binding = 4) uniform Camera
				{
					mat4 matrix;
					// mat4 projection_matrix;
					// mat4 view_matrix;
				} camera;



				layout (location = 0) in VertexOut
				{
					vec3 position_ndc;
				};



				layout (location = 0) out vec4 fragment_color;



				Ray ray;

				float fov = radians(45.0f);
				float fx = tan(fov / 2.0f) / 800.0f;
				vec2 d = fx * (gl_FragCoord.xy * 2.0f - vec2(800.0f, 600.0f));



				// // SPIR-V has no built-in inverse ?
				// mat4 _inverse (mat4 m)
				// {
				// 	float
				// 			a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
				// 			a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
				// 			a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
				// 			a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

				// 			b00 = a00 * a11 - a01 * a10,
				// 			b01 = a00 * a12 - a02 * a10,
				// 			b02 = a00 * a13 - a03 * a10,
				// 			b03 = a01 * a12 - a02 * a11,
				// 			b04 = a01 * a13 - a03 * a11,
				// 			b05 = a02 * a13 - a03 * a12,
				// 			b06 = a20 * a31 - a21 * a30,
				// 			b07 = a20 * a32 - a22 * a30,
				// 			b08 = a20 * a33 - a23 * a30,
				// 			b09 = a21 * a32 - a22 * a31,
				// 			b10 = a21 * a33 - a23 * a31,
				// 			b11 = a22 * a33 - a23 * a32,

				// 			det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

				// 	return mat4(
				// 			a11 * b11 - a12 * b10 + a13 * b09,
				// 			a02 * b10 - a01 * b11 - a03 * b09,
				// 			a31 * b05 - a32 * b04 + a33 * b03,
				// 			a22 * b04 - a21 * b05 - a23 * b03,
				// 			a12 * b08 - a10 * b11 - a13 * b07,
				// 			a00 * b11 - a02 * b08 + a03 * b07,
				// 			a32 * b02 - a30 * b05 - a33 * b01,
				// 			a20 * b05 - a22 * b02 + a23 * b01,
				// 			a10 * b10 - a11 * b08 + a13 * b06,
				// 			a01 * b08 - a00 * b10 - a03 * b06,
				// 			a30 * b04 - a31 * b02 + a33 * b00,
				// 			a21 * b02 - a20 * b04 - a23 * b00,
				// 			a11 * b07 - a10 * b09 - a12 * b06,
				// 			a00 * b09 - a01 * b07 + a02 * b06,
				// 			a31 * b01 - a30 * b03 - a32 * b00,
				// 			a20 * b03 - a21 * b01 + a22 * b00) / det;
				// }



				// Ray getRayFromPixel (vec2 pixel_coord)
				// {
				// 	Ray ray;

				// 	ray.origin = camera.matrix[3].xyz;

				// 	ray.direction.xy = pixel_coord;
				// 	ray.direction.z = 0.5f;

				// 	// ray.direction = (camera.matrix * _inverse(camera.projection_matrix) * vec4(ray.direction, 1.0)).xyz;
				// 	ray.direction = (_inverse(camera.view_matrix) * _inverse(camera.projection_matrix) * vec4(ray.direction, 1.0)).xyz;
				// 	ray.direction = normalize(ray.direction - ray.origin);

				// 	return ray;
				// }



				vec3 intersection_box = ray.origin;
				vec3 intersection_box_far = ray.origin;
				vec3 intersection_box_far1 = ray.origin;
				vec3 intersection = ray.origin;



				// bool getIntersectionRayBox (Ray ray, vec3 box_min, vec3 box_max)
				// {
				// 	vec3 tMin = (box_min - ray.origin) / ray.direction;
				// 	vec3 tMax = (box_max - ray.origin) / ray.direction;
				// 	vec3 t1 = min(tMin, tMax);
				// 	vec3 t2 = max(tMin, tMax);
				// 	float tNear = max(max(t1.x, t1.y), t1.z);
				// 	float tFar = min(min(t2.x, t2.y), t2.z);

				// 	if (tNear > tFar)
				// 	{
				// 		return false;
				// 	}

				// 	intersection_box = ray.direction;
				// 	intersection_box *= tNear;
				// 	intersection_box += ray.origin;

				// 	return true;
				// }

				bool getIntersectionRayBox (Ray ray, vec3 box_min, vec3 box_max)
				{
					vec3 tMin = (box_min - ray.origin) / ray.direction;
					vec3 tMax = (box_max - ray.origin) / ray.direction;
					vec3 t1 = min(tMin, tMax);
					vec3 t2 = max(tMin, tMax);
					float tNear = max(max(t1.x, t1.y), t1.z);
					float tFar = min(min(t2.x, t2.y), t2.z);

					if (tNear > tFar)
					{
						return false;
					}

					intersection_box = ray.direction;
					intersection_box *= tNear;
					intersection_box += ray.origin;

					intersection_box_far1 = ray.direction;
					intersection_box_far1 *= tFar;
					intersection_box_far1 += ray.origin;

					return true;
				}

				bool getIntersectionRayBoxFar (Ray ray, vec3 box_min, vec3 box_max)
				{
					vec3 tMin = (box_min - ray.origin) / ray.direction;
					vec3 tMax = (box_max - ray.origin) / ray.direction;
					vec3 t1 = min(tMin, tMax);
					vec3 t2 = max(tMin, tMax);
					float tNear = max(max(t1.x, t1.y), t1.z);
					float tFar = min(min(t2.x, t2.y), t2.z);

					// TODO: remove testing?
					if (tNear > tFar)
					{
						return false;
					}

					intersection_box_far = ray.direction;
					intersection_box_far *= tFar;
					intersection_box_far += ray.origin;

					return true;
				}



				float PointInOrOn(vec3 P1, vec3 P2, vec3 A, vec3 B)
				{
					vec3 ab = B - A;
					vec3 CP1 = cross(ab, P1 - A);
					vec3 CP2 = cross(ab, P2 - A);

					return step(0.0, dot(CP1, CP2));
				}

				float getIntersectionRayTriangle (Ray ray, vec3 p0, vec3 p1, vec3 p2)
				{
					vec3 N = cross(p1 - p0, p2 - p0);

					intersection = ray.origin + ray.direction * dot(p0 - ray.origin, N) / dot(ray.direction, N);

					return
					(
						PointInOrOn(intersection, p0, p1, p2) *
						PointInOrOn(intersection, p1, p2, p0) *
						PointInOrOn(intersection, p2, p0, p1)
					);
				}



				#define BOX_TREE_LEVEL_NUM 7



				void main (void)
				{
					ray.origin = camera.matrix[3].xyz;

					mat4 mmm = camera.matrix;
					mmm[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

					ray.direction = normalize((mmm * vec4(vec3(d.xy, 1.0), 1.0f)).xyz);



					intersection_box = ray.origin;
					intersection_box_far = ray.origin;
					intersection_box_far1 = ray.origin;
					intersection = ray.origin;
					vec3 p1 = vec3(0.0f);
					vec3 p2 = vec3(0.0f);
					vec3 p3 = vec3(0.0f);
					vec3 min = vec3(-10.0f, -10.0f, -10.0f);
					vec3 max = vec3(10.0f, 10.0f, 10.0f);

					float nearest_ray_triangle_intersection = 999999.0f;

					const uint dimension_segment_count = 32;

					ivec3 dimensions = ivec3(dimension_segment_count * dimension_segment_count, dimension_segment_count, 1);



					if (getIntersectionRayBox(ray, min, max))
					{
						float size = max.x - min.x;
						float segment_size = size / float(dimension_segment_count);

						ivec3 xyz = ivec3((intersection_box + (size * 0.5f)) / segment_size);

						if (xyz.x == -1)
						{
							++xyz.x;
						}

						if (xyz.y == -1)
						{
							++xyz.y;
						}

						if (xyz.z == -1)
						{
							++xyz.z;
						}

						if (xyz.x == dimension_segment_count)
						{
							--xyz.x;
						}

						if (xyz.y == dimension_segment_count)
						{
							--xyz.y;
						}

						if (xyz.z == dimension_segment_count)
						{
							--xyz.z;
						}

						uint box_index = uint(dot(xyz, dimensions));
						// uint box_index = xyz.x * dimension_segment_count * dimension_segment_count + xyz.y * dimension_segment_count + xyz.z;

						for (uint i = 0; i < dimension_segment_count; ++i)
						{
							Box box = boxes[box_index];

							if (box.triangle_end == box.triangle_start)
							{
								fragment_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);

								return;
							}

							uint intersected_triangle_count = 0;

							for (uint t_start = box.triangle_start; t_start < box.triangle_end; ++t_start)
							{
								uint triangle_index = triangles_data[t_start];

								uvec3 vertex_indices = scene_index_data[triangle_index];

								p1 = scene_position_data[vertex_indices.x];
								p2 = scene_position_data[vertex_indices.y];
								p3 = scene_position_data[vertex_indices.z];



								if (getIntersectionRayTriangle(ray, p1, p2, p3) == 1.0f)
								{
									++intersected_triangle_count;

									float ray_origin_to_intersection_distance = distance(ray.origin, intersection);

									if (ray_origin_to_intersection_distance < nearest_ray_triangle_intersection && ray_origin_to_intersection_distance > 0.001f)
									{
										nearest_ray_triangle_intersection = ray_origin_to_intersection_distance;
									}
								}
							}

							if (intersected_triangle_count > 0)
							{
								fragment_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

								return;

								// break;
							}

							min = vec3(box.min_x, box.min_y, box.min_z);
							max = vec3(box.max_x, box.max_y, box.max_z);

							getIntersectionRayBoxFar(ray, min, max);

							// If current box far intersection equals bounding box intersection,
							// it means that there is no triangle intersected.
							if (distance(intersection_box_far, intersection_box_far1) < 0.000001f)
							{
								fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

								return;

								// break;
							}

							vec3 aa = abs(intersection_box_far - min);
							vec3 bb = abs(intersection_box_far - max);

							xyz += ivec3((aa / aa) - (bb / bb));


							// if (abs(intersection_box_far.x - min.x) < 0.000001f)
							// {
							// 	--xyz.x;
							// }
							// else if (abs(intersection_box_far.x - max.x) < 0.000001f)
							// {
							// 	++xyz.x;
							// }

							// if (abs(intersection_box_far.y - min.y) < 0.000001f)
							// {
							// 	--xyz.y;
							// }
							// else if (abs(intersection_box_far.y - max.y) < 0.000001f)
							// {
							// 	++xyz.y;
							// }

							// if (abs(intersection_box_far.z - min.z) < 0.000001f)
							// {
							// 	--xyz.z;
							// }
							// else if (abs(intersection_box_far.z - max.z) < 0.000001f)
							// {
							// 	++xyz.z;
							// }


							// next box index
							box_index = uint(dot(xyz, dimensions));
							// box_index = xyz.x * dimension_segment_count * dimension_segment_count + xyz.y * dimension_segment_count + xyz.z;
						}

						// if (nearest_ray_triangle_intersection < 999.0f)
						// {
						// 	fragment_color = vec4(normalize(intersection), 1.0f);
						// }
						// else
						// {
						// 	fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
						// }
					}



					// uint qwe = 0;

					// uint current_level = 0;
					// uint box_sizes [BOX_TREE_LEVEL_NUM + 1];
					// uint box_counters [BOX_TREE_LEVEL_NUM + 1];
					// uint box_offsets [(BOX_TREE_LEVEL_NUM + 1) * 8];

					// for (uint i = 0; i < BOX_TREE_LEVEL_NUM + 1; ++i)
					// {
					// 	box_sizes[i] = 0;
					// 	box_counters[i] = 0;
					// }

					// for (uint i = 0; i < (BOX_TREE_LEVEL_NUM + 1) * 8; ++i)
					// {
					// 	box_offsets[i] = 0;
					// }

					// box_sizes[0] = 1;

					// uint box_offset = 0;

					// for (;;)
					// {
					// 	++box_counters[current_level];

					// 	vec3 box_min =
					// 		vec3
					// 		(
					// 			uintBitsToFloat(box_tree_uint[box_offset]),
					// 			uintBitsToFloat(box_tree_uint[box_offset + 1]),
					// 			uintBitsToFloat(box_tree_uint[box_offset + 2])
					// 		);

					// 	vec3 box_max =
					// 		vec3
					// 		(
					// 			uintBitsToFloat(box_tree_uint[box_offset + 4]),
					// 			uintBitsToFloat(box_tree_uint[box_offset + 5]),
					// 			uintBitsToFloat(box_tree_uint[box_offset + 6])
					// 		);

					// 	if
					// 	(
					// 		getIntersectionRayBox(ray, box_min, box_max) &&
					// 		distance(ray.origin, intersection_box) < nearest_ray_triangle_intersection
					// 	)
					// 	{
					// 		uint box_count = box_tree_uint[box_offset + 8];

					// 		if (box_count == 0)
					// 		{
					// 			uint triangle_count = box_tree_uint[box_offset + 9];

					// 			for (uint i = 0; i < triangle_count; ++i)
					// 			{
					// 				uint triangle_first_point_index = box_tree_uint[box_offset + 10 + i];

					// 				uint vertex1_index = scene_index_data[triangle_first_point_index];
					// 				uint vertex2_index = scene_index_data[triangle_first_point_index + 1];
					// 				uint vertex3_index = scene_index_data[triangle_first_point_index + 2];

					// 				uint vertex1_x_coord_index = vertex1_index * 3;
					// 				uint vertex2_x_coord_index = vertex2_index * 3;
					// 				uint vertex3_x_coord_index = vertex3_index * 3;

					// 				p1 =
					// 					vec3
					// 					(
					// 						scene_position_data[vertex1_x_coord_index],
					// 						scene_position_data[vertex1_x_coord_index + 1],
					// 						scene_position_data[vertex1_x_coord_index + 2]
					// 					);

					// 				p2 =
					// 					vec3
					// 					(
					// 						scene_position_data[vertex2_x_coord_index],
					// 						scene_position_data[vertex2_x_coord_index + 1],
					// 						scene_position_data[vertex2_x_coord_index + 2]
					// 					);

					// 				p3 =
					// 					vec3
					// 					(
					// 						scene_position_data[vertex3_x_coord_index],
					// 						scene_position_data[vertex3_x_coord_index + 1],
					// 						scene_position_data[vertex3_x_coord_index + 2]
					// 					);

					// 				if (getIntersectionRayTriangle(ray, p1, p2, p3) == 1.0f)
					// 				// if (floatBitsToUint(getIntersectionRayTriangle(ray, p1, p2, p3)) != 0)
					// 				{
					// 					float ray_origin_to_intersection_distance = distance(ray.origin, intersection);

					// 					if (ray_origin_to_intersection_distance < nearest_ray_triangle_intersection && ray_origin_to_intersection_distance > 0.001f)
					// 					{
					// 						nearest_ray_triangle_intersection = ray_origin_to_intersection_distance;
					// 					}
					// 				}
					// 			}
					// 		}

					// 		if (box_counters[current_level] > box_sizes[current_level])
					// 		{
					// 			--current_level;

					// 			if (current_level == 0)
					// 			{
					// 				break;
					// 			}
					// 		}
					// 		else
					// 		{
					// 			++current_level;

					// 			for (uint i = 0; i < box_count; ++i)
					// 			{
					// 				box_offsets[(current_level * 8) + i] = box_tree_uint[box_offset + 9 + i];
					// 			}

					// 			box_sizes[current_level] = box_count;
					// 			box_counters[current_level] = 0;
					// 		}

					// 		box_offset = box_offsets[(current_level * 8) + box_counters[current_level]];

					// 		continue;
					// 	}

					// 	if (current_level == 0)
					// 	{
					// 		fragment_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);

					// 		return;
					// 	}

					// 	if (box_counters[current_level] > box_sizes[current_level])
					// 	{
					// 		--current_level;

					// 		// Remove ?
					// 		if (current_level == 0)
					// 		{
					// 			break;
					// 		}
					// 	}

					// 	box_offset = box_offsets[(current_level * 8) + box_counters[current_level]];
					// }

					// if (nearest_ray_triangle_intersection < 999.0f)
					// {
					// 	// fragment_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
					// 	fragment_color = vec4(normalize(intersection), 1.0f);
					// }
					// else
					// {
					// 	fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
					// }
				}
			)",
	};
}

extern "C" void constructRenderityWrappers2 (void)
{
	scene->addObject(*_object);
	// scene->addObject(*object2);
	// scene->addObject(*surface_object);
}
