#include <stdio.h>
#include "cglm/cglm.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "defines.h"
#include "resource.h"
#include "sprite.h"

#define FLOAT16 \
{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    if (!window) {
        fprintf(stderr, "window initialization failed\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(err));
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f, 0.5f, 0.0f
    };

    u64 indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    // Formula = projection * model * view
    // Projection matrix
    mat4 projection;
    glm_ortho(0, 800, 600, 0, -1, 1, projection);

    mat4 model;
    glm_mat4_identity(model);

    // Model matrix
    vec3 position;
    glm_vec3_zero(position);
    position[0] = 700.0;
    position[1] = 500.0;
    glm_translate(model, position);

    glm_scale(model, (vec3){ 50.0f, 50.0f, 1.0f});

    char* vertex_shader = read_file_contents("./resources/shaders/vertex.glsl");
    char* fragment_shader = read_file_contents("./resources/shaders/fragment.glsl");

    u64 vbo, ibo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    u64 v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vertex_shader, NULL);
    glCompileShader(v);

    {
        int success = 0;
        char buffer[1024];
        glGetShaderiv(v, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(v, 1024, NULL, buffer);
            fprintf(stderr, "Vertex shader compilation error\n%s\n", buffer);
            exit(-1);
        }
    }

    u64 f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fragment_shader, NULL);
    glCompileShader(f);

    {
        int success = 0;
        char buffer[1024];
        glGetShaderiv(f, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(f, 1024, NULL, buffer);
            fprintf(stderr, "Fragment shader compilation error\n%s\n", buffer);
            exit(-1);
        }
    }


    u64 program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    {
        int success = 0;
        char buffer[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 1024, NULL, buffer);
            fprintf(stderr, "Program linkage error\n%s\n", buffer);
            exit(-1);
        }
    }

    glUseProgram(program);

    i64 proj_uni_loc = glGetUniformLocation(program, "projection");
    i64 model_uni_loc = glGetUniformLocation(program, "model");

    if (proj_uni_loc < 0 || model_uni_loc < 0) {
        fprintf(stderr, "Uniform locations could not be found\n");
        return -1;
    }

    glUniformMatrix4fv(proj_uni_loc, 1, GL_FALSE, (const GLfloat*)projection);
    glUniformMatrix4fv(model_uni_loc, 1, GL_FALSE, (const GLfloat*)model);

    glDeleteShader(v);
    glDeleteShader(f);
    free(vertex_shader);
    free(fragment_shader);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}