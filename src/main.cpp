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

#include "renderity/test/src/objects/o1.h"
#include "renderity/test/src/objects/o2.h"
#include "renderity/test/src/objects/o3.h"



#ifdef __wasm__
	#include "renderity/wasm-wrapper/src/wasm-log.h"
#else
	#include <iostream>

	#define LOG(x) std::cout << x << std::endl;
#endif



#include <iostream>
using std::cout;
using std::endl;



const O1 o1;
const O2 o2;
const O3 o3;



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
RDTY::WRAPPERS::Object* object3 {};
RDTY::WRAPPERS::Object* object4 {};
RDTY::WRAPPERS::DescriptorSet* desc_set1 {};
RDTY::WRAPPERS::DescriptorSet* desc_set2 {};

RDTY::WRAPPERS::UniformBlock* surface_uniform_block_camera {};
RDTY::WRAPPERS::StorageBlock* storage_block {};

RDTY::WRAPPERS::DescriptorSet* descriptor_set_scene {};

RDTY::WRAPPERS::Material* surface_material {};
RDTY::WRAPPERS::Object* surface_object {};

float curve_values [300];
RDTY::Transition orbit_transition;
RDTY::Transition orbit_transition2;

float xxx {};
float yyy {};

extern "C" void ___test (const size_t& time_gone)
{
	static size_t prev_time {};

	float temp { curve_values[time_gone - 1] };

	if (time_gone < prev_time)
	{
		prev_time = 0;
	}

	// orbit->rotation_speed_x = orbit->rotation_speed_y = temp * (time_gone - prev_time) * 0.01;

	orbit->rotation_speed_x *= temp;
	orbit->rotation_speed_y *= temp;

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

		300
	);

	_stack0 = new RDTY::TransitionStack { 64 };
	_stack1 = new RDTY::TransitionStack { 64 };
}

extern "C" void updateTransitions (RDTY::TransitionStack* stack)
{
	#ifndef __wasm__
		for (;;)
	#endif
		{
			stack->calculateFrametime();
			stack->update();
		}
}

extern "C" void logStacks (void)
{
	LOG(_stack0->length)
	LOG(_stack1->length)
}

extern "C" void startTransition (const float _xxx, const float _yyy)
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

	orbit->rotation_speed_x += _xxx;
	orbit->rotation_speed_y += _yyy;

	// LOG(_xxx)
	// LOG(_yyy)

	orbit_transition.start2(300, ___test);
}

extern "C" void startTransition2 (void)
{
	orbit_transition2.start2(300, ___test2);
}

