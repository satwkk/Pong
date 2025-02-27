#include "sprite.h"
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"

sprite_t* create_sprite(const char* name, vec4 color, const char* path) {
    // todo: dynamically allocate the sprite
    sprite_t* sprite = (sprite_t*)malloc(sizeof(sprite_t));

    sprite->name = name;

    glm_vec3_make((vec3){10, 10, 0}, sprite->transform.position);
    glm_vec3_make((vec3){50, 50, 1}, sprite->transform.scale);
    glm_vec4_make(color, sprite->color);

    float vertices[] = {
        // vertices           // texture coords
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f
    };

    u64 indices[] = {
        0, 1, 2,
        1, 3, 2
    };
    
    // Generate buffers
    glGenBuffers(1, &sprite->vbo);
    glGenBuffers(1, &sprite->ibo);
    glGenVertexArrays(1, &sprite->vao);

    // Bind vao
    glBindVertexArray(sprite->vao);

    // Setup buffer data
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setup index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Bind vao pointers
    glEnableVertexAttribArray(0); // vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(1); // tex coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)12);

    // unbind the vao
    glBindVertexArray(0);

    // Bind texture
    engine_assert(bind_texture(path, sprite) >= 0);

    log_info("Created sprite: %s | Address: %p | VAO: %d\n", sprite->name, (void*)&sprite, sprite->vao);

    return sprite;
}

int bind_texture(const char* path, sprite_t* sprite) {
    unsigned char* data = NULL;
    int width = 0, height = 0, channels = 0;

    if (path == NULL) {
        log_info("NULL Path provided, getting the default texture for sprite %s\n", sprite->name);

        width = height = 1;
        channels = 4;
        data = (unsigned char*)malloc(width * height * channels);

        data[0] = 255; //r 
        data[1] = 255; //g
        data[2] = 255; //b
        data[3] = 255; //a
    } else {
        data = stbi_load(path, &width, &height, &channels, 0);
    }

    if (data == NULL) {
        log_error("Could not load data from image: %s\n", path);
        stbi_image_free(data);
        return -1;
    }

    glGenTextures(1, &sprite->texture_id);

    if (sprite->texture_id == 0) {
        log_error("Failed to generate OpenGL texture for sprite: %s\n", sprite->name);
        stbi_image_free(data); 
        return -1;
    }

    log_info("Channels for sprite %s: %d\n", sprite->name, channels);

    glBindTexture(GL_TEXTURE_2D, sprite->texture_id);
    unsigned int format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data); 
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

void set_position(sprite_t* sprite, vec3 position) {
    glm_vec3_make((const float*)position, sprite->transform.position);
}

void set_rotation(sprite_t *sprite, float angle) {
    sprite->transform.angle = angle;
}

void set_scale(sprite_t* sprite, vec3 scale) {
    glm_vec3_make((const float*)scale, sprite->transform.scale);
}