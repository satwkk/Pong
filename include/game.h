#pragma once

#include "pch.h"
#include "gamecontext.h"

#define BALLSPEED 3
#define PLAYERSPEED 2

typedef struct {
    sprite_t leftPlayer;
    sprite_t rightPlayer    ;
    sprite_t ball;
    vec3 initialVelocity;
    vec3 ballVelocity;
    vec3 ballDir;
    float ballSpeed;
    float playerSpeed;
} game_state_t;

static game_state_t gameState;

// INITIALIZATION
int init_game(game_context* ctx);

// UPDATE LOOPS
void update_player(game_context* ctx);
void update_ball(game_context* ctx);
void update_game(game_context* ctx);

// CALLBACKS
void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void game_mouse_callback(GLFWwindow* window, int button, int action, int mods);

// PLAYER MOVEMENT
void move(sprite_t* player, vec3 dir, float speed);

// COLLISION EVENTS
bool is_ball_in_range_to_hit(sprite_t* player);
void on_player_hit(sprite_t* ball, vec3 hitPoint, vec3 direction);
void on_border_hit(vec3 normal, vec3 newDir);