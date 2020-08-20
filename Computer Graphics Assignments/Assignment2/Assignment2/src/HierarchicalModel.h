#ifndef __HIERARCHICALMODEL_H_
#define __HIERARCHICALMODEL_H_

#include "BodyPart.h"
#include "IDrawable.h"
#include <vector>

typedef struct Node {
    BodyPart* part;
    matrix4* tMatrix;
    std::vector<Node*> children;
} Node;

/*
    This class will be hardcoded due to time constraints.
*/
class HierarchicalModel: public IDrawable
{
public:
    HierarchicalModel();
    ~HierarchicalModel();
    Node* getNode(unsigned int id);
    void getRequiredSpace(int* vertexReq, int* indexReq);
    void update(
        vector4* vertices, vector3* colors, unsigned int* indices);

private:
    Node* root;
    void getRequiredSpaceHelper(Node* root, int* vertexReq, int* indexReq);
    void updateHelper(
        Node* root, vector4* vertices, vector3* colors, unsigned int* indices);
    void traverse(Node* &root, matrix4* transformation);

    // Inner classes.
    class Wing;
    class UpperLeg;
    class MiddleLeg;
    class LowerLeg;
    class Torso;

    class Head: public BodyPart {
    public:
        Head(
            unsigned int id,
            float minX, float maxX, float minY,
            float maxY, float minZ, float maxZ
        );
    };
};

#endif // Define __HIERARCHICALMODEL_H_
