#ifndef __EXTENDED_ANGEL_H_
#define __EXTENDED_ANGEL_H_

#include "Angel.h"

// Flags.
// #define IN_DEV

#define PI 3.14159265

// For ease of use.
typedef Angel::vec3 vector3;
typedef Angel::vec4 vector4;
typedef Angel::mat3 matrix3;
typedef Angel::mat4 matrix4;

/*
    Class used to provide extra features for Angel.h
*/
class ExtendedAngel
{
public:
    static float trackBallZ(float x, float y);
    static float determinant(matrix3 &matrix);
    static float determinant(matrix4 &matrix);
    static vector3* cross(vector3 &vec1, vector3 &vec2);
    static matrix3* identity3();
    static matrix4* identity4();
    static matrix3* inverse(matrix3 &matrix);
    static matrix4* inverse(matrix4 &matrix);
    static matrix3* minorOf(matrix4 &matrix, int row, int col);
    static matrix3* transpose(matrix3 &matrix);
    static matrix4* transpose(matrix4 &matrix);
    static matrix3* multiply(matrix3 &left, matrix3 &right);
    static matrix4* multiply(matrix4 &left, matrix4 &right);

    /*
        Transormation matrix formula source:
        https://people.cs.clemson.edu/~dhouse/courses/401/notes/affines-matrices.pdf
    */
    static matrix4* scale(float x, float y, float z);
    static matrix4* translate(float x, float y, float z);
    static matrix4* rotateX(float angle);
    static matrix4* rotateY(float angle);
    static matrix4* rotateZ(float angle);
};

#endif // Define __EXTENDED_ANGEL_H_
