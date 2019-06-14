#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include <Eigen/Dense>
using namespace Eigen;

#include "../includes/Particle.h"
#include "../includes/Gridify.h"
#include "../includes/ShaderProgram.h"
#include "../includes/Texture2D.h"
#include "../includes/Camera.h"
#include "../includes/Mesh.h"

// Global Variables
OrbitCamera orbitCamera;
float gRadius = 1.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gModelYaw = 0.0f;
float gModelPitch = 0.0f;
const char *APP_TITLE = "CLOTH SIM";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow *gWindow = NULL;
bool gWireframe = false;
bool gFlashlightOn = true;
glm::vec4 gClearColor(1.0f, 1.0f, 1.0f, 1.0f);
static bool mac_moved = false;

const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0; // units per second
const float MOUSE_SENSITIVITY = 0.2f;

// Function prototypes
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow *window, int width, int height);
void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY);
void glfw_onMouseMove(GLFWwindow *window, double posX, double posY);

void update(double elapsedTime);
void showFPS(GLFWwindow *window);
bool initOpenGL();
void mac_patch(GLFWwindow *window);

bool initOpenGL()
{
    // Intialize GLFW
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

    // Create an OpenGL 3.3 core, forward compatible context window
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(gWindow);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
    glfwSetScrollCallback(gWindow, glfw_onMouseScroll);
    glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);

    // Hides and grabs cursor, unlimited movement
    // glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

    // Define the viewport dimensions
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        gWireframe = !gWireframe;
        // if (gWireframe)
        // 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // else
        // 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        // toggle the flashlight
        gFlashlightOn = !gFlashlightOn;
    }
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow *window, int width, int height)
{
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow *window, double deltaX, double deltaY)
{
    double fov = orbitCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    orbitCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
    // Camera orientation
    double mouseX, mouseY;

    // Get the current mouse cursor position delta
    // glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    // Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
    // fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

    // Clamp mouse cursor to center of screen
    // glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    // Camera FPS movement

    // Forward/backward
    // if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    // else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

    // // Strafe left/right
    // if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
    // else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

    // // Up/down
    // if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
    // else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
    // 	fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * orbitCamera.getLook());
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * -orbitCamera.getLook());

    // Strafe left/right
    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * -orbitCamera.getRight());
    else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * orbitCamera.getRight());

    // Up/down
    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
    else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
        orbitCamera.move(MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow *window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

    elapsedSeconds = currentSeconds - previousSeconds;

    // Limit text updates to 4 times per second
    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        // The C++ way of setting the window title
        std::ostringstream outs;
        outs.precision(3); // decimal places
        outs << std::fixed
             << APP_TITLE << "    "
             << "FPS: " << fps << "    "
             << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        // Reset for next average.
        frameCount = 0;
    }

    frameCount++;
}
void mac_patch(GLFWwindow *window)
{
    if (glfwGetTime() > 2.0)
    {
        mac_moved = true;
    }
    // glfwGetTim

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && !mac_moved)
    {
        int x, y;
        glfwGetWindowPos(window, &x, &y);
        glfwSetWindowPos(window, ++x, y);
    }
    else
    {
        mac_moved = true;
    }
}
void glfw_onMouseMove(GLFWwindow *window, double posX, double posY)
{
    if (glfwGetKey(gWindow, GLFW_KEY_Q))
    {

        static glm::vec2 lastMousePos = glm::vec2(0, 0);

        // Update angles based on Left Mouse Button input to orbit around the cube
        if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1)
        {
            // each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
            gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
            gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
            // std::cout << "YAW" << gYaw << std::endl;
            // std::cout << "PITCH" << gPitch << std::endl;
        }

        // Change orbit camera radius with the Right Mouse Button
        if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1)
        {
            float dx = 0.01f * ((float)posX - lastMousePos.x);
            float dy = 0.01f * ((float)posY - lastMousePos.y);
            gRadius += dx - dy;
        }

        lastMousePos.x = (float)posX;
        lastMousePos.y = (float)posY;
    }
    else if (glfwGetKey(gWindow, GLFW_KEY_M))
    {

        static glm::vec2 lastMousePosMod = glm::vec2(0, 0);

        // Update angles based on Left Mouse Button input to orbit around the cube
        if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1)
        {
            // each pixel represents a quarter of a degree rotation (this is our mouse sensitivity)
            gModelYaw += ((float)posX - lastMousePosMod.x) * MOUSE_SENSITIVITY;
            gModelPitch += ((float)posY - lastMousePosMod.y) * MOUSE_SENSITIVITY;
        }
        lastMousePosMod.x = (float)posX;
        lastMousePosMod.y = (float)posY;
    }
}
