#define _USE_MATH_DEFINES
#include <math.h>
#include "Geometry.h"
#include "Constants.h"

#define INV_PI M_PI/180.0

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

//min_radius = パイプの半径
//max_radius = トーラスの中心からどのくらいの距離をおいてパイプを作るかの距離
static float sdTorus(Vector3 p, float min_radius, float max_radius)
{
	Vector3 pxz, q;
	float lpxz, lq;

	pxz._x = p._x;
	pxz._y = p._z;
	pxz._z = 0.0f;
	computeNorm(pxz, &lpxz);

	q._x = lpxz - max_radius;
	q._y = p._y;
	q._z = 0;
	
	computeNorm(q, &lq);
	return -(lq - min_radius);
}

static float sdPlane(Vector3 p, Vector3 normal, float dist)
{
	float dot;
	computeDotProduct(p, normal, &dot);
	return -(dist + dot);
}

static float sdCone(Vector3 p, float height, float radius)
{
	Vector3 pxy;
	Vector3 parameters;
	Vector3 normalized_parameters;
	float q;
	Vector3 qpz;
	float dot;

	parameters._x = height;
	parameters._y = radius;
	parameters._z = 0;
	//normalize(parameters, &normalized_parameters);
	normalized_parameters = parameters;

	pxy._x = p._x;
	pxy._y = p._y;
	pxy._z = 0;
	computeNorm(pxy, &q);

	qpz._x = q;
	qpz._y = p._z;
	qpz._z = 0;

	computeDotProduct(normalized_parameters, qpz, &dot);
	return -dot;
}

static float sdBox(Vector3 p, Vector3 dim)
{
	Vector3 d, d2;
	float dist, norm_d2;
	d._x = fabs(p._x) - dim._x;
	d._y = fabs(p._y) - dim._y;
	d._z = fabs(p._z) - dim._z;

	d2._x = _max(d._x, 0.0f);
	d2._y = _max(d._y, 0.0f);
	d2._z = _max(d._z, 0.0f);

	computeNorm(d2, &norm_d2);
	dist = _min(_max(d._x, _max(d._y, d._z)), 0) + norm_d2;
	return -dist;
}

static float sdCylinder(Vector3 p, float radius)
{
	Vector3 q;
	float d2;
	q._x = p._x;
	q._y = p._y;
	q._z = 0;

	computeNorm(q, &d2);
	return radius - d2;
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
	pt->_z = p._z - trans_z;
}

static void translateY(Vector3 p, float trans_y, Vector3* pt)
{
	pt->_x = p._x;
	pt->_y = p._y - trans_y;
	pt->_z = p._z;
}

static void rotateX(Vector3 in, float angle, Vector3* out)
{
	//2PI : 360 = x : angle
	//x = 2PI * angle/360
	//x = angle * PI/180

	float ca = cos(-angle * INV_PI);
	float sa = sin(-angle * INV_PI);
	out->_x = in._x;
	out->_y = ca * in._y - sa*in._z;
	out->_z = sa * in._y + ca * in._z;
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
	//cone

	Vector3 dim = { 2, 1, 2 };
	rotateX(p, -90, &p);
	return sdIntersection(sdCone(p, 1, 0.5), sdBox(p,dim));//?

	/*
	//

	rotateX(p, -90, &p);
	return sdCylinder(p, 3);

	
	//box
	Vector3 dim = { 2, 2, 2 };
	return sdBox(p, dim);
	*/
	//plane
	Vector3 normal = { 0,1,0 };
	return sdPlane(p, normal, 3);

	Vector3 p2;
	float tdist;

	translateZ(p, -2, &p); 
	tdist = sdSphere(p, 3);
	
	translateZ(p, 3, &p2);
	Vector3 eyepos, eyepos2;
	{
		//eyes
		rotateX(p2, 90, &eyepos);
		translateX(eyepos, -1, &eyepos);

		tdist = sdUnion(tdist, sdTorus(eyepos, 0.1, 0.7));
		translateZ(eyepos, -0.2, &eyepos);
		tdist = sdUnion(tdist, sdSphere(eyepos, 0.5));

		rotateX(p2, 90, &eyepos);
		translateX(eyepos, 1, &eyepos);

		tdist = sdUnion(tdist, sdTorus(eyepos, 0.1, 0.7));
		translateZ(eyepos, -0.2, &eyepos);
		tdist = sdUnion(tdist, sdSphere(eyepos, 0.5));
	}

	Vector3 mousepos;
	{
		//mouse
		translateY(p2, -2, &mousepos);
		tdist = sdSubtraction(tdist, sdSphere(mousepos, 1));
	}

	Vector3 bodypos;
	{
		translateY(p, -6, &bodypos);
		tdist = sdUnion(tdist, sdSphere(bodypos, 4));
	}

	
	
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