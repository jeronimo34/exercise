#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
#include <GL/freeglut.h>
*/

int g_projection_id = 0;
int g_use_lookat = 0;
// COMPLETE
// in this function, you need to write the code corresponding to 
// drawing the 3 axes in color.
// For the method 1: use glTranslatef and glRotatef to position 
// the axes in the camera coordinate system.
// For the method 2: use gluLookAt to position the axes in the 
// camera coordinate system.
//
static void drawAxes(void) {
	// COMPLETE
	if (g_use_lookat == 0)
	{
		glTranslated(0, 0, -3*sqrt(3));
		glRotatef(45, 1, 0, 0);
		glRotatef(-45, 0, 1, 0);
	} 
	else {
		gluLookAt(3, 3, 3, 0, 0, 0, 0,1,0);
	}

	// Specify the modelview transformation to be 
	// applied to the axes
	// Draw the axes
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

	//draw the cube
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3d(0.5, 0.5, 0);
	glutSolidCube(1);
}

// COMPLETE
// in this function you need to set one of the two projections 
// available in OpenGL:
// - perspective (methods 1 and 2)
// - orthographic (method 3)
static void setProjection(const int width, const int height) {
	// COMPLETE
	if (g_projection_id == 0) {
		gluPerspective(60, (GLfloat)width / (GLfloat)height, 0.1, 20);
	}
	else glOrtho(-1, 1, -1, 1, -10, 10);
}


// The functions below do not need to be modified
//
static void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// init modelview matrix to identity
	glLoadIdentity();

	drawAxes();

	glFlush();
}


static void init(void) {
	int width, height;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection(width, height);
	glMatrixMode(GL_MODELVIEW);
}


static void keyHandle(unsigned char key, int x, int y) {
	int w, h;

	switch (key) {
	case 27:
		glutDestroyWindow(glutGetWindow());
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		g_projection_id = !g_projection_id;
		init();
		break;
	case 's':
		g_use_lookat = !g_use_lookat;
		break;
	default:
		break;
	}

	//--------------------------------------------------
	system("@cls||clear");
	static char* str[64] = { "Translate,Rotate","LookAt",
		"Perspective","Ortho" };
	printf("use lookAt: %s\nprojection id: %s\n",
		str[g_use_lookat], str[g_projection_id + 2]);

	glutPostRedisplay();
	//----------------------------------------------------
}


static void reshape(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection(width, height);
	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv) {
	int win;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);

	glutInitWindowSize(400, 400);
	win = glutCreateWindow("ex02");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyHandle);
	glutReshapeFunc(reshape);
	
	init();

	printf("a : lookat or translation,rotation\ns : perspective or ortho");
	glutMainLoop();

	return EXIT_SUCCESS;
}