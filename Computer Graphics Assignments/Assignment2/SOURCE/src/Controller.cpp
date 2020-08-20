#include "Controller.h"
#include "ShaderUtil.h"
#include <cmath>

// Initialize static variables.
Controller* Controller::controller = NULL;

/*
    Static function that controls the whole program.
    Hence, everything the program does, is done here.
    Once the function call is over, the program is finished.
*/
void Controller::run()
{
    // Handle GLFW initialization.
    if (!glfwInit()) {
        gl_log_err("GLFW initialization failed.\n");
        return;
    }
    else {
        gl_log("GLFW initialized successfully.\n");
    }

    // Set error callback.
    glfwSetErrorCallback(errorCallback);

    #ifdef IN_DEV
    gl_log("Asserting after GLFW initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted GLFW initialization.\n");
    #endif

    // Initialize the singleton Controller.
    gl_log("Constructing the Controller.\n");
    controller = new Controller(
        WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, MODE_WINDOWED);

    #ifdef IN_DEV
    gl_log("Asserting after Controller initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted Controller initialization.\n");
    #endif

    // Start GLEW.
    glewExperimental = GL_TRUE;
    if (glewInit()) {
        gl_log_err("GLEW initialization failed.\n");
        return;
    }
    else {
        gl_log("GLEW initialized successfully.\n");
    }

    // Initialize GL stuff.
    initGL();

    #ifdef IN_DEV
    gl_log("Asserting after GL initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted GL initialization.\n");
    #endif

    // Initialize Controller GL objects.
    controller->initGLObjects();

    #ifdef IN_DEV
    gl_log("Asserting after GL objects initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted GL objects initialization.\n");
    #endif

    // Start the main loop.
    GLFWwindow* primWindow = controller->primaryWindow;

    gl_log("Enterin the main loop.\n");
    while (!glfwWindowShouldClose(primWindow)) {
        // Update and render.
        controller->update();

        #ifdef IN_DEV
        gl_log("Asserting after update.\n");
        assert(glGetError() == GL_NO_ERROR);
        gl_log("Asserted update.\n");
        #endif

        controller->render();

        #ifdef IN_DEV
        gl_log("Asserting after render.\n");
        assert(glGetError() == GL_NO_ERROR);
        gl_log("Asserted render.\n");
        #endif

        // Display last render.
        glfwSwapBuffers(primWindow);

        // Wait for events to happen.
        glfwWaitEvents();
    }

    // Cleanup.
    gl_log("Destructing the Controller.\n");
    delete controller;
}

/*
    Constructs a Controller.
*/
Controller::Controller(
    const char* title, int width, int height, bool isFullScreen)
{
    // Basic window variables.
    this->title = title;
    this->width = width;
    this->height = height;
    this->isFullScreen = isFullScreen;

    // Initialize GLFW windows.
    this->primaryWindow = glfwCreateWindow(
        width, height, title, NULL, NULL
    );

    if (!primaryWindow) {
        gl_log_err("ERROR: Controller could not initialize GLFW windows.\n");
        return;
    }

    // Set context and swap interval.
    glfwMakeContextCurrent(primaryWindow);
    glfwSwapInterval(SWAP_INTERVAL);

    // Log version.
    gl_log("Successfully created OpenGL context.\n");
    gl_log("\tRenderer: %s\n", glGetString(GL_RENDERER));
    gl_log("\tSupported OpenGL version: %s\n", glGetString(GL_VERSION));

    // Set window mode.
    if (isFullScreen) {
        setFullScreen(primaryWindow);
    }
    else {
        setWindowed(primaryWindow);
    }

    // Set keyboard listener.
    glfwSetKeyCallback(primaryWindow, handleKeypress);

    // Set mouse listeners.
    glfwSetMouseButtonCallback(primaryWindow, handleMouseButton);
    glfwSetScrollCallback(primaryWindow, handleMouseScroll);

    // Set resize listener.
    glfwSetWindowSizeCallback(primaryWindow, handleResize);

    // Create the rendered objects.
    scene = new Scene();
    overlay = new Overlay();

    // Create the uniform matrices.
    modelViewMatrix = new matrix4(1.0);
    cameraMatrix = new matrix4(1.0);
}

