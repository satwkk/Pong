#include "sprite.h"

sprite_t create_sprite(const char* name, const char* path) {
    sprite_t sprite;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f, 0.5f, 0.0f
    };

    u64 indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    // Generate buffers
    glGenBuffers(1, &sprite.vbo);
    glGenBuffers(1, &sprite.ibo);
    glGenVertexArrays(1, &sprite.vao);

    // Bind vao
    glBindVertexArray(sprite.vao);

    // Setup buffer data
    glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setup index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Bind vao pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the vao
    glBindVertexArray(0);

    return sprite;
}

void draw_sprite(sprite_t* sprite) {
    glBindVertexArray(sprite->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}