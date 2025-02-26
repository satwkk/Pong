#pragma once

#include "pch.h"

typedef struct {
    vec3 position;
    vec3 scale;
    mat4 rotation;
} transform_t;

typedef struct {
    const char* name;
    transform_t transform;
    u64 texture_id;
    vec4 color;
    u64 vbo;
    u64 ibo;
    u64 vao;
} sprite_t;

sprite_t create_sprite(const char* name, const char* path);
int bind_texture(const char* path, sprite_t* sprite);

void set_position(sprite_t* sprite, vec3 position);
void set_scale(sprite_t* sprite, vec3 scale);