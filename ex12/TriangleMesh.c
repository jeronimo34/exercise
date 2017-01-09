#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>

#include "TriangleMesh.h"


// Some utilities:
static float _min(float a, float b)
{
  return a < b ? a : b;
}


static float _max(float a, float b)
{
  return a > b ? a : b;
}


// COMPLETE
// Read a triangle mesh stored in a file using the OFF file format
void readOFF(const char* filename, TriangleMesh* tri_mesh) 
{
	FILE* fp;
	if ((fp = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "can not open file.\n");
		exit(EXIT_FAILURE);
	}

	char off[256];

	//string "OFF"
	fscanf(fp, "%s", off);
	
	//num_vertices num_triangles num_edge<-iranai
	fscanf(fp, "%d %d %d", &tri_mesh->_number_vertices, &tri_mesh->_number_triangles);
	
	tri_mesh->_vertices = (Vector3*)malloc(sizeof(Vector3) * tri_mesh->_number_vertices);
	tri_mesh->_triangles = (Triple*)malloc(sizeof(Triple) * tri_mesh->_number_triangles);
	tri_mesh->_triangle_normals = (Vector3*)malloc(sizeof(Vector3) * tri_mesh->_number_triangles);
	tri_mesh->_vertex_normals = (Vector3*)malloc(sizeof(Vector3) * tri_mesh->_number_vertices);

	float x, y, z;
	int i,v0,v1,v2,vnum;
	for (i = 0; i < tri_mesh->_number_vertices; ++i)
	{
		fscanf(fp, "%f %f %f", &x, &y, &z);
		tri_mesh->_vertices[i]._x = x;
		tri_mesh->_vertices[i]._y = y;
		tri_mesh->_vertices[i]._z = z;
	}
	for (i = 0; i < tri_mesh->_number_triangles; ++i)
	{
		fscanf(fp, "%d %d %d %d",&vnum, &v0, &v1, &v2);
		tri_mesh->_triangles[i]._v0 = v0;
		tri_mesh->_triangles[i]._v1 = v1;
		tri_mesh->_triangles[i]._v2 = v2;
	}
	fclose(fp);
}


// Compute triangle mesh bounding box
void computeBounds(TriangleMesh* tri_mesh, Vector3* lower_bound, Vector3* upper_bound)
{
  float lower_x = FLT_MAX;
  float lower_y = FLT_MAX;
  float lower_z = FLT_MAX;
  float upper_x = -FLT_MAX;
  float upper_y = -FLT_MAX;
  float upper_z = -FLT_MAX;
	
  int i;
	
  for (i = 0; i < tri_mesh->_number_vertices; i++) {
    lower_x = min(lower_x, tri_mesh->_vertices[i]._x);
    lower_y = min(lower_y, tri_mesh->_vertices[i]._y);
    lower_z = min(lower_z, tri_mesh->_vertices[i]._z);
		
    upper_x = max(upper_x, tri_mesh->_vertices[i]._x);
    upper_y = max(upper_y, tri_mesh->_vertices[i]._y);
    upper_z = max(upper_z, tri_mesh->_vertices[i]._z);
  }

  lower_bound->_x = lower_x;
  lower_bound->_y = lower_y;
  lower_bound->_z = lower_z;

  upper_bound->_x = upper_x;
  upper_bound->_y = upper_y;
  upper_bound->_z = upper_z;
}


// Compute center of mass of the vertices
void computeCenterMass(TriangleMesh* tri_mesh, Vector3* center)
{
  int i;
	
  center->_x = 0.f;
  center->_y = 0.f;
  center->_z = 0.f;
	
  for (i = 0; i < tri_mesh->_number_vertices; i++) {
    center->_x += tri_mesh->_vertices[i]._x;
    center->_y += tri_mesh->_vertices[i]._y;
    center->_z += tri_mesh->_vertices[i]._z;
  }
	
  center->_x /= tri_mesh->_number_vertices;
  center->_y /= tri_mesh->_number_vertices;
  center->_z /= tri_mesh->_number_vertices;
}


// Translate a triangle mesh such that its center of mass is aligned
// with the origin of the coordinate system
void centerTriangleMesh(TriangleMesh* tri_mesh)
{
  int i;
	
  // compute center of mass
  Vector3 center;
  computeCenterMass(tri_mesh, &center);
	
  // translate each vertex by -c
  for (i = 0; i < tri_mesh->_number_vertices; i++) {
    tri_mesh->_vertices[i]._x -= center._x;
    tri_mesh->_vertices[i]._y -= center._y;
    tri_mesh->_vertices[i]._z -= center._z;
  }
}


// Rescale a triangle mesh such that it has a bounding
// box with unit length diagonal
void normalizeTriangleMesh(TriangleMesh* tri_mesh)
{
  int i;
	
  Vector3 lower;
  Vector3 upper;
  Vector3 diagonal;
	
  float diagonal_length;
	
	
  computeBounds(tri_mesh, &lower, &upper);
  sub(upper, lower, &diagonal);
  computeNorm(diagonal, &diagonal_length);
	
  assert(diagonal_length != 0.f);
	
	
  for (i = 0; i < tri_mesh->_number_vertices; i++) {
    tri_mesh->_vertices[i]._x *= 1.0f/diagonal_length;
    tri_mesh->_vertices[i]._y *= 1.0f/diagonal_length;
    tri_mesh->_vertices[i]._z *= 1.0f/diagonal_length;
  }
}


