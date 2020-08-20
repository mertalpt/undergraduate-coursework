#include "ShaderUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include "Logger.h"

/*
    File reading code from: https://stackoverflow.com/a/3747128/8980631
    Reads the given file for the shader code.
    Compiles the shader and returns the shader ID in the out parameter id.
    Returns a boolean to indicate success or failure.
*/
bool ShaderUtil::compileShader(
    GLuint &id, const char* filePath, GLenum shaderType)
{
    gl_log("Starting processing the shader: %s\n", filePath);

    // Set the id to 0 for default error code.
    id = 0;

    // Open file.
    FILE* file = fopen(filePath, "r");

    // Check if file is opened.
    if (!file) {
        gl_log_err("ERROR: Could not open shader file.\n");
        return false;
    }

    gl_log("Shader source successfully opened.\n");

    char *buffer;
    long fSize;

    // Get file size.
    fseek(file, 0L, SEEK_END);
    fSize = ftell(file);
    rewind(file);

    gl_log("Source file successfully scanned for size.\n");

    // Create the buffer.
    buffer = (char*)malloc(sizeof(char)*fSize+1);

    if (!buffer) {
        gl_log_err("ERROR: Could not allocate buffer for the shader code.\n");
        fclose(file);
        return false;
    }

    gl_log("Source buffer successfully created.\n");

    // Copy the file into the buffer.
    if (1 != fread(buffer, fSize, 1, file)) {
        fclose(file);
        free(buffer);
        gl_log_err("ERROR: Reading the shader file failed.\n");
        return false;
    }

    // Add the terminating character.
    buffer[fSize] = '\0';

    gl_log("Source file successfully copied into the buffer.\n");

    // File is read into buffer now.
    fclose(file);

    gl_log("\tShader Source:\n%s\n", buffer);

    gl_log("Starting the shader creation process.\n");

    // Create new shader.
    id = glCreateShader(shaderType);

    if (id == 0) {
        gl_log_err("ERROR: Could not create new shader.\n");
        free(buffer);
        return false;
    }
    else {
        gl_log("Successfully created new shader.\n");
    }

    // Attach shader code.
    glShaderSource(id, 1, &buffer, NULL);
    free(buffer); // No longer needed.

    gl_log("Shader source successfully attached to the shader.\n");

    // Compile the shader.
    glCompileShader(id);

    // Check compilation status.
    int params = -1;
    glGetShaderiv(id, GL_COMPILE_STATUS, &params);

    // If and error occured.
    if (params != GL_TRUE) {
        gl_log_err(
            "ERROR: GL shader with ID %u did not compile successfully.\n", id
        );
        logShaderInfo(id);
        return false;
    }

    // Successful compilation.
    gl_log("Shader compiled successfully.\n");
    return true;
}

/*
    Compiles an OpenGL program whose ID is output with the output parameter id.
    Takes a variable amount of shader IDs. Puts them in the program.
*/
bool ShaderUtil::compileProgram(GLuint &id, GLuint* shaderIDs, int length)
{
    if (shaderIDs == NULL || length <= 0) {
        gl_log_err("ERROR: Invalid arguments in program compilation.\n");
        return false;
    }

    gl_log("Starting processing the shader program.\n");
    id = glCreateProgram();
    gl_log("Successfully created a new program.\n");

    for (int i = 0; i < length; i++) {
        glAttachShader(id, shaderIDs[i]);
    }

    gl_log("Successfully attached all shaders.\n");

    // Link program.
    glLinkProgram(id);

    int params = -1;
    glGetProgramiv(id, GL_LINK_STATUS, &params);

    if (params != GL_TRUE) {
        gl_log_err(
            "ERROR: GL program with ID %u did not link successfully.\n", id
        );
        logProgramInfo(id);
        return false;
    }

    // Successful link.
    gl_log("Successfully linked the program.\n");
    return true;
}

