#ifndef TOOLS_H
#define TOOLS_H
#include <glad/glad.h>  //确保glad文件在glfw之前
#include <GLFW\glfw3.h>
#include <shader.h>
static bool notInitialized = true;
static Shader debugShader;
static unsigned int debugVAO, debugVBO, debugEBO;
void DisplayFramebufferTexture(GLuint textureID)
{
    if(notInitialized)
    {
        float vertices[] = {
        //     ---- 位置 ----      - 纹理坐标 -
            1.0f,  1.0f, 0.0f,   1.0f, 1.0f,   // 右上
            1.0f,  0.3f, 0.0f,   1.0f, 0.0f,   // 右下
            0.3f,  0.3f, 0.0f,   0.0f, 0.0f,   // 左下
            0.3f,  1.0f, 0.0f,   0.0f, 1.0f    // 左上
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
        // 在屏幕右上角，使用NDC顶点坐标初始化着色器和VAO
        debugShader = Shader("./shader/debug.vert", "./shader/debug.frag");
        glGenVertexArrays(1, &debugVAO);
        glGenBuffers(1, &debugVBO);
        glGenBuffers(1, &debugEBO);
        glBindVertexArray(debugVAO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // link vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
        notInitialized = false;
    }
    debugShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(debugVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
#endif