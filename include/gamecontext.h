#include "pch.h"
#include "resource.h"
#include "renderer.h"

// GLOBAL STATE CONTEXT
typedef struct {
    // engine states
    GLFWwindow* win_handle;
    u64 win_h;
    u64 win_w;
    resource_container resources;
    renderer_t renderer;

    float lastFrameTime;
    float deltaSeconds;
} game_context;