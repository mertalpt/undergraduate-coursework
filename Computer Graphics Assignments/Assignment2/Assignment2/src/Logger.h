#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#define GL_LOG_FILE "./GL_LOGS.log"

bool restart_gl_log();
bool gl_log(const char* message, ...);
bool gl_log_err(const char* message, ...);

#endif // Define __LOGGER_H_
