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
	float closest_dist = INFINITY;

	Vector3 co;
	float dot;
	float D;

	for (int i = 0; i < scene._number_spheres; ++i)
	{
		Sphere sphere = scene._spheres[i];
		Vector3 xray, xint, xsph, uray, usph;
		float a, b2, c2, d2,d, radius, a_d0, a_d1, a_d;

		radius = sphere._radius;
		uray = direction;
		xsph = sphere._center;
		xray = scene._camera;

		//compute a
		sub(xsph, xray, &usph);
		computeDotProduct(uray, usph, &a);//a

		//compute c2
		computeDotProduct(usph, usph, &c2);//c*c

		//compute b2
		b2 = c2 - a*a;

		//d2
		d2 = radius*radius - b2;

		//no intersection
		if (d2 < 0)continue;
		d = sqrt(d2);
		a_d0 = a - d;
		a_d1 = a + d;

		if (a_d0 < 0 && a_d1 < 0)continue;
		else if (a_d0 < 0)a_d = a_d1;
		else if (a_d1 < 0)a_d = a_d0;
		else a_d = min(a_d0, a_d1);

		if (a_d < closest_dist) {
			//update intersection object
			closest_dist = a_d;

			//compute intersection point
			mulAV(a_d, uray, &xint);
			add(xint, xray, &xint);//tyuui
			*hit_pos = xint;//tyuui

			//compute normal vector
			sub(xint, xsph, hit_normal);
			normalize(*hit_normal, hit_normal);

			//get material color from intersection object.
			*hit_color = sphere._color;
			*hit_spec = sphere._color_spec;
		}

		/*
		Vector3 origin = scene._camera;
		float radius = sphere._radius;
		float ts, te, t;
		
		sub(sphere._center, origin, &co);//ther vector of camera to sphere center.
		computeDotProduct(co, co, &D);//c*c
		computeDotProduct(direction, co, &dot);//a
		D = radius*radius-(D - dot*dot);//r*r - (c*c-a*a)
		
		//no intersection
		if (D < 0.0) continue;
		
		ts = dot - sqrtf(D);
		te = dot + sqrtf(D);

		if (ts < 0 && te < 0)continue;
		else if (ts < 0)t = te;
		else if (te < 0)t = ts;
		else t = min(te, ts);
		
		//intersect object.
		if (t <= closest_dist)
		{
			//get hitpos
			mulAV(t, direction, hit_pos);
			add(*hit_pos, origin, hit_pos);

			// Determine the intersection coordinates and the normal to the surface
			// at the intersection point
			sub(*hit_pos, sphere._center, hit_normal);
			normalize(*hit_normal, hit_normal);

			// Save the color of the intersected sphere in hit_color and hit_spec
			*hit_color = sphere._color;
			*hit_spec = sphere._color_spec;
			
			closest_dist = t;
		}
		*/
	}

	//no intersection.
	if (closest_dist == INFINITY)return 0;

	return 1;
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
		  //float px = 8 * 2 * (x - (width - 1) *0.5) / (width - 1.0);//s * x(i)
		  //float py = 8 * 2 * (y - (height - 1)*0.5) / (height - 1.0);//s * y(i)
		  float px = screen_scale * x - 0.5f * screen_scale * width;
		  float py = screen_scale * y - 0.5f * screen_scale * height;

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
		  //computes the pixel intensity by applying the Phong illumination model.image[y][x]._red = scene._ambient._red * hit_color._red;
		  image[y][x]._red = scene._ambient._red * hit_color._red;
		  image[y][x]._blue = scene._ambient._blue * hit_color._blue;
		  image[y][x]._green = scene._ambient._green * hit_color._green;

		  for (int l = 0; l < scene._number_lights; ++l)
		  {
			  Vector3 light_dir, temp, reflected,hit_to_camera;

			  float dot, spec;

			  sub(scene._lights[l]._light_pos, hit_pos, &light_dir);
			  normalize(light_dir, &light_dir);

			  computeDotProduct(hit_normal, light_dir, &dot);
			  
			  //diffuse component
			  if (dot > 0)
			  {
				  image[y][x]._red += dot * scene._lights[l]._light_color._red * hit_color._red;
				  image[y][x]._blue += dot * scene._lights[l]._light_color._blue * hit_color._blue;
				  image[y][x]._green += dot * scene._lights[l]._light_color._green * hit_color._green;
			  }

			  //specular component
			  mulAV(2.0f*dot, hit_normal, &temp);
			  sub(temp, light_dir, &reflected);

			  hit_to_camera._x = -dir._x;
			  hit_to_camera._y = -dir._y;
			  hit_to_camera._z = -dir._z;

			  computeDotProduct(reflected, hit_to_camera, &spec);
			  spec = powf(max(spec, 0.f), 64.0f);

			  image[y][x]._red += scene._lights[l]._light_color._red * hit_spec._red * spec;
			  image[y][x]._blue += scene._lights[l]._light_color._blue * hit_spec._blue * spec;
			  image[y][x]._green += scene._lights[l]._light_color._green * hit_spec._green * spec;
		  }

		  clampColor(&image[y][x]);
	  }
  }
	
  // save image to texture buffer
  saveRaw(image, width, height, texture);
	
  for (i = 0; i < height; i++) {
    free(image[i]);
  }
	
  free(image);
}

