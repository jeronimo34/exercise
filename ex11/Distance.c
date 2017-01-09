#include <math.h>
#include "Geometry.h"
#include "Constants.h"


float _max(float o1, float o2)
{
	return (o1> o2) ? o1 : o2;
}
float _min(float o1, float o2)
{
	return (o1<o2) ? o1 : o2;
}

float _mod(float a, float b)
{
	return a - b*floor(a/b);
}

float _modV(Vector3 p, Vector3 c, Vector3* q)
{
	Vector3 res = {
		_mod(p._x, c._x),
		_mod(p._y, c._y), 
		_mod(p._z, c._z)};
	*q = res;
}

static float sdSphere(Vector3 p, float radius)
{
	float dist;
	computeNorm(p, &dist);
	return radius - dist;
}

//operation
static float sdUnion(float o1, float o2)
{
	return (o1> o2) ? o1 : o2;
}

static float sdIntersection(float o1, float o2)
{
	return (o1 < o2) ? o1 : o2;
}

static float sdSubtraction(float o1, float o2)
{
	return _min(o1, -o2);
}

static void translateX(Vector3 p, float trans_x, Vector3* pt)
{
	pt->_x = p._x - trans_x;
	pt->_y = p._y;
	pt->_z = p._z;
}

static void translateZ(Vector3 p, float trans_z, Vector3* pt)
{
	pt->_x = p._x;
	pt->_y = p._y;
	pt->_z = p._z-trans_z;
}

static void replicate(Vector3 p, Vector3 c, Vector3* q)
{
	Vector3 c2;
	_modV(p, c, q);
	mulAV(0.5f, c, &c2);
	sub(*q, c2, q);
}

// COMPLETE
// the function dist() should define some object by a distance field;
// this function is then used by the ray-tracing function 
// to produce an image of the modeled object (see file RayTrace.c).
float dist(Vector3 p) {
	float tdist;
	Vector3 p2;
	Vector3 c = { 10,10,10 };

	replicate(p, c, &p);
	float sph0 = sdSphere(p, 0.5);
	tdist = sph0;// sdSubtraction(sph0, sph1);

	return tdist;
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

