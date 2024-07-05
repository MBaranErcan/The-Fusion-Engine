#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Graphics/Light.h"

#include <reusable/Cube.h>

#include <iostream>
#include <stdio.h>

// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Settings
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
const GLfloat cameraSpeed = 2.5f;
const GLfloat squareSpeed = 0.25f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// State keepers
bool isWireframe = false;
bool pKeyWasPressed = false;

// Global ambient light
glm::vec3 globalAmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);
float globalAmbientStrength = 0.05;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Window creation */
    GLFWwindow* window = glfwCreateWindow(800, 600, "The Fusion Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // On the calling thread, make the context of the specified window current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);

    // Load GLAD for OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    // Camera Settings
    camera.MovementSpeed = cameraSpeed;

    // OpenGL Settings
    glEnable(GL_DEPTH_TEST);


    // Shaders
    Shader shader("shaders/default.vert", "shaders/default.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");


    // LightManager
    LightManager lightManager;
    // DirLight params:     glm::vec3 color, glm::vec3 direction
    // PointLight params:   glm::vec3 color, glm::vec3 position, float constant, float linear, float quadratic
    // SpotLight params:    glm::vec3 color, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff
     
    lightManager.addLight(std::make_shared<DirectionalLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f)));
    
    lightManager.addLight(std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(5.0f, 2.0f, -2.0f),    1.0f, 0.09f, 0.032f));
    lightManager.addLight(std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.5f, 3.0f, -6.0f),    1.0f, 0.09f, 0.032f));
    lightManager.addLight(std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(10.0f, -1.0f, -10.0f), 1.0f, 0.09f, 0.032f));
    lightManager.addLight(std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -3.0f),   1.0f, 0.09f, 0.032f));
    
    lightManager.addLight(std::make_shared<SpotLight>(glm::vec3(1.0f, 1.0f, 1.0f), camera.Position, camera.Front, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));


    // Models
    Model model_Backpack("assets/backpack/backpack.obj", false);

    // Skybox
    Cube* skyboxCube = new Cube(true);
    std::vector<std::string> faces = {
        "assets/skybox II/right.jpg",
        "assets/skybox II/left.jpg",
        "assets/skybox II/top.jpg",
        "assets/skybox II/bottom.jpg",
        "assets/skybox II/front.jpg",
        "assets/skybox II/back.jpg"
    };
    unsigned int skyboxTexture = Texture::loadCubemap(faces);

    while (!glfwWindowShouldClose(window))
    {
        // Time
        GLfloat currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        // Process Input
        processInput(window);

        glClearColor(0.15f, 0.25f, 0.55f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Camera and transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));


        // Skybox
        // Activate skybox shader
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", viewSkybox);
        skyboxCube->Draw();

        
        // Back to default depth function
        glDepthFunc(GL_LESS);

        // Activate default shader
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Camera position
        shader.setVec3("viewPos", camera.Position);

        // Draw objects
        // Backpack model
        shader.setVec3("objectColor", 1.0f, 0.5f, 0.5f);
        glm::mat4 modelBackpack = glm::mat4(1.0f);
        modelBackpack = glm::translate(modelBackpack, glm::vec3(0.0f, 0.0f, -5.0f));
        modelBackpack = glm::scale(modelBackpack, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", modelBackpack);
        model_Backpack.Draw(shader);

        // Lights
        int dirLightCount = 0;
        int pointLightCount = 0;
        int spotLightCount = 0;

        // Global ambient light
        shader.setVec3("globalAmbientColor", globalAmbientColor);
        shader.setFloat("globalAmbientStrength", globalAmbientStrength);

        // For each light in the lightManager, check if it is a DirectionalLight, PointLight or SpotLight
        for (auto& light : lightManager.lights) {
            if (auto dl = std::dynamic_pointer_cast<DirectionalLight>(light)) {
                dl->SendToShader(shader, "dirLights[" + std::to_string(dirLightCount++) + "]");
            }
            else if (auto pl = std::dynamic_pointer_cast<PointLight>(light)) {
                pl->SendToShader(shader, "pointLights[" + std::to_string(pointLightCount++) + "]");
            }
            else if (auto sl = std::dynamic_pointer_cast<SpotLight>(light)) {
                sl->SendToShader(shader, "spotLights[" + std::to_string(spotLightCount++) + "]");
            }
        }

        shader.setInt("numDirLights", dirLightCount);
        shader.setInt("numPointLights", pointLightCount);
        shader.setInt("numSpotLights", spotLightCount);



        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // De-allocate resources
    delete skyboxCube;


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

/* Function definitions */
//-----------------------------------------------------------
// User input
void processInput(GLFWwindow* window)
{
    // Close window (ESC)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement (W, A, S, D, Space, Shift)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

    // Change polygon mode (P)
    bool pKeyPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    if (pKeyPressed && !pKeyWasPressed)
    {
		isWireframe = !isWireframe;
		glPolygonMode(GL_FRONT_AND_BACK, isWireframe ? GL_LINE : GL_FILL);
	}
	pKeyWasPressed = pKeyPressed;
}

//-----------------------------------------------------------
// Function for window-resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//-----------------------------------------------------------
// Mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//-----------------------------------------------------------
// Mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
