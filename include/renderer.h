#pragma once

#include "pch.h"
#include "sprite.h"

typedef struct {
    float w;
    float h;
    float near_plane;
    float far_plane;
    u64 program_id;
} renderer_data_t;

typedef struct {
    mat4 projection;
    mat4 model;
    u64 program_id;
} renderer_t;

renderer_t init_renderer(renderer_data_t config);
void renderer_update(renderer_t* ctx, sprite_t* sprite);