#include "BodyPart.h"
#include "ExtendedAngel.h"

/*
    Body part constructor.
*/
BodyPart::BodyPart(
    unsigned int id,
    float minX, float maxX, float minY, float maxY, float minZ, float maxZ
): partID(id), minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ)
{
    // Set up IDrawable values to safe ones.
    this->vcLength = 0;
    this->indexLength = 0;

    // Initial values.
    this->currX = 0;
    this->currY = 0;
    this->currZ = 0;

    this->deltaX = 0;
    this->deltaY = 0;
    this->deltaZ = 0;

    this->framesLeft = 0;
}

/*
    Body part deconstructor.
*/
BodyPart::~BodyPart()
{
    delete transformation;
    delete vertices;
    delete colors;
    delete indices;
}

/*
    Used to apply transformations from the hierarchy.
*/
void BodyPart::applyTransformation(matrix4* transformation)
{
    matrix4* combT = ExtendedAngel::multiply(
        *getTransformation(), *transformation
    );

    // Transform the vertices.
    vector4* temp = new vector4[vcLength];

    for (int i = 0; i < vcLength; i++) {
        temp[i] = (*combT) * vertices[i];
    }

    // Cleanup.
    delete vertices;
    vertices = temp;
}

/*
    Gets the transformation inside.
*/
matrix4* BodyPart::getTransformation()
{
    return this->transformation;
}

/*
    Sets the transformation parameters.
*/
void BodyPart::setTransformation(float x, float y, float z, unsigned int resolution)
{
    deltaX = (x - currX) / resolution;
    deltaY = (y - currY) / resolution;
    deltaZ = (z - currZ) / resolution;
    framesLeft = resolution;
}

/*
    Gets the next frame of the animation.
*/
void BodyPart::nextFrame()
{
    if (framesLeft > 0) {
        framesLeft--;

        currX += deltaX;
        currY += deltaY;
        currZ += deltaZ;
    }
    transformationCalculator();
}

/*
    ID getter.
*/
unsigned int BodyPart::getPartID()
{
    return this->partID;
}

/*
    Returns the number of frames left for the animation on this model.
*/
unsigned int BodyPart::getFramesLeft()
{
    return this->framesLeft;
}

/*
    Calculates the transformation matrix from curr X,Y,Z values.
*/
void BodyPart::transformationCalculator()
{
    matrix4* temp1 = ExtendedAngel::rotateZ(currZ);
    matrix4* temp2 = ExtendedAngel::rotateY(currY);
    matrix4* temp3 = ExtendedAngel::multiply(*temp1, *temp2);
    delete temp1;
    delete temp2;
    temp1 = temp3;
    temp2 = ExtendedAngel::rotateX(currX);
    temp3 = ExtendedAngel::multiply(*temp3, *temp2);
    delete temp1;
    delete temp2;

    if (transformation != NULL) {
        delete transformation;
    }
    transformation = temp3;
}
