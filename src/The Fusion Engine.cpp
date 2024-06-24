/* Last modified: 2021-08-15 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <Graphics/stb_image.h>


#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <iostream>
#include <stdio.h>

// Function prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
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

// State keepers
bool isWireframe = false;
bool pKeyWasPressed = false;


// Vertex data (Cube)
GLfloat vertices[] = {
    // positions          // Color coords
    // front face
    0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,	    0.0f, 0.0f, // Lower left corner
    0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,	    0.0f, 1.0f, // Upper left corner
    -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, // Upper right corner
    -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 1.0f,     1.0f, 0.0f,  // Lower right corner

    // back face
    0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,	    0.0f, 0.0f, // Lower left corner
    0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,	    0.0f, 1.0f, // Upper left corner
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 	1.0f, 0.0f  // Lower right corner
};

GLuint indices[] = {
    // front face
    0, 1, 3, // first triangle
    1, 2, 3,  // second triangle
    // back face
    4, 5, 7, // first triangle
    5, 6, 7,  // second triangle
    // right face
    0, 1, 4, // first triangle
    1, 5, 4,  // second triangle
    // left face
    2, 3, 6, // first triangle
    3, 7, 6,  // second triangle
    // top face
    0, 3, 4, // first triangle
    3, 7, 4,  // second triangle
    // bottom face
    1, 2, 5, // first triangle
    2, 6, 5  // second triangle
};


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

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Camera Settings
    camera.MovementSpeed = cameraSpeed;


    // Shaders
    Shader shader("shaders/texture.vert", "shaders/texture.frag");


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);                       // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));   // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));   // Texture
    glEnableVertexAttribArray(2);

    //// Unbind VAO, VBO and EBO
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // Texture
    Texture texture1("assets/skybox/CloudyCrown_01_Midday/Textures/CloudyCrown_Midday_Front.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture1.texUnit(shader, "tex0", 0);


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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process Input
        processInput(window);
        moveSquare(window, squareModel, deltaTime);

        shader.use();
        texture1.Bind();

        // MVP transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 mvp = projection * view * squareModel;
        glm::mat4 mvp = projection * view * model;

        // Render the square
        shader.setMat4("mvp", mvp);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    texture1.Delete();


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
// Function to move the square
void moveSquare(GLFWwindow* window, glm::mat4& squareModel, GLfloat deltaTime)
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