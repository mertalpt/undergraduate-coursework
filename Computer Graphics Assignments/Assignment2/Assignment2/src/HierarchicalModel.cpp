#include "HierarchicalModel.h"

/*
    Model constructor.
*/
HierarchicalModel::HierarchicalModel()
{
    this->vcLength = 0;
    this->indexLength = 0;

    root = new Node;
    root->part = new Head(0, -20, 20, -40, 40, -5, 5);


}

/*
    Model deconstructor.
*/
HierarchicalModel::~HierarchicalModel()
{
    // To do.
}

// Node* getNode(unsigned int id);
void HierarchicalModel::getRequiredSpace(int* vertexReq, int* indexReq)
{
    IDrawable::getRequiredSpace(vertexReq, indexReq);

    // Call on the tree too.
    getRequiredSpaceHelper(root, vertexReq, indexReq);
}

void HierarchicalModel::getRequiredSpaceHelper(
    Node* root, int* vertexReq, int* indexReq)
{
    root->part->getRequiredSpace(vertexReq, indexReq);

    // Traverse the tree.
    for (
        std::vector<Node*>::iterator it = root->children.begin();
        it != root->children.end();
        ++it
    ) {
        getRequiredSpaceHelper(*it, vertexReq, indexReq);
    }
}

void HierarchicalModel::update(
    vector4* vertices, vector3* colors, unsigned int* indices)
{
    IDrawable::update(vertices, colors, indices);

    // Call on the tree too.
    updateHelper(root, vertices, colors, indices);
}

void HierarchicalModel::updateHelper(
    Node* root, vector4* vertices, vector3* colors, unsigned int* indices)
{
    root->part->update(vertices, colors, indices);

    // Traverse the tree.
    for (
        std::vector<Node*>::iterator it = root->children.begin();
        it != root->children.end();
        ++it
    ) {
        updateHelper(*it, vertices, colors, indices);
    }
}

// void traverse(Node* &root, matrix4* transformation);

// class Wing: public BodyPart;
// class UpperLeg: public BodyPart;
// class MiddleLeg: public BodyPart;
// class LowerLeg: public BodyPart;
// class Torso: public BodyPart;

// Only override the constructor.
HierarchicalModel::Head::Head(
    unsigned int id,
    float minX, float maxX, float minY, float maxY, float minZ, float maxZ
): BodyPart(id, minX, maxX, minY, maxY, minZ, maxZ)
{
    // Define vertices and colors and indices.
    this->vcLength = 4;
    this->indexLength = 18;

    this->vertices = new vector4[vcLength];
    this->colors = new vector3[vcLength];
    this->indices = new unsigned int[indexLength];

    // Fill the data.

    // Base of the head.
    vertices[0] = vector4(0, 0, 0, 1);
    colors[0] = vector3(0, 0, 0);

    // Tip of the head
    vertices[1] = vector4(0, -0.1, 0.5, 1);
    colors[1] = vector3(0.1, 0.1, 0.1);

    // Sides of the head.
    vertices[2] = vector4(-0.2, 0.2, 0.2, 1);
    colors[2] = vector3(0.2, 0.2, 0.2);

    vertices[3] = vector4(0.2, 0.2, 0.2, 1);
    colors[3] = vector3(0.2, 0.2, 0.2);

    vertices[4] = vector4(0, -0.2, 0.2, 1);
    colors[4] = vector3(0.2, 0.2, 0.2);

    // Indices.
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 3;

    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 4;

    indices[6] = 0;
    indices[7] = 4;
    indices[8] = 2;

    indices[9] = 1;
    indices[10] = 2;
    indices[11] = 3;

    indices[12] = 1;
    indices[13] = 3;
    indices[14] = 4;

    indices[15] = 1;
    indices[16] = 4;
    indices[17] = 2;
}