/*
    Deconstructs the Controller.
*/
Controller::~Controller()
{
    // Cleanup.
    delete scene;
    delete overlay;
    delete vertices;
    delete colors;
    delete indices;
    delete modelViewMatrix;
    delete cameraMatrix;

    // GLFW termination is handled in the main function.
}

/*
    Sets the display mode of the given window to full screen.
*/
void Controller::setFullScreen(GLFWwindow* window)
{
    isFullScreen = true;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(
        window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate
    );
}

/*
    Sets the display mode of the given window to windowed.
*/
void Controller::setWindowed(GLFWwindow* window)
{
    isFullScreen = false;
    glfwSetWindowMonitor(
        window, NULL, 0, 0, this->width, this->height, GLFW_DONT_CARE
    );
}

/*
    Updates everything contained within the Controller.
*/
void Controller::update()
{
    // Update buffer data.
    scene->update(vertices, colors, indices);
    overlay->update(vertices, colors, indices);
}

/*
    Renders everything contained within the Controller.
*/
void Controller::render()
{
    // First, clear the display.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Refresh buffer data.
    refreshBuffers();
    // Draw.
    glDrawElements(
        GL_TRIANGLES,
        indexLength,
        GL_UNSIGNED_INT,
        0
    );
}

/*
    Function used to initialize some OpenGL parameters.
*/
void Controller::initGL()
{
    glClearColor(0.85f, 0.85f, 0.85f, 0.85f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    gl_log("Successfully initialized GL parameters.\n");
}

/*
    Initializes shaders and buffers, sets the program.
*/
void Controller::initGLObjects()
{
    gl_log("Initializing GL Objects for Controller:\n");

    // Initialize buffer arrays.
    initBufferLength();
    vertices = new vector4[vcLength];
    colors = new vector3[vcLength];
    indices = new unsigned int[indexLength];

    #ifdef IN_DEV
    gl_log("Asserting after buffer length initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer length initialization.\n");
    #endif

    gl_log("Initializing buffers for Controller.\n");

    // Initialize buffers.
    if (!initBuffers()) {
        // Failure is already handled inside initBuffers.
        return;
    }

    #ifdef IN_DEV
    gl_log("Asserting after buffer initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer initialization.\n");
    #endif

    gl_log("\tInitializing shaders for Controller.\n");

    // Initialize shaders.
    if (!initShaders()) {
        // Failure is already handled inside initShaders.
        return;
    }

    #ifdef IN_DEV
    gl_log("Asserting after shader initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted shader initialization.\n");
    #endif

    gl_log("\tSetting the program for Controller.\n");

    // Set the shader program.
    glUseProgram(shaderProgram);

    #ifdef IN_DEV
    gl_log("Asserting after shader program initialization.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted program shader initialization.\n");
    #endif
}

/*
    Compiles and initializes the shaders and the program.
*/
bool Controller::initShaders()
{
    // Compile flag.
    bool status = true;

    status &= ShaderUtil::compileShader(
        vertexShader, VERTEX_SHADER_PATH, GL_VERTEX_SHADER
    );
    status &= ShaderUtil::compileShader(
        fragmentShader, FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER
    );

    if (!status) {
        gl_log("Detected unsuccessful shader compilation. Exiting program.\n");
        glfwSetWindowShouldClose(primaryWindow, GLFW_TRUE);
        return false;
    }

    // Compile the shader program.
    GLuint* shaderIDs = new GLuint[2];
    shaderIDs[0] = vertexShader;
    shaderIDs[1] = fragmentShader;
    status = ShaderUtil::compileProgram(
        shaderProgram, shaderIDs, 2
    );
    delete shaderIDs;

    if (!status) {
        gl_log("Detected unsuccessful program linking. Exiting the program.\n");
        glfwSetWindowShouldClose(primaryWindow, GLFW_TRUE);
        return false;
    }

    #ifdef IN_DEV
    // If in development mode validate the program.
    status = ShaderUtil::validateProgram(shaderProgram);
    if (!status) {
        gl_log("Detected invalid program. Exiting the program.\n");
        glfwSetWindowShouldClose(primaryWindow, GLFW_TRUE);
        return false;
    }

    // If in development mode print detailed information about the program.
    ShaderUtil::printAll(shaderProgram);
    #endif

    return true;
}

/*
    Function used to initialize vertex array objects and buffers.
*/
bool Controller::initBuffers()
{
    // Vertex buffer.
    glGenBuffers(1, &vbVertex);
    glBindBuffer(GL_ARRAY_BUFFER, vbVertex);
    glBufferData(
        GL_ARRAY_BUFFER,
        vcLength * sizeof(vector4),
        vertices,
        GL_STATIC_DRAW
    );

    // Color buffer.
    glGenBuffers(1, &vbColor);
    glBindBuffer(GL_ARRAY_BUFFER, vbColor);
    glBufferData(
        GL_ARRAY_BUFFER,
        vcLength * sizeof(vector3),
        colors,
        GL_STATIC_DRAW
    );

    // Index buffer.
    glGenBuffers(1, &vbIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbIndex);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indexLength * sizeof(unsigned int),
        indices,
        GL_STATIC_DRAW
    );

    #ifdef IN_DEV
    gl_log("Asserting after buffer creation.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer creation.\n");
    #endif

    // VAO.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    #ifdef IN_DEV
    gl_log("Asserting after VAO creation.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer VAO creation.\n");
    #endif

    // Bind vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbVertex);
    glVertexAttribPointer(SHADER_POS_LOC, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // Bind colors.
    glBindBuffer(GL_ARRAY_BUFFER, vbColor);
    glVertexAttribPointer(SHADER_COL_LOC, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Bind indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbIndex);

    #ifdef IN_DEV
    gl_log("Asserting after buffer attribute binding.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer attribute binding.\n");
    #endif

    // Enable attributes.
    glEnableVertexAttribArray(SHADER_POS_LOC);
    glEnableVertexAttribArray(SHADER_COL_LOC);

    #ifdef IN_DEV
    gl_log("Asserting after buffer attribute enabling.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted buffer attribute enabling.\n");
    #endif

    // Bind uniforms.
    uniformMV = SHADER_MV_LOC;
    uniformProj = SHADER_PROJ_LOC;

    #ifdef IN_DEV
    gl_log("Asserting after uniform attribute setting.\n");
    assert(glGetError() == GL_NO_ERROR);
    gl_log("Asserted uniform attribute set.\n");
    #endif

    // Done.
    return true;
}

/*
    Asks objects to be rendered for the space they need.
*/
void Controller::initBufferLength()
{
    vcLength = 0;
    indexLength = 0;
    scene->getRequiredSpace(&vcLength, &indexLength);
    overlay->getRequiredSpace(&vcLength, &indexLength);
    gl_log(
        "Buffer lengths acquired. VertexL: %u, IndexL: %u.\n",
        vcLength, indexLength
    );
}

/*
    Refreshes OpenGL buffers from array buffer data.
*/
void Controller::refreshBuffers()
{
    // Vertex buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbVertex);
    glBufferData(
        GL_ARRAY_BUFFER,
        vcLength * sizeof(vector4),
        vertices,
        GL_STATIC_DRAW
    );

    // Color buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbColor);
    glBufferData(
        GL_ARRAY_BUFFER,
        vcLength * sizeof(vector3),
        colors,
        GL_STATIC_DRAW
    );

    // Index buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbColor);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indexLength * sizeof(unsigned int),
        indices,
        GL_STATIC_DRAW
    );

    // Uniforms.
    matrix4* identity = new matrix4(1.0);
    matrix4* mvTemp = ExtendedAngel::multiply(*identity, *modelViewMatrix);
    matrix4* camTemp = ExtendedAngel::multiply(*identity, *cameraMatrix);
    glUniformMatrix4fv(
        uniformMV,
        1,
        GL_TRUE,
        *mvTemp
    );
    glUniformMatrix4fv(
        uniformProj,
        1,
        GL_TRUE,
        *camTemp
    );

    // Cleanup.
    delete identity;
    delete mvTemp;
    delete camTemp;
}

