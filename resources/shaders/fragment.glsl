#version 330 core

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TEXTURE01;

void main() {
    vec4 texColor = texture(TEXTURE01, TexCoord);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor * Color;
}