#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
double g_angles[5] = { 0 };

void display(void) {
	GLfloat lightAmbient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightDiffuse[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat lightSpecular[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat lightPosition[] = { 1.0, 4.0, 10.0, 1.0 };

	GLfloat mat_ambient[] = { 0.6, 0.3, 0.3, 1.0 };
	GLfloat mat_diffuse[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat mat_specular[] = { 0.3, 0.3, 0.1, 1.0 };


	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, 3.0);


	/* Complete */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//add
	
	glLoadIdentity();
	gluLookAt(3, 4, 6, 0, 0, 0, 0, 1, 0);

	//rotation for arm1, arm2, fin1,2
	glPushMatrix();
	glRotatef(g_angles[1], 0, 1, 0);
	glRotatef(g_angles[0], 0, 0, 1);
	glTranslated(1, 0, 0);

	//geometry and local transform for arm1
	glPushMatrix();
	glScalef(2, 0.4, 1.0);
	glutSolidCube(1);
	glPopMatrix();

	//rotation for arm2, fin1,2
	glTranslated(1, 0, 0);
	glRotated(g_angles[2], 0, 0, 1);
	glTranslated(1, 0, 0);

	//geometory for arm2
	glPushMatrix();
	glScalef(2, 0.4, 1);
	glutSolidCube(1);
	glPopMatrix();

	//rotation for fin1
	//geometory for fin1
	glPushMatrix();
	glTranslated(1, 0, 0);
	glRotated(20+g_angles[3], 0, 0, 1);
	glTranslated(0.25, 0, 0);
	glScaled(0.5, 0.2, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	//rotation for fin2
	//geometory for fin2
	glPushMatrix();
	glTranslated(1, 0, 0);
	glRotated(g_angles[4]-20, 0, 0, 1);
	glTranslated(0.25, 0, 0);
	glScaled(0.5, 0.2, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}


void reshape(int w, int h) {
	/* Complete */
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)w / (double)h, 0.1, 1000);
	
	glMatrixMode(GL_MODELVIEW);	
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		/* Complete */
	case 'q':
	case 'Q':
		exit(1);
		break;
	case 'S':
		g_angles[0]++;
		break;
	case 's':
		g_angles[0]--;
		break;
	case 'A':
		g_angles[1]++;
		break;
	case 'a':
		g_angles[1]--;
		break;
	case 'E':
		g_angles[2]++;
		break;
	case 'e':
		g_angles[2]--;
		break;
	case 'F':
		g_angles[3]++;
		break;
	case 'f':
		g_angles[3]--;
		break;
	case 'G':
		g_angles[4]++;
		break;
	case 'g':
		g_angles[4]--;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	GLfloat globalAmbient[] = { 0.0, 0.0, 0.0, 1.0 };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Robot Arm");

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.3, 0.3, 0.3, 1.0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}