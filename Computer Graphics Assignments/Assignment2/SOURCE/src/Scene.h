#ifndef __SCENE_H__
#define __SCENE_H__

#include "IDrawable.h"
#include "HierarchicalModel.h"

/*
    Class used to model and render objects.
*/
class Scene: public IDrawable
{
public:
    Scene();
    ~Scene();
    void getRequiredSpace(int* vertexReq, int* indexReq);
    void update(
        vector4* vertices, vector3* colors, unsigned int* indices);

private:
    HierarchicalModel model;
};

#endif // Define __SCENE_H__
