/*
 * Plot a cubic Bezier curve defined by 4 control points using either:
 * - evaluation of the Bernstein basis
 * - evaluation by the De Casteljau algorithm
 * - evaluation by OpenGL (glMap, glEvalCoord)
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/* Globals */

/* Control points */
static GLuint number_control_points = 4;
static GLdouble control_points[4][3] = {{100.0, 100.0, 0.0},
                                        {200.0, 350.0, 0.0},
                                        {400.0, 300.0, 0.0},
                                        {480.0, 100.0, 0.0}};
int show_method[3] = { 0 };

/* Number of sampling points */
static GLuint number_samples = 52;
#define MIN_NUMBER_SAMPLES 2
#define MAX_NUMBER_SAMPLES 152

/* Window dimensions */
static GLint window_width = 600;
static GLint window_height = 400;

/* Remember the selected control point */
static GLint selected_index = -1;


/* Functions */

static void PrintHelp() {
  printf("Usage: \n");
  printf("\tb/B: plot/clear the Bezier curve by evaluating the Bernstein polynomial\n");
  printf("\tc/C: plot/clear the Bezier curve using De Casteljau algorithm\n");
  printf("\tg/G: plot/clear the Bezier curve using OpenGL glMap/glEvalCoord\n");
  printf("\tn/N: increase/decrease the number of sample points by 10\n");
  printf("\th: print this help\n");
}


static void PrintControlPoints() {
  int i, j;

  printf("\nControl Points:\n");

  for (i = 0; i < number_control_points; i++) {
    printf("  ( ");
    for (j = 0; j < 3; j++) {
      printf("%g ", control_points[i][j]);
    }
    printf(")\n");
  }
}


static void Init(void) {
  /* Display help for first time usage */
  PrintHelp();

  /* Black background */
  glClearColor(0.0, 0.0, 0.0, 1.0);

  /* Set the projection and model-view transforms */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, window_width, 0.0, window_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* Add code if needed */
  glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, number_control_points, control_points);
  glEnable(GL_MAP1_VERTEX_3);
}


/* Plot control points in red using a point size of 5.0 */
static void PlotControlPoints() {
    /* COMPLETE */
	glPointSize(5.0);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < number_control_points; ++i)
	{
		glVertex3dv(control_points[i]);
	}
	glEnd();
}


/* Plot the control polygon in green using a line width of 4.0 */
static void PlotControlPolygon() {
    /* COMPLETE */
	glLineWidth(4.0);
	glColor3f(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < number_control_points; ++i)
	{
		glVertex3dv(control_points[i]);
	}
	glEnd();
}

/*--- ˆÙ‚È‚énŒÂ‚©‚çrŒÂ‚Ì®”‚ðŽæ‚èo‚·‘g‡‚¹‚Ì”‚ð•Ô‚· ---*/
int combination(int n, int r)
{
	if (r == 0 || r == n)
		return (1);
	else if (r == 1)
		return (n);
	return (combination(n - 1, r - 1) + combination(n - 1, r));
}

void BernsteinBasisEvaluation(float t, GLdouble p[3])
{
	p[0] = 0; p[1] = 0; p[2] = 0;
	int n = number_control_points - 1;
	for (int i = 0; i < number_control_points; ++i)
	{
		GLdouble B = combination(n,i)*pow((1 - t), n - i)*pow(t,i);
		p[0] += B*control_points[i][0];
		p[1] += B*control_points[i][1];
		p[2] += B*control_points[i][2];
	}
}


void DeCasteljau(GLdouble res[3], float t) {
	
	GLdouble points[4][4][3] = { 0 };
	for (int i = 0; i < number_control_points; ++i)
	{
		points[i][0][0] = control_points[i][0];
		points[i][0][1] = control_points[i][1];
		points[i][0][2] = control_points[i][2];
	}

	for (int j = 1; j <= number_control_points-1; ++j)
	{
		for (int i = 0; i <= number_control_points-j-1; ++i) {
			points[i][j][0] = (1-t)*points[i][j-1][0] + t*points[i+1][j-1][0];
			points[i][j][1] = (1-t)*points[i][j-1][1] + t*points[i+1][j-1][1];
			points[i][j][2] = (1-t)*points[i][j-1][2] + t*points[i+1][j-1][2];
		}
	}
	res[0] = points[0][number_control_points - 1][0];
	res[1] = points[0][number_control_points - 1][1];
	res[2] = points[0][number_control_points - 1][2];

}

