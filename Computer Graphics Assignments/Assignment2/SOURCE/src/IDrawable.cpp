#include "IDrawable.h"

/*
    Gets the required amount of buffer space needed for this object.
*/
void IDrawable::getRequiredSpace(int* vertexReq, int* indexReq)
{
    setVertexOffset(*vertexReq);
    setIndexOffset(*indexReq);

    *vertexReq += vcLength;
    *indexReq += indexLength;
}

/*
    Updates the given buffer arrays with the current values
    with respect to offset.
*/
void IDrawable::update(
    vector4* vertices, vector3* colors, unsigned int* indices)
{
    int vcOffset = this->getVertexOffset();
    int iOffset = this->getIndexOffset();

    for (int i = 0; i < vcLength; i++) {
        vertices[i + vcOffset] = this->vertices[i];
        colors[i + vcOffset] = this->colors[i];
    }
    for (int i = 0; i < indexLength; i++) {
        indices[i + iOffset] = this->indices[i];
    }
}

// Basic getters and setters.

int IDrawable::getVertexOffset()
{
    return vertexOffset;
}

void IDrawable::setVertexOffset(int offset)
{
    this->vertexOffset = offset;
}

int IDrawable::getIndexOffset()
{
    return indexOffset;
}

void IDrawable::setIndexOffset(int offset)
{
    this->indexOffset = offset;
}
