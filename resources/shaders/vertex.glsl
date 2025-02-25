#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;

out vec4 Color;
out vec2 TexCoord;

void main() {
    gl_Position = projection * model * vec4(aPos, 1.0);
    // Color = aColor;
    Color = vec4(1.0, 1.0, 1.0, 1.0);
    TexCoord = aTexCoord;
};