/* Plot the cubic Bezier curve using either:
    - direct evaluation of the Bernstein basis functions to weight the control points
    - the De Casteljau algorithm
    - the functions provided by the OpenGL library (glMap, glEvalCoord)
   
   The drawing method to be used should be controlled by keyboard.

   Plot the curve using a line width of 4.0. Use different colors for each method:
    - Bernstein basis evaluation: Blue color (0,0,1)
    - De Casteljau: Pink color (1,1,0)
    - OpenGL: Yellow color (0,1,1)
*/
static void PlotBezierCurve() {
	GLdouble point[3];

    /* COMPLETE */
	if (show_method[0]) {
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= number_samples; ++i)
		{
			float t = (float)i / number_samples;
			BernsteinBasisEvaluation(t, point);
			glVertex3dv(point);
		}
		glEnd();
	}

	if (show_method[1]) {
		glColor3f(1,1,0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= number_samples; ++i)
		{
			float t = (float)i / number_samples;
			//DeCasteljau(t, control_points, number_control_points, point);
			DeCasteljau(point, t);
			glVertex3dv(point);
		}
		glEnd();
	}

	if (show_method[2]) {
		glMap1d(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, number_control_points, control_points);
		glColor3f(0, 1, 1);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= number_samples; i++)
			glEvalCoord1f((GLfloat)i / number_samples);
		glEnd();
	}
}


/* The display callback function.
   Normally there is no need to modify this function.
 */
static void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  /* Reinit MODELVIEW matrix */
  glLoadIdentity();

  PlotControlPoints();
  PlotControlPolygon();
  PlotBezierCurve();
  
  glutSwapBuffers();
}


/* The callback function called when the window is reshaped.
   Normally there is no need to modify this function
 */
static void Reshape(int width, int height) {
  GLdouble ratio = (GLdouble)height / (GLdouble)width;

  glViewport(0, 0, width, height);

  /* Reset the global window dimensions */
  window_width = width;
  window_height = height;
  
  /* Set the projection matrix */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, window_width, 0.0, window_height);

  /* Reset the MODELVIEW matrix */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/* The callback function used for handling key events */
static void HandleKey(unsigned char key, int x, int y) {
  switch(key) {
    /* Esc */
    case 27:
      exit(0);
      break;

    case 'h':
      PrintHelp();
      break;

    /* COMPLETE: 
       b/B: plot/clear the Bezier curve by evaluating the Bernstein polynomial;
       c/C: plot/clear the Bezier curve using De Casteljau algorithm;
       g/G: plot/clear the Bezier curve using OpenGL glMap/glEvalCoord;
       n/N: increase/decrease the number of sample points by 10;
    */
	case 'b':
		show_method[0] = 1;
		break;
	case 'B':
		show_method[0] = 0;
		break;
	case 'c':
		show_method[1] = 1;
		break;
	case 'C':
		show_method[1] = 0;
		break;
	case 'g':
		show_method[2] = 1;
		break;
	case 'G':
		show_method[2] = 0;
		break;
	case 'n':
		
		if (number_samples > 10)number_samples -= 10;
		number_samples = min(MIN_NUMBER_SAMPLES, number_samples);
		break;
	case 'N':
		number_samples = min(152, number_samples + 10);
		break;
  }
  glutPostRedisplay();
}


/* Mouse action event handler: 
   if the left button of the mouse is clicked, find the nearest control point
   to the clicked position, and if it is within a given tolerance, set it 
   as the index of the control point to move.
*/
static void HandleMouseAction(int button, int state, int x, int y) {
  /* Picking tolerance */
  GLint pick_radius = 50;

  int i; 

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    GLuint xx = x;
    GLuint yy = window_height - y;

    for (i = 0; i < number_control_points; i++) {
      if ((abs(control_points[i][0] - xx) <= pick_radius) &&
          (abs(control_points[i][1] - yy) <= pick_radius)) 
	{
	  selected_index = i;
	  break;
	}
    }
  }

  /* Button released, reset the selected control point */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    selected_index = -1;
  }

  glutPostRedisplay();
}


/* Mouse motion handler: 
   when the mouse is moved, check if a control point has been already selected, 
   if yes, then update its coordinates. 
 */
static void HandleMouseMotion(int x, int y) {
  if (selected_index > -1) {
    GLuint xx;
    GLuint yy;

    xx = x;
    yy = window_height - y;

    if (xx < window_width) {
      control_points[selected_index][0] = xx;
    }

    if (yy < window_height) {
      control_points[selected_index][1] = yy;
    }

    glutPostRedisplay();
  }
}


int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("Cubic Bezier curve");
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(HandleKey);
  glutMouseFunc(HandleMouseAction);
  glutMotionFunc(HandleMouseMotion);
  Init();
  glutMainLoop();
  return EXIT_SUCCESS;
}

