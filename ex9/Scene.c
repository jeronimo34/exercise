#include "Scene.h"


// Assume that memory for scene has already been allocated
void createScene(Scene* scene) {
  // Set the number of objects
  scene->_number_spheres = 3;
	
  // Set property of each object:
	
  // Sphere 1:
  (scene->_spheres[0])._radius = 2.0f;
	
  (scene->_spheres[0])._center._x = 0.0f;
  (scene->_spheres[0])._center._y = 0.0f;
  (scene->_spheres[0])._center._z = -2.0f;
	
  (scene->_spheres[0])._color._red = 0.5f;
  (scene->_spheres[0])._color._green = 0.5f;
  (scene->_spheres[0])._color._blue = 0.5f;

  (scene->_spheres[0])._color_spec._red = 1.0f;
  (scene->_spheres[0])._color_spec._green = 1.0f;
  (scene->_spheres[0])._color_spec._blue = 1.0f;


  // Sphere 2:
  (scene->_spheres[1])._radius = 1.0f;
	
  (scene->_spheres[1])._center._x = 4.0f;
  (scene->_spheres[1])._center._y = 1.0f;
  (scene->_spheres[1])._center._z = -2.0f;
	
  (scene->_spheres[1])._color._red = 0.9f;
  (scene->_spheres[1])._color._green = 0.5f;
  (scene->_spheres[1])._color._blue = 0.2f;

  (scene->_spheres[1])._color_spec._red = 1.0f;
  (scene->_spheres[1])._color_spec._green = 1.0f;
  (scene->_spheres[1])._color_spec._blue = 1.0f;


  // Sphere 3:
  (scene->_spheres[2])._radius = 0.5f;
	
  (scene->_spheres[2])._center._x = -4.0f;
  (scene->_spheres[2])._center._y = -2.0f;
  (scene->_spheres[2])._center._z = -2.5f;
	
  (scene->_spheres[2])._color._red = 0.2f;
  (scene->_spheres[2])._color._green = 0.9f;
  (scene->_spheres[2])._color._blue = 0.5f;

  (scene->_spheres[2])._color_spec._red = 1.0f;
  (scene->_spheres[2])._color_spec._green = 1.0f;
  (scene->_spheres[2])._color_spec._blue = 1.0f;
  
  // Sphere 4:
  (scene->_spheres[3])._radius = 1.5f;

  (scene->_spheres[3])._center._x = -2.0f;
  (scene->_spheres[3])._center._y = 1.0f;
  (scene->_spheres[3])._center._z = -1.0f;

  (scene->_spheres[3])._color._red = 0.2f;
  (scene->_spheres[3])._color._green = 0.9f;
  (scene->_spheres[3])._color._blue = 0.5f;

  (scene->_spheres[3])._color_spec._red = 1.0f;
  (scene->_spheres[3])._color_spec._green = 1.0f;
  (scene->_spheres[3])._color_spec._blue = 1.0f;


  // Background color
  scene->_background_color._red = 1.0f;
  scene->_background_color._green = 1.0f;
  scene->_background_color._blue = 1.0f;
	
	
  // camera information
  scene->_camera._x = 0.0f;
  scene->_camera._y = 0.0f;
  scene->_camera._z = 10.0f;
	
  scene->_scale = 0.03f;
	
	
  // ambient color
  scene->_ambient._red = 0.2f;
  scene->_ambient._green = 0.2f;
  scene->_ambient._blue = 0.2f;
	

  // lights information
  scene->_number_lights = 1;

  (scene->_lights[0])._light_pos._x = 1.0f;
  (scene->_lights[0])._light_pos._y = 1.0f;
  (scene->_lights[0])._light_pos._z = 10.0f;

  (scene->_lights[0])._light_color._red = 0.6f;
  (scene->_lights[0])._light_color._green = 0.6f;
  (scene->_lights[0])._light_color._blue = 0.6f;

  //light 1
  (scene->_lights[1])._light_pos._x = -10.0f;
  (scene->_lights[1])._light_pos._y = -10.0f;
  (scene->_lights[1])._light_pos._z = 10.0f;

  (scene->_lights[1])._light_color._red = 0.1f;
  (scene->_lights[1])._light_color._green = 0.1f;
  (scene->_lights[1])._light_color._blue = 0.6f;
}