extern "C" void constructStage1 (void)
{
	renderer = new RDTY::WRAPPERS::Renderer { .width = 800, .height = 600 };

	scene = new RDTY::WRAPPERS::Scene;



	RDTY::WRAPPERS::Uniform* projection_matrix_uniform
	{ new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit->projection_matrix), .name = "projection_matrix", .block_index = offsetof(RDTY::MATH::Orbit, projection_matrix), .size = sizeof(orbit->projection_matrix) } };

	RDTY::WRAPPERS::Uniform* view_matrix_uniform
	{ new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit->view_matrix), .name = "view_matrix", .block_index = offsetof(RDTY::MATH::Orbit, view_matrix), .size = sizeof(orbit->view_matrix) } };



	uniform_block0 =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 0,
			.name = "Camera",

			.uniforms =
			{
				projection_matrix_uniform,
				view_matrix_uniform,
			},
		};

	uniform_block1 =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 1,
			.name = "Camera2",

			.uniforms =
			{
				new RDTY::WRAPPERS::Uniform { .object_addr = &(orbit2->view_matrix), .block_index = 0, .size = sizeof(float) },
			},
		};



	desc_set1 =
		new RDTY::WRAPPERS::DescriptorSet
		{
			.bindings =
			{
				uniform_block0,
			},
		};

	desc_set2 =
		new RDTY::WRAPPERS::DescriptorSet
		{
			.bindings =
			{
				uniform_block0,
				uniform_block1,
			},
		};



	material = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.code_vertex_wgsl =
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

		.uniforms =
		{
			projection_matrix_uniform,
			view_matrix_uniform,
		},

		.uniform_blocks =
		{
			uniform_block0,
		},

		.descriptor_sets =
		{
			desc_set1,
		},
	};

	material2 = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.blend_enabled = RDTY::WRAPPERS::MATERIAL::BlendEnabled::FALSE,

		.code_vertex_glsl =
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

		.code_fragment_glsl =
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

		.code_fragment_glsl100es =
			R"(
				precision highp int;
				precision highp float;

				void main (void)
				{
					gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
				}
			)",

		.code_fragment_glsl300es =
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

		.code_fragment_wgsl =
			R"(
				[[stage(fragment)]] fn main() -> [[location(0)]] vec4<f32>
				{
					return vec4<f32>(1.0, 0.0, 1.0, 1.0);
				}
			)",

		.uniforms =
		{
			projection_matrix_uniform,
			view_matrix_uniform,
		},

		.uniform_blocks =
		{
			uniform_block0,
		},

		.descriptor_sets =
		{
			desc_set2,
		},
	};

	_object = new RDTY::WRAPPERS::Object;
	object2 = new RDTY::WRAPPERS::Object;
	object3 = new RDTY::WRAPPERS::Object;
	object4 = new RDTY::WRAPPERS::Object;

	_object->dimension_segment_count = 32;
	object2->dimension_segment_count = 32;
	object3->dimension_segment_count = 32;
	object4->dimension_segment_count = 32;

	// memcpy(object2->position_data.data(), std::vector({ -1.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f }).data(), 36);

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



	surface_uniform_block_camera =
		new RDTY::WRAPPERS::UniformBlock
		{
			.binding = 4,
			.name = "Camera",
			.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },

			.uniforms =
			{
				new RDTY::WRAPPERS::Uniform
				{ .object_addr = &(orbit->object.mat), .block_index = 0, .size = 64 },

				new RDTY::WRAPPERS::Uniform
				{ .object_addr = &(orbit->projection_matrix), .block_index = 64, .size = 64 },

				new RDTY::WRAPPERS::Uniform
				{ .object_addr = &(orbit->view_matrix), .block_index = 128, .size = 64 },
			},
		};



	surface_object = new RDTY::WRAPPERS::Object;

	surface_object->position_data.resize(9);
	surface_object->normal_data.resize(9);
	memcpy(surface_object->position_data.data(), std::vector({ -1.0f, -1.0f, 0.0f, -1.0f, 3.0f, 0.0f, 3.0f, -1.0f, 0.0f }).data(), 36);



	surface_material = new RDTY::WRAPPERS::Material
	{
		.topology = RDTY::WRAPPERS::MATERIAL::Topology::TRIANGLES,

		.code_vertex_glsl =
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

		.code_fragment_glsl =
			R"(
				#version 460

				#extension GL_ARB_separate_shader_objects : enable

				precision highp int;
				precision highp float;



				#ifdef VULKAN
					#define BINDING(_set, _binding) set = _set, binding = _binding
				#else
					#define BINDING(_set, _binding) binding = _binding
				#endif



				struct Ray
				{
					vec3 origin;
					vec3 direction;
				};



				struct Vertex
				{
					vec3 position;
					uint object_index;
				};

				layout (BINDING(0, 0), std430) readonly buffer SceneVertexData
				{
					Vertex scene_vertex_data [];
				};

				layout (BINDING(0, 5), std430) readonly buffer SceneNormalData
				{
					vec3 scene_normal_data [];
				};

				// layout (BINDING(0, 2), std430) readonly buffer SceneIndexData
				// {
				// 	uvec3 scene_index_data [];
				// };

				layout (BINDING(0, 1), std430) readonly buffer SceneTriData
				{
					// uint triangles_data [];
					uvec3 triangles_data [];
				};

				struct Box
				{
					vec3 min;
					uint triangle_start;
					vec3 max;
					uint triangle_end;
				};

				layout (BINDING(0, 3), std430) readonly buffer BoxTreeUint32
				{
					Box boxes [];
				};

				layout (BINDING(0, 4)) uniform Camera
				{
					mat4 matrix;
					// mat4 projection_matrix;
					// mat4 view_matrix;
				} camera;



				struct Material
				{
					vec4 color;
				};

				struct Object
				{
					Material material;
				};

				Material materials[4];

				// materials[0].color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
				// materials[1].color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
				// materials[2].color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
				// materials[3].color = vec4(1.0f, 1.0f, 0.0f, 1.0f);

				Object objects[4];

				// objects[0].material = materials[0];
				// objects[1].material = materials[1];
				// objects[2].material = materials[2];
				// objects[3].material = materials[3];



				layout (location = 0) in VertexOut
				{
					vec3 position_ndc;
				};



				layout (location = 0) out vec4 fragment_color;



				Ray ray;

				float fov = radians(45.0f);
				float fx = tan(fov / 2.0f) / 800.0f;
				// vec2 d = fx * (gl_FragCoord.xy * 2.0f - vec2(800.0f, 600.0f));
				vec2 d = fx * position_ndc.xy * vec2(800.0f, 600.0f);



				vec3 intersection_box = ray.origin;
				vec3 intersection_box_far = ray.origin;
				vec3 intersection_box_far1 = ray.origin;
				vec3 intersection = ray.origin;
				vec3 _intersection = ray.origin;



				bool getIntersectionRayBox (Ray _ray, vec3 box_min, vec3 box_max)
				{
					vec3 tMin = (box_min - _ray.origin) / _ray.direction;
					vec3 tMax = (box_max - _ray.origin) / _ray.direction;
					vec3 t1 = min(tMin, tMax);
					vec3 t2 = max(tMin, tMax);
					float tNear = max(max(t1.x, t1.y), t1.z);
					float tFar = min(min(t2.x, t2.y), t2.z);

					if (tNear > tFar)
					{
						return false;
					}

					intersection_box = _ray.direction;
					intersection_box *= tNear;
					intersection_box += _ray.origin;

					intersection_box_far1 = _ray.direction;
					intersection_box_far1 *= tFar;
					intersection_box_far1 += _ray.origin;

					// return true;
					return (dot(ray.direction, intersection_box - ray.origin) > 0.0f);
				}

				// bool getIntersectionRayBoxFar (Ray _ray, vec3 box_min, vec3 box_max)
				void getIntersectionRayBoxFar (Ray _ray, vec3 box_min, vec3 box_max)
				{
					vec3 tMin = (box_min - _ray.origin) / _ray.direction;
					vec3 tMax = (box_max - _ray.origin) / _ray.direction;
					vec3 t1 = min(tMin, tMax);
					vec3 t2 = max(tMin, tMax);
					float tNear = max(max(t1.x, t1.y), t1.z);
					float tFar = min(min(t2.x, t2.y), t2.z);

					// TODO: remove testing?
					// if (tNear > tFar)
					// {
					// 	return false;
					// }

					intersection_box_far = _ray.direction;
					intersection_box_far *= tFar;
					intersection_box_far += _ray.origin;

					// return true;
				}



				float PointInOrOn (vec3 P1, vec3 P2, vec3 A, vec3 B)
				{
					vec3 ab = B - A;
					vec3 CP1 = cross(ab, P1 - A);
					vec3 CP2 = cross(ab, P2 - A);

					return step(0.0f, dot(CP1, CP2));
				}

				// float getIntersectionRayTriangle (Ray ray, vec3 p0, vec3 p1, vec3 p2)
				float getIntersectionRayTriangle (vec3 p0, vec3 p1, vec3 p2)
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

				float getIntersectionRayTriangle2 (Ray _ray, vec3 p0, vec3 p1, vec3 p2)
				{
					vec3 N = cross(p1 - p0, p2 - p0);

					intersection = _ray.origin + _ray.direction * dot(p0 - _ray.origin, N) / dot(_ray.direction, N);

					return
					(
						PointInOrOn(intersection, p0, p1, p2) *
						PointInOrOn(intersection, p1, p2, p0) *
						PointInOrOn(intersection, p2, p0, p1)
					);
				}



				vec3 getBarycentricCoords(vec3 p, vec3 a, vec3 b, vec3 c)
				{
					vec3 v0 = b - a;
					vec3 v1 = c - a;
					vec3 v2 = p - a;

					float d00 = dot(v0, v0);
					float d01 = dot(v0, v1);
					float d11 = dot(v1, v1);
					float d20 = dot(v2, v0);
					float d21 = dot(v2, v1);

					float denom = d00 * d11 - d01 * d01;

					float v = (d11 * d20 - d01 * d21) / denom;
					float w = (d00 * d21 - d01 * d20) / denom;
					float u = 1.0f - v - w;

					return vec3(u, v, w);
				}

				struct Intersection
				{
					vec3 point;
					uvec3 vertex_indices;
					vec3 barycentric;
					vec3 normal;
					float distance;
					uint bounding_box_index;
				};

				// bool testPointInsideBox (vec3 point, vec3 min, vec3 max)
				// {
				// 	return
				// 	(
				// 		point.x <= max.x && point.x >= min.x &&
				// 		point.y <= max.y && point.y >= min.y &&
				// 		point.z <= max.z && point.z >= min.z
				// 	);
				// }

				bool testPointInsideBox (vec3 point, vec3 _min, vec3 _max)
				{
					return (min(point, _max) == max(point, _min));
				}



				void main (void)
				{
					materials[0].color = vec4(0.5f, 0.5f, 0.0f, 1.0f);
					// materials[0].color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
					materials[1].color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
					materials[2].color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
					materials[3].color = vec4(1.0f, 0.0f, 1.0f, 1.0f);

					objects[0].material = materials[0];
					objects[1].material = materials[1];
					objects[2].material = materials[2];
					objects[3].material = materials[3];



					uint bounce_count = 0;

					vec3 diffuse = vec3(1.0f);

					vec3 mixed = vec3(1.0f);



					ray.origin = camera.matrix[3].xyz;

					mat4 mmm = camera.matrix;
					mmm[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

					ray.direction = normalize((mmm * vec4(vec3(d.xy, -1.0), 1.0f)).xyz);



					intersection_box = ray.origin;
					intersection_box_far = ray.origin;
					intersection_box_far1 = ray.origin;
					intersection = ray.origin;

					Intersection _intersection2;

					Intersection _intersection;
					_intersection.point = ray.origin;
					_intersection.distance = 999999.0f;

					Intersection _intersection_o;

					vec3 p1 = vec3(0.0f);
					vec3 p2 = vec3(0.0f);
					vec3 p3 = vec3(0.0f);

					uvec3 vi;

					uint next_bounding_box_index = 0;

					for (;;)
					{
						Box bounding_box = boxes[next_bounding_box_index];

						uint bounding_box_index = next_bounding_box_index;

						next_bounding_box_index = bounding_box.triangle_end;

						uint dimension_segment_count = bounding_box.triangle_start;

						ivec3 dimensions = ivec3(dimension_segment_count * dimension_segment_count, dimension_segment_count, 1);



						// if (testPointInsideBox(ray.origin, bounding_box.min - 0.00001f, bounding_box.max + 0.00001f))
						if (testPointInsideBox(ray.origin, bounding_box.min + 0.00001f, bounding_box.max - 0.00001f))
						{
							// fragment_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
							// return;

							float size = bounding_box.max.x - bounding_box.min.x;
							float segment_size = size / dimension_segment_count;

							ivec3 xyz = clamp(ivec3((ray.origin - bounding_box.min) / segment_size), ivec3(0), ivec3(dimension_segment_count - 1));

							uint box_index = bounding_box_index + uint(dot(xyz, dimensions)) + 1;

							for (uint i = 0; i < dimension_segment_count * 4; ++i)
							{
								Box box = boxes[box_index];

								uint intersected_triangle_count = 0;

								for (uint t_start = box.triangle_start; t_start < box.triangle_end; ++t_start)
								{
									uvec3 vertex_indices = triangles_data[t_start];

									p1 = scene_vertex_data[vertex_indices.x].position;
									p2 = scene_vertex_data[vertex_indices.y].position;
									p3 = scene_vertex_data[vertex_indices.z].position;



									if (vi != vertex_indices && getIntersectionRayTriangle(p1, p2, p3) == 1.0f)
									{
										++intersected_triangle_count;

										float ray_origin_to_intersection_distance = distance(ray.origin, intersection);

										if (ray_origin_to_intersection_distance < _intersection.distance)
										{
											_intersection_o = _intersection;

											_intersection.distance = ray_origin_to_intersection_distance;
											_intersection.point = intersection;
											_intersection.vertex_indices = vertex_indices;
											// _intersection.bounding_box_index = bounding_box_index;
											_intersection.barycentric = getBarycentricCoords(_intersection.point, p1, p2, p3);
										}
									}

									intersection = _intersection.point;
								}

								if (intersected_triangle_count > 0)
								{
									break;
								}

								// Ray rrr = ray;

								// rrr.origin -= ray.direction * 10000.0f;

								// getIntersectionRayBox(rrr, bounding_box.min, bounding_box.max);
								// getIntersectionRayBoxFar(rrr, box.min, box.max);

								getIntersectionRayBox(ray, bounding_box.min, bounding_box.max);
								getIntersectionRayBoxFar(ray, box.min, box.max);

								if (distance(intersection_box_far, intersection_box_far1) < 0.00001f)
								{
									break;
								}

								vec3 aa = abs(intersection_box_far - box.min);
								vec3 bb = abs(intersection_box_far - box.max);

								xyz += ivec3((step(0.00001f, aa) - step(0.00001f, bb)));



								box_index = bounding_box_index + uint(dot(xyz, dimensions)) + 1;
							}
						}
						else if (getIntersectionRayBox(ray, bounding_box.min, bounding_box.max))
						{
							// fragment_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
							// return;

							float size = bounding_box.max.x - bounding_box.min.x;
							float segment_size = size / dimension_segment_count;

							ivec3 xyz = clamp(ivec3((intersection_box - bounding_box.min) / segment_size), ivec3(0), ivec3(dimension_segment_count - 1));

							// TODO: use custom udot function?
							uint box_index = bounding_box_index + uint(dot(xyz, dimensions)) + 1;

							for (uint i = 0; i < dimension_segment_count * 4; ++i)
							{
								Box box = boxes[box_index];

								uint intersected_triangle_count = 0;

								for (uint t_start = box.triangle_start; t_start < box.triangle_end; ++t_start)
								{
									// uint triangle_index = triangles_data[t_start];

									// uvec3 vertex_indices = scene_index_data[triangle_index];

									uvec3 vertex_indices = triangles_data[t_start];

									p1 = scene_vertex_data[vertex_indices.x].position;
									p2 = scene_vertex_data[vertex_indices.y].position;
									p3 = scene_vertex_data[vertex_indices.z].position;



									// if (getIntersectionRayTriangle(ray, p1, p2, p3) == 1.0f)
									if (getIntersectionRayTriangle(p1, p2, p3) == 1.0f)
									{
										++intersected_triangle_count;

										float ray_origin_to_intersection_distance = distance(ray.origin, intersection);

										if (ray_origin_to_intersection_distance < _intersection.distance)
										{
											_intersection_o = _intersection;

											_intersection.distance = ray_origin_to_intersection_distance;
											_intersection.point = intersection;
											_intersection.vertex_indices = vertex_indices;
											// _intersection.bounding_box_index = bounding_box_index;
											_intersection.barycentric = getBarycentricCoords(_intersection.point, p1, p2, p3);
										}
									}

									intersection = _intersection.point;
								}

								if (intersected_triangle_count > 0)
								{
									break;
								}

								getIntersectionRayBoxFar(ray, box.min, box.max);

								// If current box far intersection equals bounding box intersection,
								// it means that there is no triangle intersected.
								if (distance(intersection_box_far, intersection_box_far1) < 0.00001f)
								{
									break;
								}

								vec3 aa = abs(intersection_box_far - box.min);
								vec3 bb = abs(intersection_box_far - box.max);

								xyz += ivec3((step(0.00001f, aa) - step(0.00001f, bb)));



								// next box index
								box_index = bounding_box_index + uint(dot(xyz, dimensions)) + 1;
							}
						}



						if (next_bounding_box_index == 0)
						{
							// if (bounce_count == 1)
							// {
							// 	uint object_index = scene_vertex_data[_intersection.vertex_indices.x].object_index;

							// 	// // TODO: How to avoid this condition:
							// 	// // "dot(ray.direction, _intersection.point - ray.origin) > 0.0f"
							// 	// // ?
							// 	// // if (_intersection.distance < 999998.0f && dot(ray.direction, _intersection.point - ray.origin) > 0.0f)
							// 	// if (_intersection.distance < 999998.0f)
							// 	// // if (_intersection.distance < 25.0f)
							// 	// {
							// 	// 	// fragment_color = objects[object_index].material.color;

							// 	// 	// return;
							// 	// }

							// 	fragment_color = vec4(objects[object_index].material.color.rgb * diffuse, 1.0f);

							// 	return;
							// }

							if (_intersection.distance < 999998.0f)
							{
								vec3 n1 = scene_normal_data[_intersection.vertex_indices.x];
								vec3 n2 = scene_normal_data[_intersection.vertex_indices.y];
								vec3 n3 = scene_normal_data[_intersection.vertex_indices.z];

								// vec3 normal = vec3(1.0f, 0.0f, 0.0f);
								// vec3 normal = (n1 + n2 + n3) / 3.0f;
								// vec3 normal = normalize((n1 * _intersection.barycentric.x) + (n2 * _intersection.barycentric.y) + (n3 * _intersection.barycentric.z));
								// vec3 normal = (normalize(n1 * _intersection.barycentric.x) + normalize(n2 * _intersection.barycentric.y) + normalize(n3 * _intersection.barycentric.z));

								vec3 normal = (n1 * _intersection.barycentric.x) + (n2 * _intersection.barycentric.y) + (n3 * _intersection.barycentric.z);

								_intersection.normal = normal;

								ray.origin = _intersection.point;

								diffuse *= dot(normal, -ray.direction);

								// if (bounce_count == 0)
								// {
								// 	_intersection_o = _intersection;
								// }
								// else
								// {
								// 	uint object_index_o = scene_vertex_data[_intersection_o.vertex_indices.x].object_index;

								// 	// diffuse *= objects[object_index_o].material.color.rgb;

								// 	mixed = mix(mixed, objects[object_index_o].material.color.rgb, 0.5);
								// }

								ray.direction = reflect(ray.direction, normal);



								intersection_box = ray.origin;
								intersection_box_far = ray.origin;
								intersection_box_far1 = ray.origin;
								intersection = ray.origin;

								_intersection.point = ray.origin;
								_intersection.distance = 999999.0f;

								p1 = vec3(0.0f);
								p2 = vec3(0.0f);
								p3 = vec3(0.0f);

								next_bounding_box_index = 0;

								vi = _intersection.vertex_indices;



								if (bounce_count == 2)
								{
									uint object_index = scene_vertex_data[_intersection.vertex_indices.x].object_index;

									// uint object_index_o = scene_vertex_data[_intersection_o.vertex_indices.x].object_index;

									// // TODO: How to avoid this condition:
									// // "dot(ray.direction, _intersection.point - ray.origin) > 0.0f"
									// // ?
									// // if (_intersection.distance < 999998.0f && dot(ray.direction, _intersection.point - ray.origin) > 0.0f)
									// if (_intersection.distance < 999998.0f)
									// // if (_intersection.distance < 25.0f)
									// {
									// 	// fragment_color = objects[object_index].material.color;

									// 	// return;
									// }

									fragment_color = vec4(mix(objects[object_index].material.color.rgb, mixed, 0.5) * diffuse, 1.0f);

									return;
								}
								else
								{
									// _intersection_o = _intersection;

									uint object_index = scene_vertex_data[_intersection.vertex_indices.x].object_index;

									mixed = mix(objects[object_index].material.color.rgb, mixed, 0.5);

									// mixed = vec3(0.0f);
								}

								++bounce_count;
							}
							else
							{
								if (bounce_count == 0)
								{
									fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
								}
								else
								{
									uint object_index = scene_vertex_data[_intersection.vertex_indices.x].object_index;

									fragment_color = vec4(mix(objects[object_index].material.color.rgb, mixed, 0.5) * diffuse, 1.0f);
								}

								return;
							}
						}
					}
				}
			)",
	};
}

