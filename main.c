#include <stdio.h>
#include "defines.h"
#include "shader.h"
#include "sprite.h"
#include "util.h"
#include "game.h"

#define WIDTH 1024
#define HEIGHT 768

int main() {
    // Initialize GLFW
    int glfw_err = glfwInit();
    engine_assert(glfw_err >= 0);

    // Setup GLFW config
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello world", NULL, NULL);
    engine_assert(window != NULL);

    // Create the window context
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    // This should be done after creating the OpenGL context
    GLenum glew_err = glewInit();
    engine_assert(glew_err == GLEW_OK);

    // Setup global game context
    game_context ctx;
    ctx.win_handle = window;
    ctx.win_w = WIDTH;
    ctx.win_h = HEIGHT;

    // Load all the resources for the game. 
    // NOTE: THIS SHOULD HAPPEN BEFORE RENDERER IS INITIALIZED
    int res = load_resources(&ctx.resources);
    engine_assert(res >= 0);

    // Initialize the renderer with required data
    ctx.renderer = init_renderer((render_config_t) {
        .w = WIDTH,
        .h = HEIGHT,
        .near_plane = -1,
        .far_plane = 1
    });

    // set the game context to main window context's user data
    glfwSetWindowUserPointer(window, &ctx);

    // set key callback for the game
    glfwSetKeyCallback(window, game_key_callback);
    glfwSetMouseButtonCallback(window, game_mouse_callback);

    // Initialize the game
    int init_game_res = init_game(&ctx);
    engine_assert(init_game_res >= 0);

    u64 color_uniform_loc = get_shader_param(ctx.renderer.program_id, "u_Color");

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    // update loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float red = sin(glfwGetTime() * 3.0);

        // ============================ GAME UPDATE =====================================

        double currentFrameTime = glfwGetTime();
        ctx.deltaSeconds = currentFrameTime - ctx.lastFrameTime;
        ctx.lastFrameTime = currentFrameTime;

        update_game(&ctx, ctx.deltaSeconds);

        // ==============================================================================

        // ============================ RENDER UPDATE ===================================
        renderer_update(&ctx.renderer);
        set_shader_param_vec4(color_uniform_loc, (vec4){ red, 1.0, 0.0, 1.0 });

        // ==============================================================================

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    cleanup_renderer(&ctx.renderer);
    cleanup_resources(&ctx.resources);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}