/*
    Function used to provide GLFW a way of notifying the program about errors.
*/
void Controller::errorCallback(int error, const char* description)
{
    gl_log_err("GLFW ERROR: Code %d, Message: %s\n", error, description);
}

/*
    Function used to handle window close.
*/
void Controller::handleExit(GLFWwindow* window)
{
    // Unused and unimplemented.
}

/*
    Handles keyboard inputs.
*/
void Controller::handleKeypress(
    GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Only work on press downs.
    if (action != GLFW_PRESS) {
        return;
    }

    // Temporary camera matrix helpers.
    matrix4** camMatrix = &(controller->cameraMatrix);
    matrix4* transformation = NULL;
    matrix4* tempCam = NULL;
    matrix4* swapPtr;

    switch (key) {
        default:
        // Unexpected or unhandled key press.
        break;

        case GLFW_KEY_ENTER:
        if (mods == GLFW_MOD_ALT) {
            controller->isFullScreen = !controller->isFullScreen;

            if (controller->isFullScreen) {
                controller->setFullScreen(controller->primaryWindow);
            }
            else {
                controller->setWindowed(controller->primaryWindow);
            }
        }
        break;

        case GLFW_KEY_R:
        if (mods == GLFW_MOD_CONTROL) {
            delete controller->cameraMatrix;
            controller->cameraMatrix = ExtendedAngel::identity4();
        }
        break;
    }

    // Cleanup.
    if (transformation != NULL) {
        delete transformation;
    }
    if (tempCam != NULL) {
        delete tempCam;
    }
}