extern "C" void updateObjectsData (void)
{
	_object->position_data.resize(sizeof(o1.position_data) / 4);
	memcpy(_object->position_data.data(), o1.position_data, sizeof(o1.position_data));

	_object->normal_data.resize(sizeof(o1.normal_data) / 4);
	memcpy(_object->normal_data.data(), o1.normal_data, sizeof(o1.normal_data));

	_object->index_data.resize(sizeof(o1.index_data) / 4);
	memcpy(_object->index_data.data(), o1.index_data, sizeof(o1.index_data));



	// object2->position_data.resize(sizeof(o2.position_data) / 4);
	// memcpy(object2->position_data.data(), o2.position_data, sizeof(o2.position_data));

	// object2->normal_data.resize(sizeof(o2.normal_data) / 4);
	// memcpy(object2->normal_data.data(), o2.normal_data, sizeof(o2.normal_data));

	// object2->index_data.resize(sizeof(o2.index_data) / 4);
	// memcpy(object2->index_data.data(), o2.index_data, sizeof(o2.index_data));



	// object3->position_data.resize(sizeof(o3.position_data) / 4);
	// memcpy(object3->position_data.data(), o3.position_data, sizeof(o3.position_data));

	// object3->normal_data.resize(sizeof(o3.normal_data) / 4);
	// memcpy(object3->normal_data.data(), o3.normal_data, sizeof(o3.normal_data));

	// object3->index_data.resize(sizeof(o3.index_data) / 4);
	// memcpy(object3->index_data.data(), o3.index_data, sizeof(o3.index_data));
}

