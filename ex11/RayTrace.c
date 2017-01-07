#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>

#include "Parameters.h"
#include "RayTrace.h"
#include "Geometry.h"
#include "Distance.h"

#include "Constants.h"


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


static void clampColor(Color* c) {
  // Note: _red,_green,_blue are > 0.f by initialization
  c->_red = min(c->_red, 1.0f);
  c->_green = min(c->_green, 1.0f);
  c->_blue = min(c->_blue, 1.0f);
}


// Detect if there is intersection between the ray (origin, direction) 
// and the sphere (center, radius)
static int searchIntersectionWithSphere(Vector3 origin, Vector3 direction, Vector3 center, float radius, float* ts, float* te) {
  Vector3 co;
  float dot;
  float D;
	
  sub(origin, center, &co);
  computeDotProduct(co, direction, &dot);
  computeDotProduct(co, co, &D);
  D = dot*dot - (D - radius*radius);
	
  // no intersection
  if (D < 0.0) return 0;
	
  *ts = -dot - sqrtf(D);
  *te = -dot + sqrtf(D);
	
  if (*ts < 0) {
    if (*te < 0) return 0; // no intersection
  }
	
  return 1;
}


static void bissect(Vector3 origin, Vector3 direction, float tl, float tr, float* t) {
  int count = 1;
  int count_max = 50;
  float tol = 1e-5f;

  while (count < count_max) {
    Vector3 temp;
    Vector3 p;
    float f;
    Vector3 pl;
    float fl;
				
    *t = (tl + tr) / 2.0f;
    mulAV(*t, direction, &temp);
    add(origin, temp, &p);
    f = dist(p);
		
    if (fabsf(f) < tol || (tr - tl) / 2.0f < tol) {
      break;
    }
		
    count = count + 1;
    mulAV(tl, direction, &temp);
    add(origin, temp, &pl);
    fl = dist(pl);
		
    // if sign f and sign fl are same
    if (f * fl) {
      tl = *t;
    } else {
      tr = *t;
    }
  }	
}


// March along the ray (origin, direction) and search for intersection with the 
// implicit represented by dist
static int intervalSearch(Vector3 origin, Vector3 direction, float ts, float te, float* t) {
  Vector3 temp;
  Vector3 p;
  Vector3 g;
  float norm;
  float fp;
  float dt;
	
  float tl;
  float fpl;
  float tr;
	
	
  mulAV(ts, direction, &temp);
  add(origin, temp, &p);
	
	
  // marching step: dt = |dist(p)| is an approximation of the dist
  // to the surface
  fp = dist(p);
  dt = fabsf(fp);
	
  gradient(p, &g);
  computeNorm(g, &norm);
  if (norm > EPS) dt = dt / norm;
	
	
  tl = ts;
  fpl = fp;
  tr = tl + dt;
	
  while (tr < te) {
    float fpr;
		
    mulAV(tr, direction, &temp);
    add(origin, temp, &p);
		
    fpr = dist(p);
		
    if (fpl * fpr < 0) {
      // find the root by bissection
      bissect(origin, direction, tl, tr, t);
      return 1;
    } else {
      // march along the ray
      tl = tr;
      fpl = fpr;
      dt = fabsf(fpl);
			
      gradient(p, &g);
      computeNorm(g, &norm);
      if (norm > EPS) dt = dt / norm;			
			
      if (dt < EPS) {
        // we are close enough to the surface
        *t = tr;
        return 1;
      }
			
      tr = tr + dt;
    }
  }
	
  return 0;
}


// Detect if there is intersection between the ray given by (parameters._camera, direction) with the distance field 
// computed by distance(p)
static int searchIntersectionWithDistanceField(Vector3 origin, Vector3 direction, Vector3* hit_pos, Vector3* hit_normal) {
  int hit;
  float t;
	
  Vector3 temp;
	
	
  // Detect if there is intersection with a bounding sphere of
  // the implicit object.
  //
  // Compute a conservative bounding sphere: 
  // centered in the origin of the coordinate system
  // and with radius 1/2 the distance from the origin to the camera position
	
  // center and radius of the bounding sphere
  float radius;
  Vector3 center;
  // parameters corresponding to the first and 
  // second intersection of the ray with the bounding sphere
  float ts, te; 


  // Assume the scene is all contained in the sphere of center: the
  // origin of the coordinate system and radius: 1/2 the length of the vector
  // origin to camera location
  center._x = 0.0f;
  center._y = 0.0f;
  center._z = 0.0f;
  computeNorm(origin, &radius);
  radius = 0.5f * radius;
  

  hit = searchIntersectionWithSphere(origin, direction, center, radius, &ts, &te);
  if (!hit) return 0;
	
	
  // If there is, then check if there is intersection with the implicit by marching along the ray
  hit = intervalSearch(origin, direction, ts, te, &t);
  if (!hit) return 0;
	
	
  // There is intersection;
  // compute the position and the outward normal unit vector
  // at the intersection point
  mulAV(t, direction, &temp);
  add(origin, temp, hit_pos);
  gradient(*hit_pos, hit_normal);
  hit_normal->_x = -hit_normal->_x;
  hit_normal->_y = -hit_normal->_y;
  hit_normal->_z = -hit_normal->_z;
  
  normalize(*hit_normal, hit_normal);
	
  return 1;
}


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