// COMPLETE
// Precompute normal to each triangle
void computeTriangleNormals(TriangleMesh* tri_mesh)
{
	Vector3 p0, p1, p2, normal;
	for (int i = 0; i < tri_mesh->_number_triangles; ++i)
	{
		p0 = tri_mesh->_vertices[tri_mesh->_triangles[i]._v0];
		p1 = tri_mesh->_vertices[tri_mesh->_triangles[i]._v1];
		p2 = tri_mesh->_vertices[tri_mesh->_triangles[i]._v2];
		sub(p1, p0, &p1);
		sub(p2, p0, &p2);
		computeCrossProduct(p1, p2, &normal);
		normalize(normal, &normal);
		tri_mesh->_triangle_normals[i] = normal;
	}
}


// COMPLETE
// Precompute normal to the surface at each vertex
void computeVertexNormals(TriangleMesh* tri_mesh)
{
	Vector3 normal = {0,0,0};
	for (int i = 0; i < tri_mesh->_number_vertices; ++i)
	{
		//init
		tri_mesh->_vertex_normals[i] = normal;
	}

	for (int i = 0; i < tri_mesh->_number_triangles; ++i)
	{
		//sum of normals
		normal = tri_mesh->_triangle_normals[i];
		add(tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v0], normal, &tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v0]);
		add(tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v1], normal, &tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v1]);
		add(tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v2], normal, &tri_mesh->_vertex_normals[tri_mesh->_triangles[i]._v2]);		
	}

	for (int i = 0; i < tri_mesh->_number_vertices; ++i)
	{
		normalize(tri_mesh->_vertex_normals[i], &tri_mesh->_vertex_normals[i]);
	}
}


// Access the coordinates of the three vertices of the i-th triangle
void getTriangleVertices(TriangleMesh* tri_mesh, int i, Vector3 coordinates[3])
{
  assert (i >= 0);
  assert (i < tri_mesh->_number_triangles);
  assert(tri_mesh->_triangles);
  assert(tri_mesh->_vertices);

	
  int v0 = tri_mesh->_triangles[i]._v0;
  int v1 = tri_mesh->_triangles[i]._v1;
  int v2 = tri_mesh->_triangles[i]._v2;
	
  coordinates[0]._x = tri_mesh->_vertices[v0]._x;
  coordinates[0]._y = tri_mesh->_vertices[v0]._y;
  coordinates[0]._z = tri_mesh->_vertices[v0]._z;
	
  coordinates[1]._x = tri_mesh->_vertices[v1]._x;
  coordinates[1]._y = tri_mesh->_vertices[v1]._y;
  coordinates[1]._z = tri_mesh->_vertices[v1]._z;
	
  coordinates[2]._x = tri_mesh->_vertices[v2]._x;
  coordinates[2]._y = tri_mesh->_vertices[v2]._y;
  coordinates[2]._z = tri_mesh->_vertices[v2]._z;
}


// Access the normals to each of the three vertices of the i-th triangle
void getTriangleVertexNormals(TriangleMesh* tri_mesh, int i, Vector3 normals[3]) 
{
  assert (i >= 0);
  assert (i < tri_mesh->_number_triangles);
  assert(tri_mesh->_triangles);
  assert(tri_mesh->_vertex_normals);
	
  int v0 = tri_mesh->_triangles[i]._v0;
  int v1 = tri_mesh->_triangles[i]._v1;
  int v2 = tri_mesh->_triangles[i]._v2;
	
  normals[0]._x = tri_mesh->_vertex_normals[v0]._x;
  normals[0]._y = tri_mesh->_vertex_normals[v0]._y;
  normals[0]._z = tri_mesh->_vertex_normals[v0]._z;

  normals[1]._x = tri_mesh->_vertex_normals[v1]._x;
  normals[1]._y = tri_mesh->_vertex_normals[v1]._y;
  normals[1]._z = tri_mesh->_vertex_normals[v1]._z;

  normals[2]._x = tri_mesh->_vertex_normals[v2]._x;
  normals[2]._y = tri_mesh->_vertex_normals[v2]._y;
  normals[2]._z = tri_mesh->_vertex_normals[v2]._z;
}


// Access the (pre-computed) face normal to the i-th triangle
void getTriangleNormal(TriangleMesh* tri_mesh, int i, Vector3* normal)
{
  assert (i >= 0);
  assert (i < tri_mesh->_number_triangles);
  assert(tri_mesh->_triangle_normals);
	
  // is it better to return a pointer inside _face_normal ?
  normal->_x = tri_mesh->_triangle_normals[i]._x;
  normal->_y = tri_mesh->_triangle_normals[i]._y;
  normal->_z = tri_mesh->_triangle_normals[i]._z;
}


// Access the (pre-computed) vertex normal for the i-th vertex
void getVertexNormal(TriangleMesh* tri_mesh, int i, Vector3* normal)
{
  assert(i >= 0);
  assert(i < tri_mesh->_number_vertices);
  assert(tri_mesh->_vertex_normals);

  normal->_x = tri_mesh->_vertex_normals[i]._x;
  normal->_y = tri_mesh->_vertex_normals[i]._y;
  normal->_z = tri_mesh->_vertex_normals[i]._z;
}


void getNumberTriangles(TriangleMesh* tri_mesh, int* num_tris) 
{
  *num_tris = tri_mesh->_number_triangles;
}


void getNumberVertices(TriangleMesh* tri_mesh, int* num_verts)
{
  *num_verts = tri_mesh->_number_vertices;
}


// Free memory used by the triangle mesh
void freeTriangleMeshStructures(TriangleMesh* tri_mesh)
{
  free(tri_mesh->_vertices);
  free(tri_mesh->_triangles);
  if (tri_mesh->_triangle_normals) free(tri_mesh->_triangle_normals);
  if (tri_mesh->_vertex_normals) free(tri_mesh->_vertex_normals);
}

