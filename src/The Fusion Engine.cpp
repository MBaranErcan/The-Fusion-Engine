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

#include <iostream>
#include <stdio.h>

// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadCubemap(std::vector<std::string> faces);


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

/*** Lighting ***/
// Point light
glm::vec3 lightPos = glm::vec3(0.0f, 3.0f, 3.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // Bright white light

// Light attenuation factors
float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;

// Ambient light
glm::vec3 ambientColor = glm::vec3(0.15f, 0.15f, 0.15f);;

// Object light properties
float specularIntensity = 0.5f;
float shininess = 32.0f;


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

    glEnable(GL_DEPTH_TEST);

    // Camera Settings
    camera.MovementSpeed = cameraSpeed;


    // Shaders
    Shader shader("shaders/texture.vert", "shaders/texture.frag");
  //  Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
    Shader lightShader("shaders/light.vert", "shaders/light.frag");

    // LightManager
    LightManager lightManager;
    // TODO: Implement a light manager class and test it


    // Models
    Model model_Backpack("assets/backpack/backpack.obj", false);

    while (!glfwWindowShouldClose(window))
    {
        // Time
        GLfloat currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        // Process Input
        processInput(window);

        // Render
        glClearColor(0.15f, 0.25f, 0.55f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // MVP transformations     
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Shader uniforms
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightColor", lightColor);
        shader.setFloat("constant", constant);
        shader.setFloat("linear", linear);
        shader.setFloat("quadratic", quadratic);
        shader.setVec3("ambientColor", ambientColor);
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("specularIntensity", specularIntensity);
        shader.setFloat("shininess", shininess);

        // Backpack model
        glm::mat4 modelBackpack = glm::mat4(1.0f);
        modelBackpack = glm::translate(modelBackpack, glm::vec3(0.0f, 0.0f, 0.0f));
        modelBackpack = glm::scale(modelBackpack, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setMat4("model", modelBackpack);
        model_Backpack.Draw(shader);

        lightShader.use();

        // Send lights to shader
        //dirLight.SendToShader(lightShader, "dirLight");


        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources



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

//-----------------------------------------------------------
GLuint loadCubemap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
