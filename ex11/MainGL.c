#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>


#include "Parameters.h"
#include "RayTrace.h"

#include <stdio.h>
#include <math.h>

GLint g_width = 512;
GLint g_height = 512;
GLuint g_tex_id;


void initGL() 
{
  // Load the ray traced image as a texture
  GLubyte* texture;
		
  // Set the parameters of the scene (background color, camera position, field of view)
  Parameters parameters;
	
	
  glClearColor(1,1,1,1);

  glViewport(0, 0, g_width, g_height);
	
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	
	
  glMatrixMode(GL_MODELVIEW);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	
  // Set the parameters
  setParameters(&parameters);
		
  // Ray-trace the dist function and save it in the texture
  rayTrace(parameters, g_width, g_height, &texture);
	
	
  glGenTextures(1, &g_tex_id);
  glBindTexture(GL_TEXTURE_2D, g_tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
	
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);


  free(texture);
}


void reshape(int width, int height)
{}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
	
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, g_tex_id);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
	
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, -1.0f);
	
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, -1.0f);
	
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);
	
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, 1.0f);
	
  glEnd();
	
  glFlush();
}


void handleKeyEvents(unsigned char key, int x, int y)
{
  if (key == 27) exit(0);
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutCreateWindow("ray casting");
	
  initGL();
	
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(handleKeyEvents);
	
  glutMainLoop();
	
  return 0;
}

