#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform float time;
uniform mat4 transform;
out vec2 TexCoord;

void main()
{
    float det_x = sin(time);
    gl_Position = transform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}