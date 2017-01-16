#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#pragma comment(lib, "glew32.lib")

#ifndef __APPLE__
#include <gl\glew.h>

#ifdef WIN32
#include <gl\wglew.h>
#else
#include "glxew.h"
#endif
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Location of attributes / (in variables) in the shader
enum {POSITION_LOCATION};


//static GLuint g_vao;
static GLuint g_vbo[1];
static GLuint g_programHandle;
static GLsizei g_stride;


static void BuildGeometry() {
  GLfloat verts[] = {
    // Vertex 1 position
    -0.5f, -0.5f, 
    // Vertex 2 position
    +0.0f, +0.5f, 
    // Vertex 3 position
    +0.5f, -0.5f 
  };

  
  //GLuint vbo;
  GLsizeiptr vbo_size = sizeof(verts);
  g_stride = 2 * sizeof(GLfloat);
  GLenum usage = GL_STATIC_DRAW;

  glGenBuffers(1, g_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, g_vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, vbo_size, verts, usage);
}


// Read a shader source.
// Return a char* that needs to be freed by the caller after
// the shader is compiled.
static char* GetShaderSource(const char* filename) {
  FILE* file_handle = fopen(filename, "r");
  if (!file_handle) {
    return NULL;
  }

  fseek(file_handle, 0L, SEEK_END);
  long size = ftell(file_handle);

  fseek(file_handle, 0L, SEEK_SET);
  char* source = (char *)malloc((size+1) * sizeof(char));
  fread(source, 1, size, file_handle);

  source[size] = '\0';

  fclose(file_handle);
  return source;
}


static void LoadShaders() {
  char* vsSource, *fsSource;
  GLuint vsHandle, fsHandle;
  GLint success;
  GLchar message[256];
  //GLuint programHandle;


  // Compile vertex shader
  vsSource = GetShaderSource("simple1.vs.glsl");
  vsHandle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vsHandle, 1, (const char**)&vsSource, 0);
  glCompileShader(vsHandle);
  glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &success);
  glGetShaderInfoLog(vsHandle, sizeof(message), 0, message);
  free(vsSource);
  if (success == GL_FALSE) {
    fprintf(stderr, "Error in compiling: simple1.vs.glsl\n");
    fprintf(stderr, "Error message: %s\n", message);
    exit(1);
  }
  
  
  // Compile the fragment shader
  fsSource = GetShaderSource("simple1.fs.glsl");
  fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fsHandle, 1, (const char**)&fsSource, 0);
  glCompileShader(fsHandle);
  glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &success);
  glGetShaderInfoLog(fsHandle, sizeof(message), 0, message);
  free(fsSource);
  if (success == GL_FALSE) {
    fprintf(stderr, "Error in compiling: simple1.fs.glsl\n");
    fprintf(stderr, "Error message: %s\n", message);
    exit(1);
  }


  // Create a program, attach the shader objects to it and link 
  g_programHandle = glCreateProgram();
  if (g_programHandle == 0) {
    fprintf(stderr, "Error when creating a shader program\n");
    exit(1);    
  }
  glAttachShader(g_programHandle, vsHandle);
  glAttachShader(g_programHandle, fsHandle);
  glBindAttribLocation(g_programHandle, POSITION_LOCATION, "vPosition");
  glLinkProgram(g_programHandle);
  glGetProgramiv(g_programHandle, GL_LINK_STATUS, &success);
  glGetProgramInfoLog(g_programHandle, sizeof(message), 0, message);
  if (success == GL_FALSE) {
    fprintf(stderr, "Error during linking\n");
    fprintf(stderr, "Error message: %s\n", message);
    exit(1);
  }


  // Use this program for rendering
  glUseProgram(g_programHandle);
}


const char* GetWindowName() {
  return "Triangle";
}


static void InitGL(void) {
  BuildGeometry();
  LoadShaders();
}


static void HandleDisplay(void) {
  GLenum error;

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
 
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(g_programHandle);

  glBindBuffer(GL_ARRAY_BUFFER, g_vbo[0]);

  glEnableVertexAttribArray(POSITION_LOCATION);
  glVertexAttribPointer(POSITION_LOCATION, 2, GL_FLOAT, GL_FALSE, g_stride, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  error = glGetError();
  if (error != GL_NO_ERROR) {
    printf("gl error: %s\n", gluErrorString(error));
  }

  glFlush();
}


static void HandleKeyboardEvents(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
    case 'q':
    case 'Q':
      exit(EXIT_SUCCESS);
  }
}


int main(int argc, char** argv) {
  GLenum err;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Triangle");

#ifndef __APPLE__
  err = glewInit();
  if (err != GLEW_OK) {
    printf("Error in initializing glew\n");
    printf("%s\n", glewGetErrorString(err));
  }
#endif

  
  InitGL();

  glutDisplayFunc(HandleDisplay);
  glutKeyboardFunc(HandleKeyboardEvents);

  glutMainLoop();
  return EXIT_SUCCESS;
}

