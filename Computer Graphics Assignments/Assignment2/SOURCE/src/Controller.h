#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include <iostream>
#include <cassert>
#include <GL/glew.h>
#define GLFW_DLL // To use the latest GL libraries.
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "ExtendedAngel.h"
#include "Logger.h"
#include "ShaderUtil.h"
#include "IDrawable.h"
#include "Scene.h"
#include "Overlay.h"

// Flags.
#define IN_DEV
#define IN_DEV_VERBOSE

// Defined constants.
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define WINDOW_TITLE    "Alpine Engine v.Alpha"
#define MODE_FULLSCREEN true
#define MODE_WINDOWED   false
#define SWAP_INTERVAL   1

#define TRANSLATION_AMOUNT  0.05
#define ROTATION_AMOUNT     5
#define ZOOM_IN_AMOUNT      1.1
#define ZOOM_OUT_AMOUNT     0.9

// Shader paths.
#define VERTEX_SHADER_PATH "./shaders/main_vert.glsl"
#define FRAGMENT_SHADER_PATH "./shaders/main_frag.glsl"

/*
    Class used as a main controller.
    It is responsible for creating the window to work in,
    getting the OpenGL context and rendering other objects.

    Implemented as a singleton.
*/
class Controller
{
public:
    // Function called to start the Window.
    static void run();

private:
    // Privatized to implement as a singleton.
    Controller(const char* title, int width, int height, bool isFullScreen);
    ~Controller();

    // The singleton object.
    static Controller* controller;

    // GLFW windows, they act as OpenGL contexts.
    GLFWwindow* primaryWindow;

    // Primary window characteristics.
    const char* title;
    int width;
    int height;
    bool isFullScreen;

    // Shaders.
    GLuint vertexShader;
    GLuint fragmentShader;

    // Shader program.
    GLuint shaderProgram;

    // Rendered objects. They will be hardcoded for now.
    Scene* scene;
    Overlay* overlay;

    // Buffer objects.
    GLuint vao;
    GLuint vbVertex;
    GLuint vbColor;
    GLuint vbIndex;

    // Uniforms.
    GLuint uniformMV;
    GLuint uniformProj;

    // Buffer data.
    vector4* vertices;
    vector3* colors;
    unsigned int* indices;
    int vcLength;
    int indexLength;

    // Uniform data.
    matrix4* modelViewMatrix;
    matrix4* cameraMatrix;

    // Non-static functions.
    void setFullScreen(GLFWwindow* window);
    void setWindowed(GLFWwindow* window);
    void update();
    void render();

    // OpenGL related functions.
    static void initGL();
    void initGLObjects();
    bool initShaders();
    bool initBuffers();
    void initBufferLength();
    void refreshBuffers();

    // Static functions for GLFW.
    static void errorCallback(int error, const char* description);
    static void handleExit(GLFWwindow* window);
    static void handleKeypress(
        GLFWwindow* window, int key, int scancode, int action, int mods
    );
    static void handleMouseButton(
        GLFWwindow* window, int button, int action, int mods
    );
    static void handleMouseScroll(
        GLFWwindow* window, double xoffset, double yoffset
    );
    static void handleResize(GLFWwindow* window, int nWidth, int nHeight);
};

#endif // Define __CONTROLLER_H_
