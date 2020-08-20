#ifndef __BODYPART_H_
#define __BODYPART_H_

#include "IDrawable.h"

/*
    Class used to model a part of an articulated body.
*/
class BodyPart: public IDrawable
{
public:
    BodyPart(
        unsigned int id,
        float minX, float maxX, float minY, float maxY, float minZ, float maxZ
    );
    ~BodyPart();
    void applyTransformation(matrix4* transformation);
    matrix4* getTransformation();
    void setTransformation(float x, float y, float z, unsigned int resolution = 1);
    void nextFrame();
    unsigned int getPartID();
    unsigned int getFramesLeft();

private:
    const unsigned int partID;
    unsigned int framesLeft;
    matrix4* transformation;

    // Angles.
    const float minX;
    const float maxX;
    const float minY;
    const float maxY;
    const float minZ;
    const float maxZ;
    float currX;
    float currY;
    float currZ;
    float deltaX;
    float deltaY;
    float deltaZ;

    // Helpers.
    void transformationCalculator();
};

#endif // Define __BODYPART_H_
