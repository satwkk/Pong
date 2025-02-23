#include "shader.h"

int load_shader(char* shaderSrc, int shader_type, shader_t* out_shader) {
    char* v_src = read_file_contents(shaderSrc);
    if (v_src == NULL) {
        return SHADER_LOAD_SUCCESS;
    }
    out_shader->shader_id = glCreateShader(shader_type);
    glShaderSource(out_shader->shader_id, 1, &v_src, NULL);
    glCompileShader(out_shader->shader_id);
    SHADER_COMPILE_CHECK(out_shader->shader_id, vertex);
    free(v_src);
    return SHADER_LOAD_FAILED;
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

int set_shader_param_float(u64 program, const char *name, float value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform1f(loc, value);
        return 0;
    }
    return -1;
}

int set_shader_param_vec2(u64 program, const char *name, vec2 value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform2f(loc, value[0], value[1]);
        return 0;
    }
    return -1;
}

int set_shader_param_vec3(u64 program, const char *name, vec3 value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform3f(loc, value[0], value[1], value[2]);
        return 0;
    }
    return -1;
}

int set_shader_param_vec4(u64 program, const char *name, vec4 value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniform4f(loc, value[0], value[1], value[2], value[3]);
        return 0;
    }
    return -1;
}

int set_shader_param_mat3(u64 program, const char *name, mat3 value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat*)value);
        return 0;
    }
    return -1;
}

int set_shader_param_mat4(u64 program, const char *name, mat4 value) {
    i64 loc = glGetUniformLocation(program, name);
    if (loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)value);
        return 0;
    }
    return -1;
}

char* read_file_contents(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    size_t read_size = fread(buffer, 1, size, fp);
    fclose(fp);
    buffer[read_size] = '\0';
    return buffer;
}