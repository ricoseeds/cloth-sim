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
float gRadius = 20.0f;
float gYaw = 29.0f;
float gPitch = 25.0f;
float gModelYaw = 0.0f;
float gModelPitch = 0.0f;
const char *APP_TITLE = "SOFT TISSUE DEFORMATION";
int gWindowWidth = 1024;
int gWindowHeight = 768;
GLFWwindow *gWindow = NULL;
bool gWireframe = false;
bool gFlashlightOn = true;
glm::vec4 gClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
            // liverMat = glm::rotate(glm::mat4(1.0), glm::radians((float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
            gModelPitch += ((float)posY - lastMousePosMod.y) * MOUSE_SENSITIVITY;
            // std::cout << "YAW" << gModelYaw << std::endl;
            // std::cout << "PITCH" << gModelPitch << std::endl;
        }
        lastMousePosMod.x = (float)posX;
        lastMousePosMod.y = (float)posY;
    }
}

int main()
{
    // Gridify *grid;
    // grid = new Gridify(100);
    // grid->build_connections();
    // MatrixXd conn = grid->get_mat();
    // std::cout << conn << std::endl;
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    ShaderProgram lightingShader;
    lightingShader.loadShaders("shaders/vert.vs", "shaders/frag.fs");

    // Load meshes and textures
    const int numModels = 1;
    Mesh mesh[numModels];
    Texture2D texture[numModels];

    mesh[0].loadOBJ("model/sphere.obj", false);
    texture[0].loadTexture("textures/red.png", true);

    // Model positions
    glm::vec3 modelPos[] = {
        glm::vec3(0.0f, 0.0f, 0.0f)

    };

    // Model scale
    glm::vec3 modelScale[] = {
        glm::vec3(0.10f, 0.1f, 0.1f)

    };

    // Point Light positions
    glm::vec3 pointLightPos[3] = {
        glm::vec3(-5.0f, 12.8f, 0.0f),
        glm::vec3(0.5f, 13.8f, 0.0f),
        glm::vec3(5.0f, 13.8, 0.0f)};

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        glfwPollEvents();
        update(deltaTime);
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);

        // Create the View matrix
        orbitCamera.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        orbitCamera.rotate(gYaw, gPitch);
        // orbitCamera.move(glm::vec3(10.0f, 0.0f, 0.0f));
        orbitCamera.setRadius(gRadius);
        view = orbitCamera.getViewMatrix();

        // Create the projection matrix
        projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

        // update the view (camera) position
        glm::vec3 viewPos;
        viewPos = orbitCamera.getPosition();
        // std::cout << glm::to_string

        // Must be called BEFORE setting uniforms because setting uniforms is done
        // on the currently active shader program.
        lightingShader.use();
        lightingShader.setUniform("model", glm::mat4(1.0)); // do not need to translate the models so just send the identity matrix
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("viewPos", viewPos);

        // Directional light
        lightingShader.setUniform("sunLight.direction", glm::vec3(0.0f, -0.9f, -0.17f));
        lightingShader.setUniform("sunLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f)); // dark
        lightingShader.setUniform("sunLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));

        // Point Light 1
        lightingShader.setUniform("pointLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // green-ish light
        lightingShader.setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("pointLights[0].position", pointLightPos[0]);
        lightingShader.setUniform("pointLights[0].constant", 1.0f);
        lightingShader.setUniform("pointLights[0].linear", 0.022f);
        lightingShader.setUniform("pointLights[0].exponent", 0.020f);

        // Point Light 2
        lightingShader.setUniform("pointLights[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("pointLights[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // red-ish light
        lightingShader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("pointLights[1].position", pointLightPos[1]);
        lightingShader.setUniform("pointLights[1].constant", 1.0f);
        lightingShader.setUniform("pointLights[1].linear", 0.022f);
        lightingShader.setUniform("pointLights[1].exponent", 0.020f);

        // Point Light 3
        lightingShader.setUniform("pointLights[2].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("pointLights[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // blue-ish light
        lightingShader.setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("pointLights[2].position", pointLightPos[2]);
        lightingShader.setUniform("pointLights[2].constant", 1.0f);
        lightingShader.setUniform("pointLights[2].linear", 0.22f);
        lightingShader.setUniform("pointLights[2].exponent", 0.20f);

        // Spot light
        glm::vec3 spotlightPos = orbitCamera.getPosition();
        // offset the flash light down a little
        spotlightPos.y -= 0.5f;

        lightingShader.setUniform("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("spotLight.position", spotlightPos);
        lightingShader.setUniform("spotLight.direction", orbitCamera.getLook());
        lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(15.0f)));
        lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(20.0f)));
        lightingShader.setUniform("spotLight.constant", 1.0f);
        lightingShader.setUniform("spotLight.linear", 0.0007f);
        lightingShader.setUniform("spotLight.exponent", 0.0000017f);
        lightingShader.setUniform("spotLight.on", gFlashlightOn);
        for (int i = 0; i < numModels; i++)
        {
            model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
            lightingShader.setUniform("model", model);
            // Set material properties
            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.9f, 0.9f, 0.9f));
            lightingShader.setUniform("material.shininess", 100.0f);
            if (gWireframe && i == 0)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                // texture[i].bind(0);
                mesh[i].draw();
                // texture[i].unbind(0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else
            {
                // texture[i].bind(0);
                mesh[i].draw();
                // texture[i].unbind(0);
            }
        }
        // Swap front and back buffers
        glfwSwapBuffers(gWindow);
        mac_patch(gWindow);
        lastTime = currentTime;
    }
    glfwTerminate();

    return 0;
}
