#pragma once

#include "pch.h"
#include "gamecontext.h"

static sprite_t left_player;
static sprite_t right_player;
static const float move_speed = 2;

int init_game(game_context* ctx);
void poll_input();
void update_game(game_context* ctx);
void game_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void game_mouse_callback(GLFWwindow* window, int button, int action, int mods);

void move(sprite_t* player, vec3 dir, float speed);
