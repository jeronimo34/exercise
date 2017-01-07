#include "Geometry.h"


#ifndef PARAMETERS_H
#define PARAMETERS_H



// Color data-structure.
// Used in the definition of the objects in the scene
struct _Color {
  float _red;
  float _green;
  float _blue;
};

typedef struct _Color Color;


// Light data-structure.
struct _Light {
  Color _light_color;
  Vector3 _light_pos;
};

typedef struct _Light Light;


// Parameters for the scene:
// position of the camera, field of view
// position and color of the lights
// color of the implicit
#define MAX_NUMBER_LIGHTS 7
struct _Parameters {
  // Color of the implicit object
  Color _object;
  Color _object_spec;
	
	
  // put background color
  Color _background_color;
	
	
  // camera information
  Vector3 _camera;
  float _scale;

  
  // ambient color for the scene
  Color _ambient;

  
  // light information
  int _number_lights;
  Light _lights[MAX_NUMBER_LIGHTS];  
};

typedef struct _Parameters Parameters;


// Function where all the parameters are set
void setParameters(Parameters* parameters);

#endif
