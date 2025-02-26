#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform vec4 u_Color;

out vec4 Color;
out vec2 TexCoord;

void main() 
{
    gl_Position = projection * model * vec4(aPos, 1.0);
    Color = u_Color;
    TexCoord = aTexCoord;
};