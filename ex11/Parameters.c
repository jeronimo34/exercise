#include "Parameters.h"


// Assume that memory for the parameters has already been allocated
void setParameters(Parameters* parameters) {
  // Color for the implicit objects
  parameters->_object._red = 0.6f;
  parameters->_object._green = 0.6f;
  parameters->_object._blue = 0.9f;
	
  parameters->_object_spec._red = 1.0f;
  parameters->_object_spec._green = 1.0f;
  parameters->_object_spec._blue = 1.0f;
	
	
	
  // Background color
  parameters->_background_color._red = 1.0f;
  parameters->_background_color._green = 1.0f;
  parameters->_background_color._blue = 1.0f;
	
	
  // camera information
  parameters->_camera._x = 0.0f;
  parameters->_camera._y = 0.0f;
  parameters->_camera._z = 20.0f;
	
  parameters->_scale = 0.03f;
	
	
  // Scene ambient component
  parameters->_ambient._red = 0.2f;
  parameters->_ambient._green = 0.2f;
  parameters->_ambient._blue = 0.2f;

  
  // light information
  parameters->_number_lights = 1;
  
  parameters->_lights[0]._light_pos._x = 1.0f;
  parameters->_lights[0]._light_pos._y = 1.0f;
  parameters->_lights[0]._light_pos._z = 20.0f;
	
  parameters->_lights[0]._light_color._red = 0.6f;
  parameters->_lights[0]._light_color._green = 0.6f;
  parameters->_lights[0]._light_color._blue = 0.6f;
}

