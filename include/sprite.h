#pragma once

#include "pch.h"

typedef struct {
    vec3 position;
    vec3 scale;
    mat4 rotation;
} transform_t;

typedef struct {
    transform_t transform;

    vec4 color;

    // TODO: These should move into a struct called render_data that will be given to renderer
    u64 vbo;
    u64 ibo;
    u64 vao;
} sprite_t;

sprite_t create_sprite(const char* name, const char* path);

void set_position(transform_t* transform, vec3 position);
void set_scale(transform_t* transform, vec3 scale);

// TODO: This will be moved into renderer
void draw_sprite(sprite_t* sprite);