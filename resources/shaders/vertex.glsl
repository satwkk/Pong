#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform vec4 u_Color;
uniform float u_ShakeTimer;

out vec4 Color;
out vec2 TexCoord;

void main() {
    const float shakeStrength = 0.01;
    Color = u_Color;
    TexCoord = aTexCoord;
    gl_Position = projection * model * vec4(aPos, 1.0);
    gl_Position.x += cos(u_ShakeTimer * 40) * shakeStrength;
    gl_Position.y += sin(u_ShakeTimer * 50) * shakeStrength;
};