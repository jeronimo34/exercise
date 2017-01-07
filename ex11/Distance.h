#include "Geometry.h"

#ifndef DISTANCE_H
#define DISTANCE_H


// Return the signed distance (or an approximation) to an implicit
float dist(Vector3 p);

// Compute the gradient at p of the signed distance
void gradient(Vector3 p, Vector3* g);


#endif
