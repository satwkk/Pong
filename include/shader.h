#pragma once

#include "pch.h"

#define SHADER_LOAD_SUCCESS 0
#define SHADER_LOAD_FAILED -1

#define VERTEXSHADERPATH "./resources/shaders/vertex.shader"
#define FRAGMENTSHADERPATH "./resources/shaders/fragment.shader"

#define SHADER_COMPILE_CHECK(id, shader_name) \
{\
    int success = 0;\
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
    int success = 0;\
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

int load_shader(char* shaderSrc, int shader_type, shader_t* out_shader);

char* read_file_contents(const char*);

u64 create_program(
    shader_t* vertex, 
    shader_t* fragment
    /** Add more here */
);

void use_program(u64 program);

int set_shader_param_float(u64 program, const char* name, float value);
int set_shader_param_vec2(u64 program, const char* name, vec2 value);
int set_shader_param_vec3(u64 program, const char* name, vec3 value);
int set_shader_param_vec4(u64 program, const char* name, vec4 value);
int set_shader_param_mat3(u64 program, const char* name, mat3 value);
int set_shader_param_mat4(u64 program, const char* name, mat4 value);