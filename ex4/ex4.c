#ifdef __APPLE__
#include <GLUT/glut.h>
#else
//#include <GL/glut.h>
#endif 

#include <GL/freeglut.h>
#include <stdlib.h>


void DrawGridPlane()
{
	int i, j;

	glNormal3f(0.0, 0.0, 1.0);
	for (i = -12; i < 12; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = -12; j <= 12; j++) {
			glVertex3f((float)i / 4.0, (float)j / 4.0, 0);
			glVertex3f(((float)(i + 1)) / 4.0, (float)j / 4.0, 0);
		}
		glEnd();
	}
}


void DrawSphere() {
	GLfloat mat_diff[] = { 1, 0, 0, 1 };
	GLfloat mat_spec[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess = 128.0f;

	/* set materials for sphere */
	//task1
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/* draw sphere */
	glutSolidSphere(1.0, 32, 32);
}


void DrawCube() {

	/* set materials for cube */
	GLfloat mat_diff[] = { 0, 1, 0, 1 };
	GLfloat mat_spec[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess = 64.0f;

	/* set materials for sphere */
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/* draw cube */
	glutSolidCube(1.0);
}


void SetLight() {
	/* set light position, diffuse, and enable lights */
	GLfloat light_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat light0_diff[] = { 1.f, 0.f, 0.f, 1.f };
	GLfloat light1_diff[] = { 0.f, 0.5f, 1.f, 1.f };
	GLfloat light_spec[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat light0_pos[] = { 3, 3, 3, 1 };
	GLfloat light1_pos[] = { -3, -3, 3, 1 };

	//light0
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	//light1
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

	//directional_light
	//GLfloat directional_light_direction[] = {3, 3, 3, 0};
	//glLightfv(GL_LIGHT0, GL_POSITION, directional_light_direction);
}


void displayCallback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	SetLight();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -3.0);
	DrawGridPlane();
	glPopMatrix();

	DrawSphere();

	//DrawCube();

	glutSwapBuffers();
}


void reshapeCallback(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)width / (GLdouble)height, 0.01, 100);
}


void keyboardCallback(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;

	default:
		break;
	}
}


void initGL(void)
{
	GLfloat globalAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Lighting");

	initGL();

	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutReshapeFunc(reshapeCallback);

	glutMainLoop();

	return EXIT_SUCCESS;
}
