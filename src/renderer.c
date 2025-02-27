#include "renderer.h"
#include "util.h"
#include "dynlist.h"

renderer_t init_renderer(render_config_t config) {
    renderer_t ctx;
	memset(&ctx, 0, sizeof(renderer_t));
    
    // Load vertex shader
    int res = load_shader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER, &ctx.vertex_shader);
    engine_assert(res >= 0);
    
    // Load fragment shader
    res = load_shader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, &ctx.fragment_shader);
    engine_assert(res >= 0);

    // Create the program
    ctx.program_id = create_program(&ctx.vertex_shader, &ctx.fragment_shader);

    // setup projection matrix
    glm_ortho(0, config.w, config.h, 0, config.near_plane, config.far_plane, ctx.projection);

    // setup model matrix
    glm_mat4_identity(ctx.model);

    // Cache the uniform location
    ctx.projection_uniform_loc = get_shader_param(ctx.program_id, "projection");
    ctx.model_uniform_loc = get_shader_param(ctx.program_id, "model");

    return ctx;
}

void push_sprite(renderer_t* ctx, sprite_t* sprite)
{
    if (ctx->sprite_entry_idx >= MAX_SPRITE_ENTRY) {
		log_error("Max sprite entry reached. Cannot add more sprites\n");
        return;
    }
	ctx->sprite_resource_arr[ctx->sprite_entry_idx++] = sprite;
}

void destroy_sprite(renderer_t *renderer, sprite_t *sprite) {
    for (size_t i = 0; i < renderer->sprite_entry_idx; i++) {
        sprite_t* s = renderer->sprite_resource_arr[i];

        if (strcmp(s->name, sprite->name) == 0) {
            log_info("Destroying sprite: %s\n", s->name);

            // if not last element
            if (i < renderer->sprite_entry_idx - 1) {

                // Swap with last sprite
                sprite_t* last_sprite = renderer->sprite_resource_arr[renderer->sprite_entry_idx - 1];
                log_info("Swapping with last sprite: %s\n", last_sprite->name);
                renderer->sprite_resource_arr[renderer->sprite_entry_idx - 1] = s;
                renderer->sprite_resource_arr[i] = last_sprite;
    
                // Free the last sprite
                sprite_t* updated_last_sprite = renderer->sprite_resource_arr[renderer->sprite_entry_idx - 1];
                log_info("Last sprite updated to: %s\n", updated_last_sprite->name);
                free(updated_last_sprite);
                updated_last_sprite = NULL;
            } else {
                free(s);
                s = NULL;
            }
            renderer->sprite_entry_idx--;
            break;
        }
    }
}

void renderer_update(renderer_t *ctx) {
    set_shader_param_mat4(ctx->projection_uniform_loc, ctx->projection);

    for (size_t i = 0; i < ctx->sprite_entry_idx; i++) {
		sprite_t* sprite = ctx->sprite_resource_arr[i];
        if (sprite != NULL && sprite->vao > 0) {
            // translate rotate and set scale
            glm_mat4_identity(ctx->model);
            glm_translate(ctx->model, sprite->transform.position);
            glm_rotate(ctx->model, glm_rad(sprite->transform.angle), (vec3) { 0, 0, 1});
            glm_scale(ctx->model, sprite->transform.scale);
            // set model matrix
            set_shader_param_mat4(ctx->model_uniform_loc, ctx->model);
            // set color
            i64 colorUniform = get_shader_param(ctx->program_id, "u_Color");
            set_shader_param_vec4(colorUniform, sprite->color);
            // draw the sprite
            draw_sprite(sprite);
        }
    }
}

void cleanup_renderer(renderer_t* renderer) {
    for (size_t i = 0; i < renderer->sprite_entry_idx; i++) {
        sprite_t* sprite_ref = renderer->sprite_resource_arr[i];
        if (sprite_ref != NULL) {
            free(sprite_ref);
            sprite_ref = NULL;
        }
    }

    // Delete the shader program
    glDeleteProgram(renderer->program_id);
}

void draw_sprite(sprite_t* sprite) {
    glBindVertexArray(sprite->vao);
    glBindTexture(GL_TEXTURE_2D, sprite->texture_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}