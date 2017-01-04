#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
 
#include <stdlib.h>
#include <time.h>

int BAR,SWING;
int g_anglur_speed_mode = 0;//0 liner, 1 sin
double g_angle = 0;
double g_angle_dir = 1;
double g_previous_time;
double fps = 20;//frame/second
double g_time_step_duration;
double T = 2;

void makeBar() 
{
  glNewList(BAR, GL_COMPILE);
 
  // Draw the pole
  glBegin(GL_QUADS);
 
    glColor3f(1.0, 1.0, 0.0);
     
    // Bottom
    glVertex3d(-50, 30, -1);
    glVertex3d( 50, 30, -1);
    glVertex3d( 50, 30,  1);
    glVertex3d(-50, 30,  1);
    // Top
    glVertex3d(-50, 33, -1);
    glVertex3d( 50, 33, -1);
    glVertex3d( 50, 33,  1);
    glVertex3d(-50, 33,  1);
 
    glColor3f(1.0, 0.65, 0.0);
 
    // Front
    glVertex3d(-50, 33, 1);
    glVertex3d(-50, 30, 1);
    glVertex3d( 50, 30, 1);
    glVertex3d( 50, 33, 1);
    // Back
    glVertex3d( 50, 33, -1);
    glVertex3d( 50, 30, -1);
    glVertex3d(-50, 30, -1);
    glVertex3d(-50, 33, -1);
 
    glColor3f(1.0, 0.0, 0.0);
 
    // Left
    glVertex3d(-50, 33, -1);
    glVertex3d(-50, 30, -1);
    glVertex3d(-50, 30,  1);
    glVertex3d(-50, 33,  1);
    // Right
    glVertex3d(50, 33, 1);
    glVertex3d(50, 30, 1);
    glVertex3d(50, 30, -1);
    glVertex3d(50, 33, -1);
  glEnd();
  glEndList();
 
}
 
void makeSwing() 
{
glNewList(SWING, GL_COMPILE);
 
  glColor3f(1.0, 0.0, 1.0);
  glLineWidth(5.0);
     
  // Draw the ropes
  glBegin(GL_LINES);
    glVertex3d(-20,  30, 0);
    glVertex3d(-20, -30, 0);
    glVertex3d( 20,  30, 0);
    glVertex3d( 20, -30, 0);
  glEnd();
 
  glColor3f(0.0, 1.0, 0.0);
  
  // Draw the sides of the seat
  glBegin(GL_QUAD_STRIP);
    glVertex3d( 20, -30, -10);
    glVertex3d( 20, -33, -10);
    glVertex3d(-20, -30, -10);
    glVertex3d(-20, -33, -10);
    glVertex3d(-20, -30,  10);
    glVertex3d(-20, -33,  10);
    glVertex3d( 20, -30,  10);
    glVertex3d( 20, -33,  10);
    glVertex3d( 20, -30, -10);
    glVertex3d( 20, -33, -10);
  glEnd();
     
  glColor3f(0.0, 0.0, 1.0);
     
  // Draw the top and bottom of the seat
  glBegin(GL_QUADS);
    glVertex3d(-20, -33, -10);
    glVertex3d( 20, -33, -10);
    glVertex3d( 20, -33,  10);
    glVertex3d(-20, -33,  10);
     
    glVertex3d(-20, -30, -10);
    glVertex3d( 20, -30, -10);
    glVertex3d( 20, -30,  10);
    glVertex3d(-20, -30,  10);
  glEnd();
  glEndList();
   
}
 
void reshape(int width, int height) 
{
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLdouble)width/(GLdouble)height, 5.0, 200.0);
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(150.0, 20.0, 40.0, 0.0, -10.0, 0.0, 0.0, 1.0, 0.0);

  g_previous_time = clock();
}
 
static void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 27:  exit(EXIT_SUCCESS);
    break;
  default:
    break;
  }
}
 
void display(void) 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glCallList(BAR);
	
	glTranslated(0, 30, 0);
	glRotated(g_angle,1,0,0);
	glTranslated(0, -30,0);
	glCallList(SWING);
 
  //glFlush();
  glutSwapBuffers();
 
}

void idle()
{
	g_time_step_duration = 1000 / fps;
	while ((clock() - g_previous_time) / CLOCKS_PER_SEC < g_time_step_duration / 1000);

	double p = k / (T*fps);
	if (g_anglur_speed_mode == 0) {
		g_angle += g_angle_dir;
		if (g_angle > 40) {
			g_angle = 40;
			g_angle_dir = -1;
		}
		else if(g_angle < -40)
		{
			g_angle = -40;
			g_angle_dir = 1;
		}
	}
	else
	{
	
	}
	g_previous_time = clock();

	glLoadIdentity();
	gluLookAt(150.0, 20.0, 40.0, 0.0, -10.0, 0.0, 0.0, 1.0, 0.0);
	glutPostRedisplay();
}
 
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Swing");
 
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
 
  BAR = glGenLists(2);
  SWING = BAR + 1;
  makeBar();
  makeSwing();
   
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  glutMainLoop();
   
  return EXIT_SUCCESS;
}