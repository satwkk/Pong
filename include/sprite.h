#pragma once

#include "pch.h"

typedef struct {
    vec3 position;
    vec3 scale;
    mat4 rotation;
} transform_t;

typedef struct {
    transform_t transform;

    // gl data
    u64 vbo;
    u64 ibo;
    u64 vao;
} sprite_t;

sprite_t create_sprite(const char* name, const char* path);

void setup_transform(transform_t* transform);

void draw_sprite(sprite_t* sprite);