void rayTrace(Parameters parameters, int width, int height, GLubyte** texture) {
  Color** image;
  int i;
  int j;
  int l;
	
  Vector3 camera_pos;
  float screen_scale;
	
	
  image = (Color**)malloc(height * sizeof(Color*));
  for (i = 0; i < height; i++) {
    image[i] = (Color*)malloc(width * sizeof(Color));
  }

	
  // Init the image with the background color
  initImageWithBackground(parameters._background_color, image, width, height);
	
	
  // get parameters for the camera position and the screen fov
  camera_pos._x = parameters._camera._x;
  camera_pos._y = parameters._camera._y;
  camera_pos._z = parameters._camera._z;
	
  screen_scale = parameters._scale;
	
	
  // go through each pixel and check for intersection between the ray and the scene
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      float x = screen_scale * j - 0.5f * screen_scale * width;
      float y = screen_scale * i - 0.5f * screen_scale * height;
			
      Vector3 direction;
      Vector3 direction_normalized;
			
      Vector3 hit_pos;
      Vector3 hit_normal;
      Color hit_color;
      Color hit_spec;
      int hit;
			
      Vector3 light_dir;
      Vector3 light_dir_normalized;
			
      float dot;
			
      Vector3 temp;
      Vector3 reflected;
			
      Vector3 hit_to_camera;
      float specular_component;
			
			
      // Vector camera to current pixel
      direction._x = x - camera_pos._x;
      direction._y = y - camera_pos._y;
      direction._z = -camera_pos._z;
			
      normalize(direction, &direction_normalized);
			
			
      // does the ray intersect an object in the scene?
      hit = 
          searchIntersectionWithDistanceField(parameters._camera, direction_normalized, &hit_pos, &hit_normal);
      if (!hit) continue; 
			
			
      // the ray intersect an object in the scene
      image[i][j]._red = 0.f;
      image[i][j]._green = 0.f;
      image[i][j]._blue = 0.f;
      
      
      // ambient component
      image[i][j]._red += parameters._ambient._red * parameters._object._red;
      image[i][j]._green += parameters._ambient._green * parameters._object._green;
      image[i][j]._blue += parameters._ambient._blue * parameters._object._blue;


      // for each light in the scene
      for (l = 0; l < parameters._number_lights; l++) {
        // vector from hit_pos to light_pos
        light_dir._x = parameters._lights[l]._light_pos._x - hit_pos._x;
        light_dir._y = parameters._lights[l]._light_pos._y - hit_pos._y;
        light_dir._z = parameters._lights[l]._light_pos._z - hit_pos._z;
			
        normalize(light_dir, &light_dir_normalized);

        
        computeDotProduct(hit_normal, light_dir_normalized, &dot);
        // diffuse component
        if (dot > 0) {
          image[i][j]._red += dot * parameters._lights[l]._light_color._red * parameters._object._red;
          image[i][j]._green += dot * parameters._lights[l]._light_color._green * parameters._object._green;
          image[i][j]._blue += dot * parameters._lights[l]._light_color._blue * parameters._object._blue;
        }
			
        // specular component
        mulAV(2.0f*dot, hit_normal, &temp);
        sub(temp, light_dir_normalized, &reflected);
			
        hit_to_camera._x = -direction_normalized._x;
        hit_to_camera._y = -direction_normalized._y;
        hit_to_camera._z = -direction_normalized._z;
			
        computeDotProduct(reflected, hit_to_camera, &specular_component);
        specular_component = powf(max(specular_component, 0.f), 64.0f);
        image[i][j]._red += parameters._lights[l]._light_color._red * parameters._object_spec._red * specular_component;
        image[i][j]._green += parameters._lights[l]._light_color._green * parameters._object_spec._green * specular_component;
        image[i][j]._blue += parameters._lights[l]._light_color._blue * parameters._object_spec._blue * specular_component;

      }
      
      // clamp values
      clampColor(&image[i][j]);
    }
  }
  
	
  // save image to texture buffer
  saveRaw(image, width, height, texture);
	
	
  for (i = 0; i < height; i++) {
    free(image[i]);
  }
	
  free(image);
}

