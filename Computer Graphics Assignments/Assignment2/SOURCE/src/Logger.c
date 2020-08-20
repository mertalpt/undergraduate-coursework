#include "Logger.h"

/*
    Function used to restart the log file.
*/
bool restart_gl_log()
{
    FILE *file = fopen(GL_LOG_FILE, "w");

    if (!file) {
        fprintf(
            stderr,
            "ERROR: Could not open the log file %s for writing.\n",
            GL_LOG_FILE
        );
        return false;
    }

    // Get the date.
    time_t now = time(NULL);
    char* date = ctime(&now);

    // Write to file.
    fprintf(file, "Starting the log. Local time: %s\n", date);

    // Cleanup.
    fclose(file);
    return true;
}

/*
    Function used to log non-error messages to the log file.
*/
bool gl_log(const char* message, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILE, "a");

    if (!file) {
        fprintf(
            stderr,
            "ERROR: Could not open the log file %s for appending.\n",
            GL_LOG_FILE
        );
        return false;
    }

    // Write to file.
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);

    // Cleanup.
    fclose(file);
    return true;
}

/*
    Function used to log error messages to the log file and the terminal.
*/
bool gl_log_err(const char* message, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILE, "a");

    if (!file) {
        fprintf(
            stderr,
            "ERROR: Could not open the log file %s for appending.\n",
            GL_LOG_FILE
        );
        return false;
    }

    // Write to terminal.
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);

    // Write to file.
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);

    // Cleanup.
    fclose(file);
    return true;
}
