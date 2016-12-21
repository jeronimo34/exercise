#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static GLfloat angle = 0.0;
static GLuint texture;
static int g_transparency_mode = 0;
static int g_fog_mode = -1;//no fog

static void initLight(void)
{
	GLfloat lightPosition[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightAmbient[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}


static void initTexture(void)
{
	GLubyte data[64][64][3];
	int i, j, c;

	for (i = 0; i < 64; i++) {
		for (j = 0; j < 64; j++) {
			c = ((i / 8) & 1) ^ ((j / 8) & 1);
			data[i][j][0] = c * 255;
			data[i][j][1] = c * 255;
			data[i][j][2] = c * 255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)data);
}


static void init(void)
{
	initLight();
	initTexture();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);

	//set fog params
	GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
	glEnable(GL_FOG);
	g_fog_mode = GL_EXP;
	glFogi(GL_FOG_MODE, g_fog_mode);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.35);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 5.0);

}


static void drawPlane(void)
{
	GLfloat planeColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat planeSpecular[] = { 0.0, 0.0, 0.0, 0.0 };

	GLboolean texture2d;

	texture2d = glIsEnabled(GL_TEXTURE_2D);

	if (texture2d != GL_TRUE) glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, planeColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planeSpecular);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, 0.0, -3.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 0.0, 3.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 0.0, 3.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(3.0, 0.0, -3.0);
	glEnd();

	if (texture2d != GL_TRUE) glDisable(GL_TEXTURE_2D);
}

static void drawCone(void)
{
	GLfloat coneColor[] = { 1.0, 1.0, 0.0, 0.5 };

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coneColor);
	glutSolidCone(0.5, 1.0, 16, 16);
	glPopMatrix();

}

static void drawBox(void)
{
	GLfloat cubeColor[] = { 0.0, 0.0, 1.0, 0.5 };

	glPushMatrix();
	glTranslatef(0.0, 0.376, 1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cubeColor);
	glutSolidCube(0.75);
	glPopMatrix();

}


static void displayFunc(void) {
	GLfloat specular[] = { 0.5, 0.5, 0.5, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(angle, 0.0, 1.0, 0.0);

	drawPlane();

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);

	//draw opaque objects
	if (g_transparency_mode == 0) {
		drawBox();
		drawCone();
	} 
	else if (g_transparency_mode == 1) {
		drawCone();
	}
	else drawBox();

	//draw translucent object
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (g_transparency_mode == 1) drawBox();
	else drawCone();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glPopMatrix();

	glutSwapBuffers();
}


static void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:     exit(EXIT_SUCCESS);
		break;
	case 't':
		g_transparency_mode = (g_transparency_mode + 1) % 3;
		break;
	case 'f':
		if (g_fog_mode == GL_EXP) g_fog_mode = GL_EXP2;
		else if (g_fog_mode == GL_EXP2) g_fog_mode = GL_LINEAR;
		else if (g_fog_mode == GL_LINEAR) g_fog_mode = -1;
		else if (g_fog_mode == -1)g_fog_mode = GL_EXP;

		if (g_fog_mode == -1) glDisable(GL_FOG);
		else {
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE, g_fog_mode);
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}


static void reshapeFunc(int width, int height)
{
	glViewport(0, 0,
		(GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)width / (GLdouble)height, 0.01, 100.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

static void idleFunc(void)
{
	angle += 0.05;
	if (angle >= 360.0) angle -= 360.0;
	glutPostRedisplay();
}

static void visibilityFunc(int state)
{
	if (state == GLUT_VISIBLE) glutIdleFunc(idleFunc);
	else                       glutIdleFunc(NULL);
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutCreateWindow("Blending-Fog");

	init();

	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyboardFunc);
	glutReshapeFunc(reshapeFunc);
	glutVisibilityFunc(visibilityFunc);

	glutMainLoop();

	return EXIT_SUCCESS;
}