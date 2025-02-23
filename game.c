#include <stdio.h>
#include "defines.h"
#include "resource.h"
#include "shader.h"
#include "sprite.h"

typedef struct {
    GLFWwindow* win_handle;
    resource_container resources;
} game_context;

int main() {
    int glfw_err = glfwInit();
    if (glfw_err < 0) {
        fprintf(stderr, "GLFW initialization failed: %s\n", glewGetErrorString(glfw_err));
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    if (!window) {
        fprintf(stderr, "window initialization failed\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum glew_err = glewInit();

    if (glew_err != GLEW_OK) {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(glew_err));
        return -1;
    }

    // Setup game context
    game_context ctx;
    ctx.win_handle = window;
    int res = load_resources(&ctx.resources);

    if (res < 0) {
        fprintf(stderr, "Resource loading failed\n");
        return -1;
    }

    sprite_t sprite = create_sprite("red box", NULL);

    mat4 projection;
    glm_ortho(0, 800, 600, 0, -1, 1, projection);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){500, 400, 0.0});
    glm_scale(model, (vec3){50.0, 50.0, 1.0});

    set_shader_param_mat4(ctx.resources.program_id, "projection", projection);
    set_shader_param_mat4(ctx.resources.program_id, "model", model);

    // update loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        float red = sin(glfwGetTime());

        use_program(ctx.resources.program_id);
        set_shader_param_vec4(ctx.resources.program_id, "u_Color", (vec4){ red, 1.0, 0.0, 1.0 });
        draw_sprite(&sprite);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    cleanup_resources(&ctx.resources);
    glDeleteVertexArrays(1, &sprite.vao);
    glDeleteBuffers(1, &sprite.vbo);
    glDeleteBuffers(1, &sprite.ibo);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}