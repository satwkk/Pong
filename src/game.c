
#include "game.h"
#include "sprite.h"
#include "util.h"
#include "renderer.h"

void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void game_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    printf("Key pressed: %d\n", button);
}

int init_game(game_context* ctx) {
    int width = 0, height = 0;
    glfwGetWindowSize(ctx->win_handle, &width, &height);

    // Create the player sprites
    left_player = create_sprite("Left Player", "./resources/images/ball_texture.jpg");
    log_info("Instantiate sprite: %s | Address: %p | VAO: %d\n", left_player.name, (void*)&left_player, left_player.vao);
    right_player = create_sprite("Right Player", NULL);

    // Set player initial positions
    set_position(&left_player, (vec3){ 5, (float)height / 2, 1});
    set_scale(&left_player, (vec3){ 10, 50, 1});
    set_position(&right_player, (vec3){ width - 5, (float)height / 2, 1});
    set_scale(&right_player, (vec3){ 10, 50, 1});

    // TODO: Make these sprites automatically submit calls to renderer
    push_sprite(&ctx->renderer, &left_player);
    push_sprite(&ctx->renderer, &right_player);

    return 0;
}

void poll_input()
{
}

void move(sprite_t* player, vec3 dir, float speed) {
    vec3 velocity;
    glm_vec3_scale(dir, speed, velocity);
    glm_vec3_add(player->transform.position, velocity, player->transform.position);
}

void update_game(game_context* ctx) {
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_S)) {
        move(&left_player, vec3_up, move_speed);
    }
    
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_W)) {
        move(&left_player, vec3_down, move_speed);
    }

    if (glfwGetKey(ctx->win_handle, GLFW_KEY_DOWN)) {
        move(&right_player, vec3_up, move_speed);
    }

    if (glfwGetKey(ctx->win_handle, GLFW_KEY_UP)) {
        move(&right_player, vec3_down, move_speed);
    }

    log_info("Left player position: %f, %f\n", left_player.transform.position[0], left_player.transform.position[1]);
}