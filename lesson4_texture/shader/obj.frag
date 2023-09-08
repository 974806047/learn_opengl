#version 330 core
out vec4 FragColor;

uniform float time;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
// uniform sampler2D texture2;

void main()
{
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(ourColor, 1.0);
    // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // GL_CLAMP_TO_BORDER 时用这个
    // FragColor = texture(texture1, TexCoord);
    float scale = (sin(time) + 1) / 2;
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), scale) * vec4(ourColor, 1.0);
}