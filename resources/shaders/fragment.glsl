#version 330 core

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TEXTURE01;

void main() 
{
    vec4 texColor = texture(TEXTURE01, TexCoord);
    FragColor.rgb = texColor.rgb * Color.rgb;
    FragColor.a = texColor.a * Color.a;
}