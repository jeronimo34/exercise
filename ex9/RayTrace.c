#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

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
	Vector3 Usph, Xint, a_dUray, cameraTohitpos;
	float a, b2, c, d, dot_res, r, eff;
	float closest_dist = INFINITY, tdist;

	for (int i = 0; i < scene._number_spheres; ++i)
	{
		Sphere sphere = scene._spheres[i];
		Vector3 cameraPos = scene._camera;
		
		sub(sphere._center, cameraPos, &Usph);
		computeNorm(Usph, &c);
		normalize(Usph, &Usph);
		computeDotProduct(direction, Usph, &dot_res);

		a = c * dot_res;
		b2 = c*c - a*a;
		r = sphere._radius;

		if (r*r - b2 >= 0) {
			//intersect object.
			d = sqrt(r*r - b2);
			eff = a - d;

			//Xint is position of intersection.
			//Xint = Xray + (a-d)Uray.
			mulAV(eff, direction, &a_dUray);
			add(cameraPos, a_dUray, &Xint);

			
			sub(Xint, cameraPos, &cameraTohitpos);
			computeNorm(cameraTohitpos,&tdist);
			
			if (tdist <= closest_dist)
			{
				closest_dist = tdist;

				// Determine the intersection coordinates and the normal to the surface
				// at the intersection point
				(*hit_pos) = Xint;
				sub(Xint, sphere._center, hit_normal);
				normalize(*hit_normal, hit_normal);

				// Save the color of the intersected sphere in hit_color and hit_spec
				*hit_color = sphere._color;
				*hit_spec = sphere._color_spec;
			}
		}
	}

	if (closest_dist != INFINITY)return 1;
	//no intersection.
	return 0;
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
		  float px = 8*2 * (x - (width - 1) / 2.0) / (width - 1.0);//s * x(i)
		  float py = 8*2 * (y - (height - 1) / 2.0) / (height - 1.0);//s * y(i)

		  Vector3 dir;
		  Vector3 hit_pos, hit_normal;
		  Color hit_color, hit_spec;
		  dir._x = px - camera_pos._x;
		  dir._y = py - camera_pos._y;
		  dir._z = -camera_pos._z;

		  normalize(dir, &dir);

		  int intersected = searchIntersectionWithScene(scene, dir, &hit_pos, &hit_normal, &hit_color, &hit_spec);
		  if (!intersected)continue;

		    //set the ambient component
			  //computes the pixel intensity by applying the Phong illumination model.
			  Light light = scene._lights[0];
			  Color l_color = light._light_color;
			  Vector3 l_pos = light._light_pos;

			  float ln, rv, shineness = 128, l_dot_n;
			  Vector3 L, V, R, _2NLN;

			  //Light Vector
			  sub(l_pos, hit_pos,  &L);
			  normalize(L, &L);
			  
			  //Camera Vec
			  sub(camera_pos, hit_pos, &V);
			  normalize(V, &V);

			  //reflected vector
			  //R = L - 2N(L.N).
			  computeDotProduct(L, hit_normal, &l_dot_n);
			  mulAV(2*l_dot_n, hit_normal, &_2NLN);
			  sub(L, _2NLN, &R);

			  //last_color = sigma[Kd(LiN)+Ks(RiV)^n]ILi
			  //LiN
			  computeDotProduct(L, hit_normal, &ln);

			  //RiV
			  computeDotProduct(R, hit_normal, &rv);

			  Color amb = {
				  scene._ambient._red * hit_color._red,
				  scene._ambient._green * hit_color._green,
				  scene._ambient._blue * hit_color._blue
			  };

			  Color diff = {
				  l_color._red * hit_color._red * ln,
				  l_color._green * hit_color._green * ln,
				  l_color._blue * hit_color._blue * ln
			  };

			  Color spec = {
				  l_color._red * hit_spec._red * pow(rv, shineness),
				  l_color._green * hit_spec._green * pow(rv, shineness),
				  l_color._blue * hit_spec._blue * pow(rv, shineness) };

			  Color last_color = {
				  amb._red + diff._red + spec._red,
				  amb._green + diff._green + spec._green,
				  amb._blue + diff._blue + spec._blue};

			  clampColor(&last_color);

			  image[y][x] = last_color;
	  }
  }
	
  // save image to texture buffer
  saveRaw(image, width, height, texture);
	
  for (i = 0; i < height; i++) {
    free(image[i]);
  }
	
  free(image);
}

