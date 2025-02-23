#pragma once

#include "shader.h"
#include "pch.h"

#define VERTEX_SHADER_PATH "./resources/shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH "./resources/shaders/fragment.glsl"

typedef struct {
    shader_t vertex_shader;
    shader_t fragment_shader;
    u64 program_id;
} resource_container;

int load_resources(resource_container* resources);
void cleanup_resources(resource_container* resources);