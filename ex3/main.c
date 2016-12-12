/*
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
*/

#include <gl/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

GLfloat vertices[][3] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ 1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

GLfloat colors[][4] = { { 0.0,0.0,0.0 },{ 1.0,0.0,0.0 },
{ 1.0,1.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 },
{ 1.0,0.0,1.0 },{ 1.0,1.0,1.0 },{ 0.0,1.0,1.0 } };

void rotateCube(int, GLfloat);
void translateCube(int, GLfloat);
void scaleCube(int, GLfloat);

float g_pos[3] = { 0, 0, 0 };
float g_angle[3] = { 0, 0, 0 };
float g_scale[3] = { 1, 1, 1 };
void (*g_transform_func)(int, GLfloat) = translateCube;

void polygon(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glVertex3fv(vertices[a]);
	glColor3fv(colors[b]);
	glVertex3fv(vertices[b]);
	glColor3fv(colors[c]);
	glVertex3fv(vertices[c]);
	glColor3fv(colors[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube(void)
{
	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);
}

void drawAxes(void)
{
	/* Red X axis */
	/* Green Y axis */
	/* Blue Z axis */
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();

	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();

}


static void setProjection(const int width, const int height) {
	// COMPLETE
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.1, 20);
}

void display(void)
{
	//clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// init modelview matrix to identity
	glLoadIdentity();

	//view 
	gluLookAt(3,3,3,0,0,0,0,1,0);

	//world
	//draw models
	drawAxes();
	
	glTranslated(g_pos[0], g_pos[1], g_pos[2]);
	glRotated(g_angle[0], 1, 0, 0);
	glRotated(g_angle[1], 0, 1, 0);
	glRotated(g_angle[2], 0, 0, 1);
	glScaled(g_scale[0], g_scale[1], g_scale[2]);

	colorcube();

	//swap buffer
	glutSwapBuffers();
}

//added
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection(w, h);
	glMatrixMode(GL_MODELVIEW);
}

void rotateCube(int axis, GLfloat amount)
{
	g_angle[axis] += amount;
}

void translateCube(int axis, GLfloat amount)
{
	g_pos[axis] += amount;
}

void scaleCube(int axis, GLfloat amount)
{
	g_scale[axis] += amount;
}

//added
void key(unsigned char k, int x, int y)
{
	if (k == 'q' || k == 27) exit(0);
	/* Actions for keys t,r,s;  x,X, y,Y, z,Z */

	switch (k)
	{
	case 't':
		g_transform_func = translateCube;
		break;
	case 'r':
		g_transform_func = rotateCube;
		break;
	case 's':
		g_transform_func = scaleCube;
		break;
	case 'x':
		// move to -x
		(*g_transform_func)(0, -1);
		break;
	case'X':
		(*g_transform_func)(0, 1);
		break;
	case 'y':
		(*g_transform_func)(1, -1);
		break;
	case 'Y':
		(*g_transform_func)(1, 1);
		break;
	case 'z':
		(*g_transform_func)(2, -1);
		break;
	case 'Z':
		(*g_transform_func)(2, 1);
		break;
	}
	glutPostRedisplay();

	//show infomations.
	system("@cls||clear");
	
	if (g_transform_func == translateCube)
	{
		printf("TRANSLATE:\n");
	}
	else if (g_transform_func == rotateCube)
	{
		printf("ROTATE:\n");
	}
	else
	{
		printf("SCALE:\n");
	}
	
	printf("pos:(%f, %f, %f)\n", g_pos[0], g_pos[1], g_pos[2]);
	printf("rotate:(%f, %f, %f)\n", g_angle[0], g_angle[1], g_angle[2]);
	printf("scale:(%f, %f, %f)\n", g_scale[0], g_scale[1], g_scale[2]);
}

//add
void init()
{
	int width, height;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection(width, height);
	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Transformations");

	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//add
	init();

	glutMainLoop();
}