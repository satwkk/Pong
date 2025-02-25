#pragma once

#include "pch.h"

#define SHADER_LOAD_SUCCESS 0
#define SHADER_LOAD_FAILED -1

#define VERTEXSHADERPATH "./resources/shaders/vertex.shader"
#define FRAGMENTSHADERPATH "./resources/shaders/fragment.shader"

#define SHADER_COMPILE_CHECK(id, shader_name) \
{\
    i64 success = 0;\
    char buffer[1024];\
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);\
    if (!success) {\
        glGetShaderInfoLog(id, 1024, NULL, buffer);\
        fprintf(stderr, "%s compilation error\n%s\n", #shader_name, buffer);\
        exit(-1);\
    }\
}

#define PROGRAM_LINK_CHECK(id) \
{\
    i64 success = 0;\
    char buffer[1024];\
    glGetProgramiv(id, GL_LINK_STATUS, &success);\
    if (!success) {\
        glGetProgramInfoLog(id, 1024, NULL, buffer);\
        fprintf(stderr, "Program linkage error\n%s\n", buffer);\
        exit(-1);\
    }\
}

typedef struct {
    u64 shader_id;
} shader_t;

i64 load_shader(char* shaderSrc, int shader_type, shader_t* out_shader);

u64 create_program( shader_t* vertex, shader_t* fragment);

void use_program(u64 program);

i64 get_shader_param(u64 program_id, const char* name);
i64 set_shader_param_float(i64 uniform_loc, float value);
i64 set_shader_param_vec2(i64 uniform_loc, vec2 value);
i64 set_shader_param_vec3(i64 uniform_loc, vec3 value);
i64 set_shader_param_vec4(i64 uniform_loc, vec4 value);
i64 set_shader_param_mat3(i64 uniform_loc, mat3 value);
i64 set_shader_param_mat4(i64 uniform_loc, mat4 value);