#version 330 core

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TEXTURE01;

void main() {
    FragColor = texture(TEXTURE01, TexCoord) * Color;
}