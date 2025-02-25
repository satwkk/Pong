#include "pch.h"
#include "resource.h"
#include "renderer.h"

typedef struct {
    GLFWwindow* win_handle;
    resource_container resources;
    renderer_t renderer;
} game_context;