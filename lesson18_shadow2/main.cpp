#include <glad/glad.h>  //确保glad文件在glfw之前
#include <GLFW\glfw3.h>
#include "shader.h"
#include <string>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
char * get_path(std::string name);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int read_image(std::string name, GLint internalformat, GLenum type);
void DisplayFramebufferTexture(GLuint textureID);
// 初始数据
int WIDTH = 800;
int HEIGHT = 600;
float deltaTime = 0.0f;	// 每帧间隔
float lastFrame = 0.0f; // 上次点击时间
bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
float del_x = 0;
bool handle_mouse_move = false;
GLboolean shadows = true;
// meshes
unsigned int planeVAO;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
struct InitData
{
    GLFWwindow* window;
};
struct GLBufferData{
    unsigned int VAO, VBO, EBO;
};

// 初始化VAO 这些东西
GLBufferData initGLBuffer(float vertices[], int v_num, bool containCoord){
    // 创建VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 创建VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 1. 绑定顶点数组对象
    glBindVertexArray(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v_num, vertices, GL_STATIC_DRAW);
    // 4. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置法向量
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    GLBufferData data = GLBufferData();
    data.VAO = VAO;
    data.VBO = VBO;
    return data;
}
// 创建context window这些
GLFWwindow* initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //   明确指定当前使用的是核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw "Failed to create GLFW window";
    }
    glfwMakeContextCurrent(window);
    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw "Failed to initialize GLAD";
    }
    // 窗口大小变化时修改视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback); // 鼠标位置回调
    glfwSetScrollCallback(window, scroll_callback); // 滚轮回调
    try
    {
        /* code */
        // imgui初始化
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return window;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void RenderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// renders the 3D scene
// --------------------
void RenderScene(const Shader &shader)
{
    // Room cube
    glm::mat4 model;
    model = glm::scale(model, glm::vec3(10.0));
    shader.setMat4("model", model);
    glDisable(GL_CULL_FACE); // Note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    
    shader.setInt("reverse_normals", 1);
    RenderCube();
    shader.setInt("reverse_normals", 0);
    glEnable(GL_CULL_FACE);
    // Cubes
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(1.5));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(1.5));
    shader.setMat4("model", model);
    RenderCube();
}

void testCase1() {
    GLFWwindow* window = initWindow();
    glm::vec3 lightColor = glm::vec3(1.0f);
    // GLBufferData objBufferData = initGLBuffer(, 36 * 5, true); // 加载数据
    // 创建并使用程序对象
    Shader depthShader("./shader/depth.vert", "./shader/depth.frag", "./shader/depth.gs");
    Shader shadowShader("./shader/shadow.vert", "./shader/shadow.frag");
    unsigned int woodTexture = read_image("./image/wood.png", GL_RGB, GL_RGB);
    
    std::cout<< 113 << std::endl;
    // unsigned int marble = read_image("./image/marble.jpg", GL_RGB, GL_RGB); // 加载图片
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 使用线框模式
    float time = 0;
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_ALWAYS); // 深度测试永远通过
    bool show_imgui_window = true;
    float aspect = (float)WIDTH / (float)HEIGHT;
    shadowShader.use();
    shadowShader.setInt("diffuseTexture", 0);
    shadowShader.setInt("shadowMap", 1);

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
    // GLuint depthMap = CreateDepthMapFBO();
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // 生成立方体贴图
    GLuint depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (GLuint i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Attach cubemap as depth map FBO's color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    std::cout<< 114 << std::endl;
    // objShader.setInt("texture1", 0);
    // 循环渲染
    while(!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        // 更新deltaTime
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
#pragma region imgui相关
        // imgui窗口
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (show_imgui_window){
            ImGui::Begin("config", &show_imgui_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::SliderFloat("fov", &camera.Zoom, 20.0f, 90.0f);  
            ImGui::SliderFloat("aspect", &aspect, 0.5f, 2.0f);
            ImGui::Checkbox("p handle_mouse_move", &handle_mouse_move);
            if (ImGui::Button("Close Me"))
                show_imgui_window = false;
            ImGui::End();
        }
        ImGui::Render();
#pragma endregion
        
        DisplayFramebufferTexture(woodTexture);

        // // 光空间变换矩阵
        // GLfloat near1 = 1.0f;
        // GLfloat far1 = 25.0f;
        // glm::mat4 shadowProj = glm::perspective(90.0f, aspect, near1, far1);
        // std::vector<glm::mat4> shadowTransforms;
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
        // shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));

        // // 1. Render scene to depth cubemap
        // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        // glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        // glClear(GL_DEPTH_BUFFER_BIT);
        // depthShader.use();
        // for (GLuint i = 0; i < 6; ++i)
        //     glUniformMatrix4fv(glGetUniformLocation(depthShader.ID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
        // glUniform1f(glGetUniformLocation(depthShader.ID, "far_plane"), far1);
        // glUniform3fv(glGetUniformLocation(depthShader.ID, "lightPos"), 1, &lightPos[0]);
        // RenderScene(depthShader);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // // 2. Render scene as normal 
        // glViewport(0, 0, WIDTH, HEIGHT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // shadowShader.use();
        // glm::mat4 projection = glm::perspective(camera.Zoom, aspect, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // glUniformMatrix4fv(glGetUniformLocation(shadowShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // // Set light uniforms
        // glUniform3fv(glGetUniformLocation(shadowShader.ID, "lightPos"), 1, &lightPos[0]);
        // glUniform3fv(glGetUniformLocation(shadowShader.ID, "viewPos"), 1, &camera.Position[0]);
        // // Enable/Disable shadows by pressing 'SPACE'
        // glUniform1i(glGetUniformLocation(shadowShader.ID, "shadows"), shadows);
        // glUniform1f(glGetUniformLocation(shadowShader.ID, "far_plane"), far1);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, woodTexture);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        // RenderScene(shadowShader);

        // 清除颜色缓冲
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // 双循环渲染
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    // glDeleteVertexArrays(1, &objBufferData.VAO);
    // glDeleteBuffers(1, &objBufferData.VBO);
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int argc, char* argv[])
{
    // testCase1();
    testCase1();

    // glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

unsigned int read_image(std::string name, GLint internalformat, GLenum type){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    
    stbi_set_flip_vertically_on_load(true);
    char * image_path = get_path(name);
    
    unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 0);
    
    try
    {
        /* code */
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, type, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    stbi_image_free(data);
    return texture;
}
char * get_path(std::string name){
    // 使用exe的路径来读取shader
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    /*
    strrchr:函数功能：查找一个字符c在另一个字符串str中末次出现的位置（也就是从str的右侧开始查找字符c首次出现的位置），
    并返回这个位置的地址。如果未能找到指定字符，那么函数将返回NULL。
    使用这个地址返回从最后一个字符c到str末尾的字符串。                                                                  
    */
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串//
    std::string path = szFilePath;
    path += name;
    return const_cast<char*>(path.c_str());
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        handle_mouse_move = !handle_mouse_move;
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        camera.Focus(glm::vec3(0.0f));
    }

    // 相机移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if(!handle_mouse_move){
        return;
    }
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
bool notInitialized = true;
Shader debugShader;
InitData debugData;
unsigned int debugVAO, debugVBO, debugEBO;
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