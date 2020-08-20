#ifndef __IDRAWABLE_H_
#define __IDRAWABLE_H_

#include <GL/glew.h>
#define GLFW_DLL // To use the latest GL libraries.
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "ExtendedAngel.h"

/*
    Interface to serve as a base for OpenGL drawable objects.
*/
class IDrawable
{
public:
    // I read it is required for proper cleanup.
    virtual ~IDrawable() {}
    void getRequiredSpace(int* vertexReq, int* indexReq);
    void update(
        vector4* vertices, vector3* colors, unsigned int* indices);
    int getVertexOffset();
    void setVertexOffset(int offset);
    int getIndexOffset();
    void setIndexOffset(int offset);


    int vcLength;

protected:
    vector4* vertices;
    vector3* colors;

    unsigned int* indices;
    int indexLength;

private:
    int vertexOffset;
    int indexOffset;
};

#endif // Define __IDRAWABLE_H_
