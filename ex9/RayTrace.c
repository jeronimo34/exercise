#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "Scene.h"
#include "RayTrace.h"
#include "Geometry.h"


// Initialize the image with the background color specified as input.
// width and height corresponds to the dimension of the image.
static void initImageWithBackground(Color background_color, Color** image, int width, int height) {
  int i;
  int j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      image[i][j]._red = background_color._red;
      image[i][j]._green = background_color._green;
      image[i][j]._blue = background_color._blue;
    }
  }
}


static float _max(float a, float b) {
  return a > b? a : b;
}


static float _min(float a, float b) {
  return a < b? a : b;
}


// After applying the illumination model, the color's components
// may be greater than 1.0. If this is the case, caps the component 
// value to 1.0.
static void clampColor(Color* c) {
  // Note: _red,_green,_blue are > 0.f by initialization
  c->_red = min(c->_red, 1.0f);
  c->_green = min(c->_green, 1.0f);
  c->_blue = min(c->_blue, 1.0f);
}


// COMPLETE
// Check if the ray defined by (scene._camera, direction) is intersecting
// any of the spheres defined in the scene.
// Return 0 if there is no intersection, and 1 otherwise.
//
// If there is an intersection:
// - the position of the intersection with the closest sphere is computed 
// in hit_pos
// - the normal to the surface at the intersection point in hit_normal
// - the diffuse color and specular color of the intersected sphere
// in hit_color and hit_spec
static int searchIntersectionWithScene(Scene scene, Vector3 direction, Vector3* hit_pos, Vector3* hit_normal, Color* hit_color, Color* hit_spec) {

  // COMPLETE
  
  // For each sphere in the scene
  //   detect if the ray (scene._camera, direction) intersects the current
  //   sphere
  //   if there is more than one sphere intersected by the ray, save the sphere
  //   with the closest intersection point to the camera
	
	
  // Determine the intersection coordinates and the normal to the surface
  // at the intersection point
  
  // Save the color of the intersected sphere in hit_color and hit_spec
}


// Save the image in a raw buffer (texture)
// The memory for texture is allocated in this function. It needs to 
// be freed in the caller.
static void saveRaw(Color** image, int width, int height, GLubyte** texture) {
  int count = 0;
  int i;
  int j;
  *texture = (GLubyte*)malloc(sizeof(GLubyte) * 3 * width * height);
	
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      unsigned char red = image[i][j]._red * 255.0f;
      unsigned char green = image[i][j]._green * 255.0f;
      unsigned char blue = image[i][j]._blue * 255.0f;
			
      (*texture)[count] = red;
      count++;
			
      (*texture)[count] = green;
      count++;
			
      (*texture)[count] = blue;
      count++;
    }
  }
}


// COMPLETE
void rayTrace(Scene scene, int width, int height, GLubyte** texture) {
  Color** image;
  int i;
  int j;
	
  Vector3 camera_pos;
  float screen_scale;
	
	
  image = (Color**)malloc(height * sizeof(Color*));
  for (i = 0; i < height; i++) {
    image[i] = (Color*)malloc(width * sizeof(Color));
  }

	
  // Init the image with the background color
  initImageWithBackground(scene._background_color, image, width, height);
	
	
  // get parameters for the camera position and the screen fov
  camera_pos._x = scene._camera._x;
  camera_pos._y = scene._camera._y;
  camera_pos._z = scene._camera._z;
	
  screen_scale = scene._scale;
	
	
  // COMPLETE
  // go through each pixel and check for intersection between the ray and the scene
      // Form the vector camera to current pixel
      // Check if the ray intersects an object in the scene? (use the function
      //  searchIntersectionWithScene)
      // If the ray intersects an object in the scene
      // Initialize the current pixel color
      // Set the ambient component
			
      // For each light in the scene
        // Form the vector from hit position to light position
        // Compute the diffuse component	
        // Compute the specular component

      // clamp values for the current pixel (use the function clampColor)
  for (int y = 0; y < height; ++y)
  {
	  for (int x = 0; x < width; ++x)
	  {
		  double px = x;
		  double py = y;
		  Vector3 dir = {x-camera_pos._x,y-camera_pos._y,-camera_pos._z},
			      hit_pos, hit_normal, hit_color, hit_spec;
		  
		  int idx = x + y * width;
		  int res = searchIntersectionWithScene(scene, dir, &hit_pos, &hit_normal, &hit_color, &hit_spec);

		  if (res == 1)
		  {
			  //set the ambient component
		  }
		  else
		  {
			//
			
		  }
	  }
  }
	
  // save image to texture buffer
  saveRaw(image, width, height, texture);
	
  for (i = 0; i < height; i++) {
    free(image[i]);
  }
	
  free(image);
}

