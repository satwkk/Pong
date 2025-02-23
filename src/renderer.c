#include "renderer.h"
#include "shader.h"

renderer_t init_renderer(renderer_data_t config) {
    // setup projection matrix
    renderer_t ctx;
    ctx.program_id = config.program_id;

    glm_ortho(0, config.w, config.h, 0, config.near_plane, config.far_plane, ctx.projection);

    // setup model matrix
    glm_mat4_identity(ctx.model);
    return ctx;
}

void renderer_update(renderer_t *ctx, sprite_t *sprite) {
    // translate and set scale
    glm_mat4_identity(ctx->model);
    glm_translate(ctx->model, sprite->transform.position);
    glm_scale(ctx->model, sprite->transform.scale);
    
    // set uniforms
    set_shader_param_mat4(ctx->program_id, "projection", ctx->projection);
    set_shader_param_mat4(ctx->program_id, "model", ctx->model);

    draw_sprite(sprite);
}
