#include "resource.h"

int load_resources(resource_container* resources) {
    // Load vertex shader
    int res = load_shader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, &resources->vertex_shader);
    assert(res < 0);

    // Load fragment shader
    res = load_shader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, &resources->fragment_shader);
    assert(res < 0);

    // Create the program
    resources->program_id = create_program(&resources->vertex_shader, &resources->fragment_shader);

    return 0;
}

void cleanup_resources(resource_container* resources) {
    glDeleteProgram(resources->program_id);
}
