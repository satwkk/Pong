#pragma once

#include "pch.h"

typedef struct {
    vec3 position;
    vec3 scale;
    float angle;
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


sprite_t* create_sprite(const char* name, vec4 color, const char* path);
int bind_texture(const char* path, sprite_t* sprite);
void set_position(sprite_t* sprite, vec3 position);
void set_rotation(sprite_t *sprite, float angle);
void set_scale(sprite_t* sprite, vec3 scale);