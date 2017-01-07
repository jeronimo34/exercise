#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Parameters.h"


#ifndef RAYTRACE_H
#define RAYTRACE_H

void rayTrace(Parameters parameters, int width, int height, GLubyte** texture);

#endif
