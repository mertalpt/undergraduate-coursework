#include <thread>
#include <cassert>
#include "Logger.h"
#include "Controller.h"

/*
    Starts the program, then hands the control
    to the Window object.
*/
int main(int argc, char** argv)
{
    // Ensure the log file is operable.
    assert(restart_gl_log());

    // Hand over the control.
    gl_log("Handing the control to Controller.run().\n");
    std::thread controllerThread(Controller::run);

    // Take the control.
    controllerThread.join();
    gl_log("Taking the control from Controller.run().\n");

    // Cleanup and exit the program.
    gl_log("Terminating GLFW.\n");
    glfwTerminate();
    gl_log("Program exited successfully.");
    return 0;
}
