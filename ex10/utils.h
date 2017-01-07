#ifndef UTILS_H
#define UTILS_H


struct Color_ {
  unsigned short red;
  unsigned short green;
  unsigned short blue;
};

typedef struct Color_ Color;


struct ColorBuffer_ {
  int width;
  int height;
  
  Color* data;
};

typedef struct ColorBuffer_ ColorBuffer;

void ColorBufferCreate(int width, int height, ColorBuffer* color_buffer);
void ColorBufferDelete(ColorBuffer* color_buffer);
void ColorBufferGetWidth(ColorBuffer color_buffer, int* width);
void ColorBufferSetWidth(ColorBuffer* color_buffer, int width);
void ColorBufferGetHeight(ColorBuffer color_buffer, int* height);
void ColorBufferSetHeight(ColorBuffer* color_buffer, int height);
void ColorBufferGetPixelColor(ColorBuffer color_buffer, int i, int j, Color* color_ij);
void ColorBufferSetPixelColor(ColorBuffer* color_buffer, int i, int j, Color color_ij);
void ColorBufferSavePPM(ColorBuffer color_buffer, char* filename);


struct Vertex_ {
  int x;
  int y;
  
  // For storing depth - if needed
  float z;

  // Vertex color
  Color c;
};

typedef struct Vertex_ Vertex;


struct LineSegment_ {
  Vertex v0;
  Vertex v1;
};

typedef struct LineSegment_ LineSegment;


struct Triangle_ {
  Vertex v0;
  Vertex v1;
  Vertex v2;
};

typedef struct Triangle_ Triangle;


#endif
