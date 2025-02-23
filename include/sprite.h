#pragma once

#include "cglm/cglm.h"
#include "defines.h"

typedef struct {
    // transform data
    vec3 position;
    vec3 scale;
    mat4 rotation;

    // gl data
    u64 vbo;
    u64 ibo;
    u64 vao;
} sprite_t;

sprite_t create_sprite(const char* name, const char* path);

void draw(sprite_t* sprite);