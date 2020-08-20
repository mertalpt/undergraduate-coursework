#ifndef __SHADERUTIL_H_
#define __SHADERUTIL_H_

#include <GL/glew.h>
#include <GL/gl.h>

// Shader attribute locations.
#define SHADER_POS_LOC  0
#define SHADER_COL_LOC  1
#define SHADER_MV_LOC   2
#define SHADER_PROJ_LOC 3

/*
    Utility class used to work with OpenGL shaders.
*/
class ShaderUtil
{
public:
    static bool compileShader(
        GLuint &id, const char* filePath, GLenum shaderType
    );
    static bool compileProgram(GLuint &id, GLuint* shaderIDs, int length);
    static bool validateProgram(GLuint program);
    static void printAll(GLuint program);
private:
    static void logShaderInfo(GLuint id);
    static void logProgramInfo(GLuint id);
    static const char* glTypeToString(GLenum type);
};

#endif // Define __SHADERUTIL_H_