/*
    Validates a program.
    Only use during development, it is computationally expensive.
*/
bool ShaderUtil::validateProgram(GLuint program)
{
    gl_log("Validating the program with the ID: %u.\n", program);
    glValidateProgram(program);
    gl_log("Program validation is done.\n");

    int params = -1;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &params);

    if (params != GL_TRUE) {
        gl_log_err("The program is not valid.\n");
        logProgramInfo(program);
        return false;
    }
    else {
        gl_log("The program is valid.\n");
        return true;
    }
}

/*
    Prints detailed information about the given program.
*/
void ShaderUtil::printAll(GLuint program)
{
    gl_log("-----\nShader Program %i Info:\n", program);
    int params = -1;
    glGetProgramiv(program, GL_LINK_STATUS, &params);
    gl_log("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
    gl_log("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
    gl_log("GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for (int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib (
            program,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );
        if (size > 1) {
            for(int j = 0; j < size; j++) {
                char long_name[64];
                sprintf(long_name, "%s[%i]", name, j);
                int location = glGetAttribLocation(program, long_name);
                gl_log(
                    "  %i) Type:%s, Name:%s Location:%i\n",
                    i, glTypeToString(type), long_name, location
                );
            }
        }
        else {
            int location = glGetAttribLocation(program, name);
            gl_log(
                "  %i) Type:%s Name:%s Location:%i\n",
                i, glTypeToString(type), name, location
            );
        }
    }

    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
    gl_log("GL_ACTIVE_UNIFORMS = %i\n", params);
    for(int i = 0; i < params; i++) {
        char name[64];
        int max_length = 64;
        int actual_length = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(
            program,
            i,
            max_length,
            &actual_length,
            &size,
            &type,
            name
        );
        if(size > 1) {
            for(int j = 0; j < size; j++) {
                char long_name[64];
                sprintf(long_name, "%s[%i]", name, j);
                int location = glGetUniformLocation(program, long_name);
                gl_log(
                    "  %i) Type:%s Name:%s Location:%i\n",
                    i, glTypeToString(type), long_name, location
                );
            }
        } else {
            int location = glGetUniformLocation(program, name);
            gl_log(
                "  %i) Type:%s Name:%s Location:%i\n",
                i, glTypeToString(type), name, location
            );
        }
    }

    gl_log("---\n");
    logProgramInfo(program);
    gl_log("-----\n");
}

/*
    Logs the shader info.
    Used when the shader compilation was not successful.
*/
void ShaderUtil::logShaderInfo(GLuint id)
{
    gl_log("Shader compilation information:\n");

    int max_length = 2048;
    int actual_length = 0;
    char* shader_log = (char*)malloc(sizeof(char) * max_length);

    if (!shader_log) {
        gl_log_err(
            "ERROR: Error logging could not allocate memory for shader log.\n"
        );
        return;
    }

    // Get the log.
    glGetShaderInfoLog(id, max_length, &actual_length, shader_log);

    // Log the log.
    gl_log("Shader ID: %u\n%s", id, shader_log);

    // Cleanup.
    free(shader_log);
}

/*
    Logs the program info.
    Used when the program linking was not successful.
*/
void ShaderUtil::logProgramInfo(GLuint id)
{
    gl_log("Program linking information:\n");

    int max_length = 2048;
    int actual_length = 0;
    char* program_log = (char*)malloc(sizeof(char) * max_length);

    if (!program_log) {
        gl_log_err(
            "ERROR: Error logging could not allocate memory for program log.\n"
        );
        return;
    }

    // Get the log.
    glGetProgramInfoLog(id, max_length, &actual_length, program_log);

    // Log the log.
    gl_log("Program ID: %u\n%s", id, program_log);

    // Cleanup.
    free(program_log);
}

/*
    Converts GL type to human readable character.
*/
const char* ShaderUtil::glTypeToString(GLenum type)
{
    switch(type) {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}
