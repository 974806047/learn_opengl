#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
uniform float time;
uniform int scale_y;
out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    float det_x = sin(time);
    gl_Position = vec4(aPos.x + det_x, scale_y * aPos.y, aPos.z, 1.0);
    // gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    // TexCoord = aTexCoord;
    TexCoord =  vec2(1-aTexCoord.x, aTexCoord.y); // 笑脸翻转
}