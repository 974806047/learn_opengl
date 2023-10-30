#version 330 core
out vec4 FragColor;

uniform float time;
uniform vec3 lightColor;
in vec2 TexCoord;

void main()
{
    FragColor = vec4(lightColor, 1.0f);
}