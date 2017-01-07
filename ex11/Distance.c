#include <math.h>
#include "Geometry.h"
#include "Constants.h"


// COMPLETE
// the function dist() should define some object by a distance field;
// this function is then used by the ray-tracing function 
// to produce an image of the modeled object (see file RayTrace.c).
float dist(Vector3 p) {

}


// Approximate the gradient by finite differences
void gradient(Vector3 p, Vector3* g) {
  float fpl_x;
  float fpr_x;
  float fpl_y;
  float fpr_y;
  float fpl_z;
  float fpr_z;
	
  Vector3 pl;
  Vector3 pr;

	
  pl._x = p._x;
  pl._y = p._y;
  pl._z = p._z;
	
  pr._x = p._x;
  pr._y = p._y;
  pr._z = p._z;
	
	
  // partial x
  pl._x = p._x - EPS;
  pr._x = p._x + EPS;
	
  fpl_x = dist(pl);
  fpr_x = dist(pr);
	
  g->_x = (fpr_x - fpl_x) / (2.0f * EPS);
	
  pl._x = p._x;
  pr._x = p._x;


  // partial y
  pl._y = p._y - EPS;
  pr._y = p._y + EPS;
	
  fpl_y = dist(pl);
  fpr_y = dist(pr);
	
  g->_y = (fpr_y - fpl_y) / (2.0f * EPS);
	
  pl._y = p._y;
  pr._y = p._y;
	
	
  // partial z
  pl._z = p._z - EPS;
  pr._z = p._z + EPS;
	
  fpl_z = dist(pl);
  fpr_z = dist(pr);
	
  g->_z = (fpr_z - fpl_z) / (2.0f * EPS);
	
  pl._z = p._z;
  pr._z = p._z;
	
}

