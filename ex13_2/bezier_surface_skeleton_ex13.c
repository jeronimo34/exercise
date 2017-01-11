#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/* Globals */

/* Control points */
static GLuint number_control_points_u = 4;
static GLuint number_control_points_v = 4;
static GLdouble control_points[4][4][3] = {
  {{-3.0, -3.0, -1.0},{-1.0, -3.0, 0.0},{1.0, -3.0, 0.0},{3.0, -3.0, -2.0}},
  {{-3.0, -1.0, 0.0},{-1.0, -1.0, 2.0},{1.0, -1.0, 2.0},{3.0, -1.0, 0.0}},
  {{-3.0, 1.0, 0.0},{-1.0, 1.0, 2.0},{1.0, 1.0, 2.0},{3.0, 1.0, 0.0}},
  {{-3.0, 3.0, -1.0},{-1.0, 3.0, 0.0},{1.0, 3.0, 0.0},{3.0, 3.0, -1.0}}
};

/* Number of sample points */
static GLint number_samples_u = 52;
static GLint number_samples_v = 52;
#define MIN_NUMBER_SAMPLES 2
#define MAX_NUMBER_SAMPLES 152


static void PrintHelp(void) {
  printf("Usage: \n");
  printf("\tp: toggle on/off the display of the control points\n");
  printf("\tm: toggle on/off the display of the control polygon\n");
  printf("\tu/U: increase/decrease the number of sample points by 10 along u direction\n");
  printf("\tv/V: increase/decrease the number of sample points by 10 along v direction\n");
  printf("\th: print this help\n");
}


/* Initialize the light properties and some OpenGL setup. 
   Normally there is no need to modify this function.
*/
static void Init (void) {
  GLfloat light_ambient[]={0.3, 0.3, 0.3, 1.0};
  GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[]={80.0, 120.0, -6.0, 0.0};

  /* Print help for first time use */
  PrintHelp();

  glClearColor(0.0, 0.0, 0.0, 1.0);

  glEnable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);


  /* Automatically compute normal when GL_MAP2_VERTEX_3
     or GL_MAP2_VERTEX_4 is used to generate vertices. */
  glEnable(GL_AUTO_NORMAL);
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, number_control_points_u,
	  0, 1, 12, number_control_points_v, &control_points[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
}


/* Plot control points of the Bezier surface in blue
   using a size of 5.0 */
static void PlotControlPoints(void) {
/* Complete */
}


/* Plot the control polygon in white with a line width of 4.0 */
static void PlotControlPolygon(void) {
/* Complete */
}


/* Plot the cubic Bezier surface using an OpenGL evaluator
   (glMap, glEvalMesh, glMapGrid).
   Use a sampling of 52 in u and v direction for the Bezier surface.
   The number of samples can be modified interactively with the keyboard.
   The order of the Bezier surface as well as the control points are specified above as global variables.
   The material properties of the surface are given below.
*/
static void PlotBezierSurface(void) {
  /* Specify the material properties of the surface */
  GLfloat mat_specular[]={1.0, 1.0, 0.5, 1.0};
  GLfloat mat_diffuse[]= {1.0, 0.1, 0.1, 1.0};
  GLfloat mat_ambient[]= {1.0, 0.1, 0.1, 1.0};
  GLfloat mat_shininess= {100.0};

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

  /* Complete */
  glMapGrid2f(number_samples_u, 0.0, 1.0, number_samples_v, 0.0, 1.0);
  glEvalMesh2(GL_FILL, 0, number_samples_u, 0, number_samples_v);
}


/* Callback display function.
   Modify if necessary.
*/
static void Display (void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  PlotControlPoints();
  PlotControlPolygon();
  PlotBezierSurface();

  glutSwapBuffers();
}


/* Callback function used when the window is resized.
   No need to modify normally.
*/
static void Reshape (int w, int h) {
  glViewport (0, 0, w, h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, (GLfloat)w/(GLfloat)h, 1.0, 100.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(12.0, 12.0, 8.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0);
}


/* Callback function to handle key events. 
   The key-action mapping is:
    - p: toggle on/off the display of the control points
    - m: toggle on/off the display of the control polygon
    - u/U: increase/decrease the number of samples in the u direction
    - v/V: increase/decrease the number of samples in the v direction
    - h: print the help
*/
static void HandleKey(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
      break;

    /* Complete */

  }
  glutPostRedisplay();
}


/* Main function. No need to modify. */
int main(int argc, char **argv) {
  glutInit(&argc, argv); 
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
  glutInitWindowSize (480, 480);                
  glutCreateWindow("Cubic Bezier surface"); 

  glutReshapeFunc(Reshape); 
  glutDisplayFunc(Display);
  glutKeyboardFunc(HandleKey);
  
  Init(); 

  glutMainLoop(); 

  return EXIT_SUCCESS;
}