/*
    Handles mouse button events.
*/
void Controller::handleMouseButton(
    GLFWwindow* window, int button, int action, int mods)
{
    // Keep track of pointer position.
    static double xPos = 0;
    static double yPos = 0;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xPos, &yPos);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        double currX;
        double currY;
        glfwGetCursorPos(window, &currX, &currY);

        // Do some rotations.
        double dX = currX - xPos;
        double dY = currY - yPos;

        // Normalize the drag to [-120, +120].
        if (!controller->isFullScreen) {
            dX = dX * 120 / controller->width;
            dY = dY * 120 / controller->height;
        }
        else {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            dX = dX * 120 / mode->width;
            dY = dY * 120 / mode->height;
        }

        // Do the transformations.
        matrix4* xT = ExtendedAngel::rotateX(-dY); // Swap is intentional.
        matrix4* yT = ExtendedAngel::rotateY(-dX);

        matrix4* tMatrix = ExtendedAngel::multiply(
            *yT, *controller->cameraMatrix
        );
        delete yT;
        delete controller->cameraMatrix;

        yT = tMatrix;
        tMatrix = ExtendedAngel::multiply(
            *xT, *tMatrix
        );
        delete yT;
        delete xT;

        controller->cameraMatrix = tMatrix;

    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xPos, &yPos);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double currX;
        double currY;
        glfwGetCursorPos(window, &currX, &currY);
        // Do some translations.
        double dX = currX-xPos;
        double dY = currY-yPos;
        // Normalize the drag to [-2, +2].
        if (!controller->isFullScreen) {
            dX = dX * 2 / controller->width;
            dY = -dY * 2 / controller->height;
        }
        else {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            dX = dX * 2 / mode->width;
            dY = -dY * 2 / mode->height;
        }

        // Apply the transformation.
        matrix4* tMat = ExtendedAngel::translate(dX, dY, 0);
        matrix4* temp = ExtendedAngel::multiply(*tMat, *controller->cameraMatrix);
        delete tMat;
        tMat = controller->cameraMatrix;
        controller->cameraMatrix = temp;
        delete tMat;
    }
}

/*
    Handles mouse scroll events.
*/
void Controller::handleMouseScroll(
    GLFWwindow* window, double xoffset, double yoffset)
{
    // Apply the transformation.
    matrix4* tMat;

    if (yoffset < 0) {
        tMat = ExtendedAngel::scale(
            ZOOM_OUT_AMOUNT, ZOOM_OUT_AMOUNT, ZOOM_OUT_AMOUNT
        );
    }
    else {
        tMat = ExtendedAngel::scale(
            ZOOM_IN_AMOUNT, ZOOM_IN_AMOUNT, ZOOM_IN_AMOUNT
        );
    }

    matrix4* temp = ExtendedAngel::multiply(*tMat, *controller->cameraMatrix);
    delete tMat;
    tMat = controller->cameraMatrix;
    controller->cameraMatrix = temp;
    delete tMat;
}

/*
    Handles window resize events.
*/
void Controller::handleResize(GLFWwindow* window, int nWidth, int nHeight)
{
    // Update value and viewport if windowed.
    if (!controller->isFullScreen) {
        controller->width = nWidth;
        controller->height = nHeight;
        glViewport(0, 0, controller->width, controller->height);
    }
    // Only update the viewport otherwise.
    else {
        glViewport(0, 0, nWidth, nHeight);
    }
}
