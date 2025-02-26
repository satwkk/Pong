#pragma once

#include "pch.h"
#include "sprite.h"
#include "shader.h"

#define VERTEX_SHADER_PATH "./resources/shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH "./resources/shaders/fragment.glsl"
#define MAX_SPRITE_ENTRY 50

typedef struct {
    float w;
    float h;
    float near_plane;
    float far_plane;
} render_config_t;

typedef struct {
    mat4 projection;
    mat4 model;
    u64 model_uniform_loc;
    u64 projection_uniform_loc;
    // TODO: Fix the dlist and replace it with something dynamic for particles rendering
    sprite_t* sprite_resource_arr[MAX_SPRITE_ENTRY];
    u64 sprite_entry_idx;
    shader_t vertex_shader;
    shader_t fragment_shader;
    u64 program_id;
} renderer_t;

renderer_t init_renderer(render_config_t config);
void push_sprite(renderer_t* ctx, sprite_t* sprite);
void cleanup_renderer(renderer_t* renderer);
void renderer_update(renderer_t* ctx);
void draw_sprite(sprite_t* sprite);