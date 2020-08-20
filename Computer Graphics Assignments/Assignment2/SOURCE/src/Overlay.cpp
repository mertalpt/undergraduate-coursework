#include "Overlay.h"

/*
    Overlay constructor.
*/
Overlay::Overlay()
{
    vcLength = 0;
    indexLength = 0;
}

/*
    Overlay destructor.
*/
Overlay::~Overlay()
{
    // Cleanup.
    delete vertices;
    delete colors;
    delete indices;
}
