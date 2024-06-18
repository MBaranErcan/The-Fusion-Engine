/* Last modified: 2021-08-15 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Shader.h"

#include <iostream>
#include <stdio.h>

// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void moveSquare(GLFWwindow* window, glm::mat4& squareModel, GLfloat deltaTime); // Move: 8, 5, 4, 6 (up, down, left, right)

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


    // Load GLAD for OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Camera Settings
    camera.MovementSpeed = cameraSpeed;

    
    // Shaders
    Shader shader("shaders/texture.vert", "shaders/texture.frag");


    // Vertex data
    GLfloat vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f  // top left
    };

    GLuint indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0); // The positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // The colors
    glEnableVertexAttribArray(1);

    // Square model matrix
    glm::mat4 squareModel = glm::mat4(1.0f); // Initialize with identity matrix


    while (!glfwWindowShouldClose(window))
    {
        // Time
        GLfloat currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrame;
        lastFrame = currentFrameTime;

        // Render
		glClearColor(0.15f, 0.25f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        // Process Input
        processInput(window);
        moveSquare(window, squareModel, deltaTime);

        shader.use();

        // MVP transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * squareModel;

        // Render the square
        shader.setMat4("mvp", mvp);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

       
        // Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


    glfwTerminate();
    return 0;
}

/* Function definitions */

// User input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Function for window-resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function to move the square
void moveSquare(GLFWwindow* window, glm::mat4 &squareModel, GLfloat deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        squareModel = glm::translate(squareModel, glm::vec3(0.0f, squareSpeed * deltaTime, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        squareModel = glm::translate(squareModel, glm::vec3(0.0f, -squareSpeed * deltaTime, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        squareModel = glm::translate(squareModel, glm::vec3(-squareSpeed * deltaTime, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        squareModel = glm::translate(squareModel, glm::vec3(squareSpeed * deltaTime, 0.0f, 0.0f));
    }
}
