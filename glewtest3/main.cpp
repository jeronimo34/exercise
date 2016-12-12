//#include <GL/glew.h>
//#include <GL/wglew.h>

#include <GL/freeglut.h>
#include <stdlib.h>


static void display(void) {
	// repaint the background with the color 
	// specified by glClearColor()
	glClear(GL_COLOR_BUFFER_BIT);

	// COMPLETE 
	// type your code here for drawing various primitives 
	// on the screen. 
	// Make sure that these objects are all defined within 
	// the unit square [0,1]x[0,1]
	glBegin(GL_POINTS);
	glVertex3f(0.1, 0.1, 0);
	glVertex3f(0.2, 0.1, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.1, 0.2, 0);
	glVertex3f(0.5, 0.2, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3d(0.2, 0.2, 0);
	glVertex3d(0.5, 0.2, 0);
	glVertex3d(0.4, 0.5, 0);
	glVertex3d(0.1, 0.3, 0);
	glEnd();

	glFlush();
}



// No need to modify the functions below
//

static void init(void) {
	// set the color for painting the background
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// set an orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}


static void keyHandle(unsigned char key, int x, int y) {
	// exit when ESC is pressed
	if (key == '27') exit(0);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ex01");

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyHandle);
	glutMainLoop();
	return 0;
}