// Assume that memory for scene has already been allocated
void createSceneOneSphere(Scene* scene) {
  // Set the number of objects
  scene->_number_spheres = 1;
	
  // Set property of each object:
	
  // Sphere 1:
  (scene->_spheres[0])._radius = 2.0f;
	
  (scene->_spheres[0])._center._x = 0.0f;
  (scene->_spheres[0])._center._y = 0.0f;
  (scene->_spheres[0])._center._z = -2.0f;
	
  (scene->_spheres[0])._color._red = 0.5f;
  (scene->_spheres[0])._color._green = 0.5f;
  (scene->_spheres[0])._color._blue = 0.5f;

  (scene->_spheres[0])._color_spec._red = 1.0f;
  (scene->_spheres[0])._color_spec._green = 1.0f;
  (scene->_spheres[0])._color_spec._blue = 1.0f;


  // Background color
  scene->_background_color._red = 1.0f;
  scene->_background_color._green = 1.0f;
  scene->_background_color._blue = 1.0f;
	
	
  // camera information
  scene->_camera._x = 0.0f;
  scene->_camera._y = 0.0f;
  scene->_camera._z = 10.0f;
	
  scene->_scale = 0.03f;
	
	
  // ambient color
  scene->_ambient._red = 0.2f;
  scene->_ambient._green = 0.2f;
  scene->_ambient._blue = 0.2f;
	

  // lights information
  scene->_number_lights = 1;
	
  (scene->_lights[0])._light_pos._x = 1.0f;
  (scene->_lights[0])._light_pos._y = 1.0f;
  (scene->_lights[0])._light_pos._z = 10.0f;
	
  (scene->_lights[0])._light_color._red = 0.6f;
  (scene->_lights[0])._light_color._green = 0.6f;
  (scene->_lights[0])._light_color._blue = 0.6f;
}


// Scene with one sphere and two lights
void createSceneOneSphereTwoLights(Scene* scene) {
  // Set the number of objects
  scene->_number_spheres = 1;
	
  // Set property of each object:
	
  // Sphere 1:
  (scene->_spheres[0])._radius = 2.0f;
	
  (scene->_spheres[0])._center._x = 0.0f;
  (scene->_spheres[0])._center._y = 0.0f;
  (scene->_spheres[0])._center._z = -2.0f;
	
  (scene->_spheres[0])._color._red = 0.5f;
  (scene->_spheres[0])._color._green = 0.5f;
  (scene->_spheres[0])._color._blue = 0.5f;
	
  (scene->_spheres[0])._color_spec._red = 1.0f;
  (scene->_spheres[0])._color_spec._green = 1.0f;
  (scene->_spheres[0])._color_spec._blue = 1.0f;
	
	
  // Background color
  scene->_background_color._red = 1.0f;
  scene->_background_color._green = 1.0f;
  scene->_background_color._blue = 1.0f;
	
	
  // camera information
  scene->_camera._x = 0.0f;
  scene->_camera._y = 0.0f;
  scene->_camera._z = 10.0f;
	
  scene->_scale = 0.03f;
	
	
  // ambient color
  scene->_ambient._red = 0.2f;
  scene->_ambient._green = 0.2f;
  scene->_ambient._blue = 0.2f;
	
	
  // lights information
  scene->_number_lights = 2;
	
  (scene->_lights[0])._light_pos._x = 1.0f;
  (scene->_lights[0])._light_pos._y = 1.0f;
  (scene->_lights[0])._light_pos._z = 10.0f;
	
  (scene->_lights[0])._light_color._red = 0.6f;
  (scene->_lights[0])._light_color._green = 0.6f;
  (scene->_lights[0])._light_color._blue = 0.6f;

	
  (scene->_lights[1])._light_pos._x = 1.0f;
  (scene->_lights[1])._light_pos._y = -5.0f;
  (scene->_lights[1])._light_pos._z = 10.0f;
	
  (scene->_lights[1])._light_color._red = 0.6f;
  (scene->_lights[1])._light_color._green = 0.6f;
  (scene->_lights[1])._light_color._blue = 0.6f;
}

