#include "game.h"
#include "sprite.h"
#include "util.h"
#include "renderer.h"
#include "colors.h"
#include <stdbool.h>
#include "shader.h"

void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void game_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
}

int init_game(game_context* ctx) {
    gameState.ballSpeed = BALLSPEED;
    gameState.playerSpeed = PLAYERSPEED;
    gameState.speedIncreaseInterval = 10.0;
    gameState.lastTickTimer = 0.0;
    gameState.cameraShakeDuration = 0.2;
    gameState.cameraShakeTimer = gameState.cameraShakeDuration;
    
    // Create the sprites
    gameState.leftPlayer = create_sprite("Left Player", TRANSPARENT, "./resources/images/sci_fi_paddle.jpg");
    set_position(gameState.leftPlayer, (vec3){ 10, (float)ctx->win_h / 2, 1});
    set_scale(gameState.leftPlayer, (vec3){ 10, 90, 1});
    
    gameState.rightPlayer = create_sprite("Right Player", TRANSPARENT, "./resources/images/sci_fi_paddle.jpg");
    set_position(gameState.rightPlayer, (vec3){ ctx->win_w - 10, (float)ctx->win_h / 2, 1});
    set_scale(gameState.rightPlayer, (vec3){ 10, 90, 1});
    
    gameState.ball = create_sprite("Ball", TRANSPARENT, "./resources/images/sci_fi_ball.png");
    set_position(gameState.ball, (vec3){ 50, 50, 1});
    set_scale(gameState.ball, (vec3){30, 30, -1});
    
    gameState.background = create_sprite("Background", TRANSPARENT, "./resources/images/sci_fi_bg_1.png");
    set_position(gameState.background, (vec3){ ctx->win_w / 2, ctx->win_h / 2, -0.9} );
    set_scale(gameState.background, (vec3){ ctx->win_w, ctx->win_h, 1 });
    
    // TODO: Make these sprites automatically submit calls to renderer
    push_sprite(&ctx->renderer, gameState.background);
    push_sprite(&ctx->renderer, gameState.leftPlayer);
    push_sprite(&ctx->renderer, gameState.rightPlayer);
    push_sprite(&ctx->renderer, gameState.ball);

    glm_vec3_make(vec3_left, gameState.ballDir);

    return 0;
}

void update_game(game_context* ctx, const float dt) {
    // Update the player
    {
        update_player(ctx);
    }

    // Update the ball
    {
        update_ball(ctx);
    }

    // Update ball speed
    {
        update_ball_speed();
    }

    // camera shake
    {
        if (gameState.bStartCameraShake) {
            if (gameState.cameraShakeTimer <= 0) {
                gameState.bStartCameraShake = false;
                gameState.cameraShakeTimer = gameState.cameraShakeDuration;
            }
            i64 shake_timer = get_shader_param(ctx->renderer.program_id, "u_ShakeTimer");
            set_shader_param_float(shake_timer, gameState.cameraShakeTimer);
            gameState.cameraShakeTimer -= dt;
        }
    }
}

void update_player(game_context* ctx) {
    // left bat move up
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_W) && gameState.leftPlayer->transform.position[1] - (gameState.leftPlayer->transform.scale[1] / 2) > 0) 
        move(gameState.leftPlayer, vec3_up, gameState.playerSpeed);
    
    // left bat move down
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_S) && gameState.leftPlayer->transform.position[1] + (gameState.leftPlayer->transform.scale[1] / 2) < ctx->win_h) 
        move(gameState.leftPlayer, vec3_down, gameState.playerSpeed);
    
    // right bat move up
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_UP) && gameState.rightPlayer->transform.position[1] - (gameState.rightPlayer->transform.scale[1] / 2) > 0) 
        move(gameState.rightPlayer, vec3_up, gameState.playerSpeed);
    
    // right bat move down
    if (glfwGetKey(ctx->win_handle, GLFW_KEY_DOWN) && gameState.rightPlayer->transform.position[1] + (gameState.rightPlayer->transform.scale[1] / 2) < ctx->win_h) 
        move(gameState.rightPlayer, vec3_down, gameState.playerSpeed);
}

void update_ball_speed() {
    float time = glfwGetTime();
    if ((time - gameState.lastTickTimer) >= gameState.speedIncreaseInterval) {
        gameState.ballSpeed += (gameState.ballSpeed * 0.1);  // 10 percent increase
        gameState.lastTickTimer = time;
        log_info("New ball speed: %f\n", gameState.ballSpeed);
    }
}

void update_ball(game_context* ctx)  {
    // rotate
    {
        gameState.ball->transform.angle += 1.0;

        if (gameState.ball->transform.angle >= 360.0) {
            gameState.ball->transform.angle -= 360.0;
        }
        
    }
    // bat hit detection
    {
        // left
        if (is_ball_in_range_to_hit(gameState.leftPlayer) && gameState.ball->transform.position[0] < gameState.leftPlayer->transform.position[0] + gameState.leftPlayer->transform.scale[0])  {
            on_player_hit(gameState.ball, gameState.leftPlayer->transform.position, gameState.ballDir);
        }
        
        // right
        if (is_ball_in_range_to_hit(gameState.rightPlayer) && gameState.ball->transform.position[0] + gameState.ball->transform.scale[0] > gameState.rightPlayer->transform.position[0]) {
            on_player_hit(gameState.ball, gameState.rightPlayer->transform.position, gameState.ballDir);
        }
    }
    
    // window edge hit detection
    {
        // up
        if (gameState.ball->transform.position[1] < 0) {
            on_border_hit((vec3){ 0, 1, 0}, gameState.ballDir);
        }
        
        // down
        if (gameState.ball->transform.position[1] > ctx->win_h) {
            on_border_hit((vec3){ 0, -1, 0}, gameState.ballDir);
        }
    }

    // Reset position if out of bounds
    {
        // if (gameState.ball->transform.position[0] < 0 || gameState.ball->transform.position[0] > ctx->win_w) {
        //     set_position(gameState.ball, (vec3){ ctx->win_w / 2, ctx->win_h / 2, 1 });
        //     glm_vec3_make(vec3_right, gameState.ballDir);
        // }
    }

    move(gameState.ball, gameState.ballDir, gameState.ballSpeed);
}

void move(sprite_t* player, vec3 dir, float speed) {
    vec3 velocity;
    glm_vec3_scale(dir, speed, velocity);
    glm_vec3_add(player->transform.position, velocity, player->transform.position);
}

bool is_ball_in_range_to_hit(sprite_t* player) {
    return gameState.ball->transform.position[1] > (player->transform.position[1] - (player->transform.scale[1] / 2)) &&
    gameState.ball->transform.position[1] < (player->transform.position[1] + (player->transform.scale[1] / 2));
}

void on_player_hit(sprite_t* ball, vec3 hitPoint, vec3 direction) {
    gameState.bStartCameraShake = true;

    float dx = ball->transform.position[0] - hitPoint[0];
    float dy = ball->transform.position[1] - hitPoint[1];

    float mx = sqrt(dx * dx + dy * dy);

    float nx = dx / mx;
    float ny = dy / mx;

    glm_vec3_make((vec3){nx, ny, 0}, direction);
}

void on_border_hit(vec3 normal, vec3 newDir) {
    glm_vec3_reflect(gameState.ballDir, normal, newDir);
}