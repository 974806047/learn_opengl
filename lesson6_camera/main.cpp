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
int WIDTH = 800;
int HEIGHT = 600;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
struct InitData
{
    GLFWwindow* window;
    unsigned int VAO, VBO, EBO;
};

InitData init(float vertices[], int v_num, unsigned int indices[], int i_num, bool containCoord){
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

    // 创建VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 创建VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // // 创建EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // 1. 绑定顶点数组对象
    glBindVertexArray(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v_num, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * i_num, indices, GL_STATIC_DRAW);
    if(containCoord) {
        // 4. 设定顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 设置颜色
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // 设置纹理坐标
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    else{

        // 4. 设定顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // 设置坐标
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    
    InitData data = InitData();
    data.window = window;
    data.VAO = VAO;
    data.VBO = VBO;
    data.EBO = EBO;
    return data;
}
float del_x = 0;
void testCase1() {
    // VAO
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    InitData initData = init(vertices, 36 * 5, indices, 6, false);
    unsigned int texture1 = read_image("./image/box.jpg", GL_RGB, GL_RGB);
    unsigned int texture2 = read_image("./image/laugh.png", GL_RGBA, GL_RGBA);
    // 创建并使用程序对象
    Shader shader("./shader/obj.vert", "./shader/obj.frag");
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 使用线框模式
    float time = 0;

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    
    // glm::mat4 view          = glm::mat4(1.0f);
    // view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection    = glm::mat4(1.0f);
    glEnable(GL_DEPTH_TEST);
    bool show_imgui_window = true;
    float fov = 45.0f;
    float aspect = (float)WIDTH / (float)HEIGHT;
    // 循环渲染
    while(!glfwWindowShouldClose(initData.window))
    {
        // 更新deltaTime
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // imgui窗口
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (show_imgui_window){
            ImGui::Begin("config", &show_imgui_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::SliderFloat("fov", &fov, 20.0f, 90.0f);  
            ImGui::SliderFloat("aspect", &aspect, 0.5f, 2.0f);  
            if (ImGui::Button("Close Me"))
                show_imgui_window = false;
            ImGui::End();
        }
        ImGui::Render();
        processInput(initData.window);
        // 渲染
        // 清除颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        time = glfwGetTime();
        // 实际渲染
        shader.setFloat("time", (sin(time) + 1) / 2);
        glm::mat4 transform = glm::mat4(1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(initData.VAO);
        glm::mat4 view = camera.GetViewMatrix();
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
            transform = projection * view * model;
            shader.setMat4("transform", transform);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // 双循环渲染
        glfwSwapBuffers(initData.window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &initData.VAO);
    glDeleteBuffers(1, &initData.VBO);
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(initData.window);
    glfwTerminate();
}

int main(int argc, char* argv[])
{
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
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}