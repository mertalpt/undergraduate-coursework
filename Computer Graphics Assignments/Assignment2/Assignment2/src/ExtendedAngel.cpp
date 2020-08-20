#include "ExtendedAngel.h"
#include <cmath>

/*
    Calculates the inverse of a 3x3 matrix.
*/
matrix3* ExtendedAngel::inverse(matrix3 &matrix)
{
    // Let's get ugly.
    matrix3* tMatrix = transpose(matrix);
    float e00 = (tMatrix[0][0]).x;
    float e01 = (tMatrix[0][1]).y;
    float e02 = (tMatrix[0][2]).z;
    float e10 = (tMatrix[1][0]).x;
    float e11 = (tMatrix[1][1]).y;
    float e12 = (tMatrix[1][2]).z;
    float e20 = (tMatrix[2][0]).x;
    float e21 = (tMatrix[2][1]).y;
    float e22 = (tMatrix[2][2]).z;
    delete tMatrix;

    // Determinant of minors.
    float det00 = e11 * e22 - e12 * e21;
    float det01 = e10 * e22 - e12 * e20;
    float det02 = e10 * e21 - e11 * e20;
    float det10 = e01 * e22 - e02 * e21;
    float det11 = e00 * e22 - e02 * e20;
    float det12 = e00 * e21 - e01 * e20;
    float det20 = e01 * e12 - e02 * e11;
    float det21 = e00 * e12 - e02 * e10;
    float det22 = e00 * e11 - e01 * e10;

    matrix3* adjoint = new matrix3(
        det00, det01, det02,
        det10, det11, det12,
        det20, det21, det22
    );

    matrix3* signs = new matrix3(
        +1, -1, +1,
        -1, +1, -1,
        +1, -1, +1
    );

    matrix3* inverseM = multiply(*adjoint, *signs);

    // Cleanup.
    delete adjoint;
    delete signs;

    // Divide by determinant.
    *inverseM /= determinant(matrix);

    // Return.
    return inverseM;
}

/*
    Calculates the inverse of a 4x4 matrix.
*/
matrix4* ExtendedAngel::inverse(matrix4 &matrix)
{
    // Adjoint matrix calculation.
    matrix4* tMatrix = transpose(matrix);
    matrix4* adjoint = new matrix4();
    matrix3* tempPtr;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tempPtr = minorOf(matrix, i ,j);
            adjoint[i][j] = determinant(*tempPtr);
            delete tempPtr;
        }
    }

    // Done with the transpose.
    delete tMatrix;

    matrix4* signs = new matrix4(
        +1, -1, +1, -1,
        -1, +1, -1, +1,
        +1, -1, +1, -1,
        -1, +1, -1, +1
    );

    matrix4* inverseM = multiply(*adjoint, *signs);

    // Cleanup.
    delete adjoint;
    delete signs;

    // Divide by determinant.
    *inverseM /= determinant(matrix);

    // Return.
    return inverseM;
}

/*
    Returns the minor matrix corresponding to the given indices.
*/
matrix3* ExtendedAngel::minorOf(matrix4 &matrix, int row, int col)
{
    float arr[3][3];
    int rowI = 0;
    int colI = 0;

    for (int i = 0; i < 4; i++) {
        if (i == row) {
            continue;
        }

        for (int j = 0; j < 4; j++) {
            if (j == col) {
                continue;
            }

            // Copy the value
            arr[i][j] = matrix[rowI][colI++];
        }

        // Reset column index.
        colI = 0;

        // Increment to next row.
        rowI++;
    }

    return new matrix3(
        arr[0][0], arr[0][1], arr[0][2],
        arr[1][0], arr[1][1], arr[1][2],
        arr[2][0], arr[2][1], arr[2][2]
    );
}

/*
    Returns the transpose of the input.
*/
matrix3* ExtendedAngel::transpose(matrix3 &matrix)
{
    vector3 row1 = matrix[0];
    vector3 row2 = matrix[1];
    vector3 row3 = matrix[2];

    return new matrix3(
        row1.x, row1.y, row1.z,
        row2.x, row2.y, row2.z,
        row3.x, row3.y, row3.z
    );
}

/*
    Returns the transpose of the input
*/
matrix4* ExtendedAngel::transpose(matrix4 &matrix)
{
    vector4 row1 = matrix[0];
    vector4 row2 = matrix[1];
    vector4 row3 = matrix[2];
    vector4 row4 = matrix[3];

    return new matrix4(
        row1.x, row1.y, row1.z, row1.w,
        row2.x, row2.y, row2.z, row2.w,
        row3.x, row3.y, row3.z, row3.w,
        row4.x, row4.y, row4.z, row4.w
    );
}

