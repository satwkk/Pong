#include "shader.h"
#include "util.h"

i64 load_shader(char* shaderSrc, int shader_type, shader_t* out_shader) {
    char* v_src = read_file_contents(shaderSrc);
    if (v_src == NULL) {
        return SHADER_LOAD_FAILED;
    }
    out_shader->shader_id = glCreateShader(shader_type);
    glShaderSource(out_shader->shader_id, 1, &v_src, NULL);
    glCompileShader(out_shader->shader_id);
    SHADER_COMPILE_CHECK(out_shader->shader_id, vertex);
    free(v_src);
    return SHADER_LOAD_SUCCESS;
}

u64 create_program(shader_t* vertex, shader_t* fragment) {
    u64 program_id = glCreateProgram();
    glAttachShader(program_id, vertex->shader_id);
    glAttachShader(program_id, fragment->shader_id);
    glLinkProgram(program_id);
    PROGRAM_LINK_CHECK(program_id);
    glDeleteShader(vertex->shader_id);
    glDeleteShader(fragment->shader_id);
    glUseProgram(program_id);
    return program_id;
}

void use_program(u64 program) {
    glUseProgram(program);
}

i64 get_shader_param(u64 program_id, const char *name) {
    return glGetUniformLocation(program_id, name);
}

i64 set_shader_param_float(i64 uniform_loc, float value) {
    if (uniform_loc >= 0) {
        glUniform1f(uniform_loc, value);
        return 0;
    }
    return -1;
}

i64 set_shader_param_vec2(i64 uniform_loc, vec2 value) {
    if (uniform_loc >= 0) {
        glUniform2f(uniform_loc, value[0], value[1]);
        return 0;
    }
    return -1;
}

i64 set_shader_param_vec3(i64 uniform_loc, vec3 value) {
    if (uniform_loc >= 0) {
        glUniform3f(uniform_loc, value[0], value[1], value[2]);
        return 0;
    }
    return -1;
}

i64 set_shader_param_vec4(i64 uniform_loc, vec4 value) {
    if (uniform_loc >= 0) {
        glUniform4f(uniform_loc, value[0], value[1], value[2], value[3]);
        return 0;
    }
    return -1;
}

i64 set_shader_param_mat3(i64 uniform_loc, mat3 value) {
    if (uniform_loc >= 0) {
        glUniformMatrix3fv(uniform_loc, 1, GL_FALSE, (const GLfloat*)value);
        return 0;
    }
    return -1;
}

i64 set_shader_param_mat4(i64 uniform_loc, mat4 value) {
    if (uniform_loc >= 0) {
        glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, (const GLfloat*)value);
        return 0;
    }
    return -1;
}
