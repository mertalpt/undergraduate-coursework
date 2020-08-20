#include "Scene.h"
#include <iostream>

/*
    Scene constructor.
*/
Scene::Scene()
{
    vcLength = 0;
    indexLength = 0;

    // Init data arrays.
    vertices = new vector4[vcLength];
    colors = new vector3[vcLength];
    indices = new unsigned int[indexLength];

    // Create model.
    model;
}

/*
    Scene destructor.
*/
Scene::~Scene()
{
    // Cleanup.
    delete vertices;
    delete colors;
    delete indices;
}

void Scene::getRequiredSpace(int* vertexReq, int* indexReq)
{
    IDrawable::getRequiredSpace(vertexReq, indexReq);
    model.getRequiredSpace(vertexReq, indexReq);
}

void Scene::update(
    vector4* vertices, vector3* colors, unsigned int* indices)
{
    IDrawable::update(vertices, colors, indices);
    model.update(vertices, colors, indices);
}