/*
    Multiplies the two matrices and returns the result.
*/
matrix3* ExtendedAngel::multiply(matrix3 &left, matrix3 &right)
{
    #ifdef IN_DEV
        std::cout << "Left:\n" << left << std::endl;
        std::cout << "Right:\n" << right << std::endl;
    #endif

    float mul[3][3] = {0};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                mul[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    matrix3* retM = new matrix3(
        mul[0][0], mul[1][0], mul[2][0],
        mul[0][1], mul[1][1], mul[2][1],
        mul[0][2], mul[1][2], mul[2][2]
    );

    #ifdef IN_DEV
        std::cout << "Result:\n" << (*retM) << std::endl;
    #endif

    return retM;
}

/*
    Multiplies the two matrices and returns the result.
*/
matrix4* ExtendedAngel::multiply(matrix4 &left, matrix4 &right)
{
    #ifdef IN_DEV
        std::cout << "Left:\n" << left << std::endl;
        std::cout << "Right:\n" << right << std::endl;
    #endif

    float mul[4][4] = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                mul[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    matrix4* retM = new matrix4(
        mul[0][0], mul[1][0], mul[2][0], mul[3][0],
        mul[0][1], mul[1][1], mul[2][1], mul[3][1],
        mul[0][2], mul[1][2], mul[2][2], mul[3][2],
        mul[0][3], mul[1][3], mul[2][3], mul[3][3]
    );

    #ifdef IN_DEV
        std::cout << "Result:\n" << (*retM) << std::endl;
    #endif

    return retM;
}

/*
    Creates and returns a 3x3 identity matrix
*/
matrix3* ExtendedAngel::identity3()
{
    return new matrix3(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
}

/*
    Creates and returns a 4x4 identity matrix
*/
matrix4* ExtendedAngel::identity4()
{
    return new matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

/*
    Calculates the projected Z coordinate for the virtual trackball.
*/
float ExtendedAngel::trackBallZ(float x, float y)
{
    float r = 1;
    float rSq = r * r;
    float sum = x * x + y * y;

    if (sum <= rSq/2) {
        return (sqrt(rSq - sum));
    }
    else {
        return (rSq / 2 / sqrt(sum));
    }
}

/*
    Calculates the determinant of a 3x3 matrix.
*/
float ExtendedAngel::determinant(matrix3 &matrix)
{
    vector3 row1 = matrix[0];
    vector3 row2 = matrix[1];
    vector3 row3 = matrix[2];

    // Horrible formula taken from:
    // en.wikipedia.org/wiki/Determinant#Algorithmic_implementation
    return (
        + row1.x * row2.y * row3.z
        + row1.y * row2.z * row3.x
        + row1.z * row2.x * row3.y
        - row1.z * row2.y * row3.x
        - row1.y * row2.x * row3.z
        - row1.x * row2.z * row3.y
    );
}

/*
    Calculates the determinant of a 4x4 matrix.
*/
float ExtendedAngel::determinant(matrix4 &matrix)
{
    matrix3* minor00 = minorOf(matrix, 0, 0);
    matrix3* minor01 = minorOf(matrix, 0, 1);
    matrix3* minor02 = minorOf(matrix, 0, 2);
    matrix3* minor03 = minorOf(matrix, 0, 3);

    float val = (
        + matrix[0][0] * determinant(*minor00)
        - matrix[0][1] * determinant(*minor01)
        + matrix[0][2] * determinant(*minor02)
        - matrix[0][3] * determinant(*minor03)
    );

    // Cleanup.
    delete minor00;
    delete minor01;
    delete minor02;
    delete minor03;

    // Return.
    return val;
}

/*
    Calculates the cross product of two vector3s.
*/
vector3* ExtendedAngel::cross(vector3 &vec1, vector3 &vec2)
{
    return new vector3(
        vec1.y*vec2.z - vec1.z*vec2.y,
        vec1.z*vec2.x - vec1.x*vec2.z,
        vec1.x*vec2.y - vec1.y*vec2.x
    );
}

/*
    Scale affine transformation.
*/
matrix4* ExtendedAngel::scale(float x, float y, float z)
{
    return new matrix4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
}

/*
    Translate affine transormation.
*/
matrix4* ExtendedAngel::translate(float x, float y, float z)
{
    return new matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    );
}

/*
    Rotation around the X-axis.
*/
matrix4* ExtendedAngel::rotateX(float angle)
{
    angle = angle * PI / 180.0;
    float cosA = (float)cos(angle);
    float sinA = (float)sin(angle);

    return new matrix4(
        1,     0,     0, 0,
        0,  cosA,  sinA, 0,
        0, -sinA,  cosA, 0,
        0,     0,     0, 1
    );
}

/*
    Rotation around the Y-axis.
*/
matrix4* ExtendedAngel::rotateY(float angle)
{
    angle = angle * PI / 180.0;
    float cosA = (float)cos(angle);
    float sinA = (float)sin(angle);

    return new matrix4(
        cosA, 0, -sinA, 0,
           0, 1,     0, 0,
        sinA, 0,  cosA, 0,
           0, 0,     0, 1
    );
}

/*
    Rotation around the Z-axis.
*/
matrix4* ExtendedAngel::rotateZ(float angle)
{
    angle = angle * PI / 180.0;
    float cosA = (float)cos(angle);
    float sinA = (float)sin(angle);

    return new matrix4(
         cosA, sinA, 0, 0,
        -sinA, cosA, 0, 0,
            0,    0, 1, 0,
            0,    0, 0, 1
    );
}