extern "C" void constructStage2 (void)
{
	scene->addObjects({ _object, object2, object3, object4 });
	// scene->addObjects({ _object, object2 });
	// scene->addObjects({ _object });
}

extern "C" void generateBoxes (RDTY::WRAPPERS::Object* object)
{
	object->makeBoundingBox();

	scene->testSimd(object);
}

extern "C" void constructStage3 (void)
{
	descriptor_set_scene =
		new RDTY::WRAPPERS::DescriptorSet
		{
			.bindings =
			{
				surface_uniform_block_camera,

				new RDTY::WRAPPERS::StorageBlock
				{
					.binding = 3,

					.data = scene->boxes,

					.size = sizeof(scene->boxes),

					.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },
				},

				new RDTY::WRAPPERS::StorageBlock
				{
					.binding = 1,

					.data = scene->triangles,

					.size = sizeof(scene->triangles),

					.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },
				},

				new RDTY::WRAPPERS::StorageBlock
				{
					.binding = 0,

					.data = scene->position_data.data(),

					.size = scene->position_data.size() * sizeof(float),

					.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },
				},

				new RDTY::WRAPPERS::StorageBlock
				{
					.binding = 5,

					.data = scene->normal_data.data(),

					.size = scene->normal_data.size() * sizeof(float),

					.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },
				},

				new RDTY::WRAPPERS::StorageBlock
				{
					.binding = 2,

					.data = scene->index_data.data(),

					.size = scene->index_data.size() * sizeof(uint32_t),

					.visibility = { RDTY::WRAPPERS::DESCRIPTOR_BINDING::Visibility::FRAGMENT },
				},
			},
		};

	surface_material->descriptor_sets.push_back(descriptor_set_scene);
}
