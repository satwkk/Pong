#include <stdio.h>
#include "defines.h"
#include "resource.h"
#include "shader.h"
#include "sprite.h"
#include "renderer.h"

typedef struct {
    GLFWwindow* win_handle;
    resource_container resources;
    renderer_t renderer;
} game_context;

int main() {
    // Initialize GLFW
    int glfw_err = glfwInit();
    if (glfw_err < 0) {
        fprintf(stderr, "GLFW initialization failed: %s\n", glewGetErrorString(glfw_err));
        return -1;
    }

    // Setup GLFW config
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    if (!window) {
        fprintf(stderr, "window initialization failed\n");
        return -1;
    }

    // Create the window context
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // This should be done after creating the OpenGL context
    GLenum glew_err = glewInit();
    if (glew_err != GLEW_OK) {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(glew_err));
        return -1;
    }

    // Setup global game context
    game_context ctx;
    ctx.win_handle = window;
    int res = load_resources(&ctx.resources);
    if (res < 0) {
        fprintf(stderr, "Resource loading failed\n");
        return -1;
    }

    // TODO: Refactoring reminder
    // Resources should contain all the loaded sprites
    // get_render_data will return an array of sprite data to renderer
    // shader compiling and stuff will be moved into renderer
    ctx.renderer = init_renderer((renderer_data_t) {
        .w = 800,
        .h = 600,
        .near_plane = -1,
        .far_plane = 1,
        .program_id = ctx.resources.program_id
    });

    // TODO: These should be moved into resource loader
    sprite_t sprite = create_sprite("red box", NULL);
    set_position(&sprite.transform, (vec3) { 100, 300, 0 });

    sprite_t sprite_2 = create_sprite("blue box", NULL);
    set_position(&sprite_2.transform, (vec3) { 100, 100, 0 });

    // set the game context to main window context's user data
    glfwSetWindowUserPointer(window, &ctx);

    // update loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        float red = sin(glfwGetTime());

        use_program(ctx.resources.program_id);

        renderer_update(&ctx.renderer, &sprite);
        renderer_update(&ctx.renderer, &sprite_2);

        // color test
        set_shader_param_vec4(ctx.resources.program_id, "u_Color", (vec4){ red, 1.0, 0.0, 1.0 });

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