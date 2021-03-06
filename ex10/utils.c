#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "utils.h"
#define TEST

float depthBuffer[64 * 64];

float minf(float a, float b)
{
  return (a<b)?a:b;
}


float maxf(float a, float b)
{
  return (a>b)?a:b;
}


void ColorBufferCreate(int width, int height, ColorBuffer* color_buffer)
{
  color_buffer->width = width;
  color_buffer->height = height;

  color_buffer->data = (Color*)malloc(width*height*sizeof(Color));
  memset(color_buffer->data, 0, width*height*sizeof(Color));//*sizeof(Color)
}


void ColorBufferDelete(ColorBuffer* color_buffer)
{
  free(color_buffer->data);
}


void ColorBufferGetWidth(ColorBuffer color_buffer, int* width)
{
  *width = color_buffer.width;
}


void ColorBufferSetWidth(ColorBuffer* color_buffer, int width)
{
  color_buffer->width = width;
}


void ColorBufferGetHeight(ColorBuffer color_buffer, int* height)
{
  *height = color_buffer.height;
}


void ColorBufferSetHeight(ColorBuffer* color_buffer, int height)
{
  color_buffer->height = height;
}


void ColorBufferGetPixelColor(ColorBuffer color_buffer, int i, int j, Color* color_ij)
{
  assert(i>=0 && i<color_buffer.width);
  assert(j>=0 && j<color_buffer.height);

  *color_ij = color_buffer.data[i * color_buffer.width + j];
}


void ColorBufferSetPixelColor(ColorBuffer* color_buffer, int i, int j, Color color_ij)
{
  assert(i>=0 && i<color_buffer->width);
  assert(j>=0 && j<color_buffer->height);

  color_buffer->data[i * color_buffer->width + j] = color_ij;
}


static int GammaCorrection(float radiance, float e) 
{
  return (int)(pow(minf(1.0f, maxf(0.0f, radiance*e)), 1.0f/2.2f) * 255.0f);
}


void ColorBufferSavePPM(ColorBuffer color_buffer, char* filename)
{
  // Note:
  // PPM format is from top to bottom, 
  // while we assume that color_buffer has (0,0) for 
  // its lower left corner, and goes from bottom to top
  // (similar to a computer screen).
  //

  FILE* file = fopen(filename, "w");
  int i, j;
  int width = color_buffer.width;
  int height = color_buffer.height;

  fprintf(file, "P3 %d %d 255\n", width, height);

  for (i = 0; i < height; ++i) {
    fprintf(file, "\n# y = %d\n", i);
    for (j = 0; j < width; ++j) {
		Color cij = {0,0,0};
      ColorBufferGetPixelColor(color_buffer, (height-1-i), j, &cij);
      fprintf(file, "%d %d %d\n", cij.red, cij.green, cij.blue);
    }
  }

  fclose(file);
}

void DrawLineTask1_1(ColorBuffer* buf, LineSegment line)
{
//incremental version
	int y = line.v0.y;
	int x0 = line.v0.x;
	int y0 = line.v0.y;
	int x1 = line.v1.x;
	int y1 = line.v1.y;
	float d = (y0 - y1)*(x0+1) + (x1 - x0)*(x0+0.5) + x0*y1 - x1*y0;
	Color c = line.v0.c;
	
	for (int x = x0; x <= x1; x++)
	{
		ColorBufferSetPixelColor(buf, y, x, c);

		if (d < 0)
		{
			y++;
			//f(x+1, y+1) = (y0-y1)*(x+1) + (x1-x0)*(y+1) + x0*y1 - x1*y0;
			//            = f(x,y) + (y0-y1) + (x1-x0);
			d += (x1 - x0) + (y0 - y1);
		}
		else {
			d += (y0 - y1);
		}
	}
}

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void DrawLineTask1_2(ColorBuffer* buf, LineSegment line) {
	int x0 = line.v0.x;
	int y0 = line.v0.y;
	int x1 = line.v1.x;
	int y1 = line.v1.y;
	Color c = line.v0.c;

	int steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep)
	{
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1)
	{
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = deltax / 2;
	int ystep;
	int y = y0;

	if (y0 < y1)ystep = 1;
	else ystep = -1;

	for (int x = x0; x <= x1; ++x)
	{
		if (steep)ColorBufferSetPixelColor(buf,x, y, c);
		else ColorBufferSetPixelColor(buf, y, x, c);
		error = error - deltay;
		if (error < 0)
		{
			y += ystep;
			error += deltax;
		}
	}
}

int orient2D(int x0, int y0, int x1, int y1, int x2, int y2)
{
	return (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);
}

void DrawTriangleWithDepth(ColorBuffer* buf, Triangle t)
{

	int x0 = t.v0.x, x1 = t.v1.x, x2 = t.v2.x;
	int y0 = t.v0.y, y1 = t.v1.y, y2 = t.v2.y;
	Color c0 = t.v0.c, c1 = t.v1.c, c2 = t.v2.c, c;

	int xmin = minf(x0, minf(x1, x2)), ymin = minf(y0, minf(y1, y2));
	int xmax = maxf(x0, maxf(x1, x2)), ymax = maxf(y0, maxf(y1, y2));
	int x, y;
	int w0, w1, w2;
	float z0 = t.v0.z, z1 = t.v1.z, z2 = t.v2.z, z;

	for (x = xmin; x <= xmax; ++x)
	{
		for (y = ymin; y <= ymax; ++y)
		{

			w0 = orient2D(x0, y0, x1, y1, x, y);
			w1 = orient2D(x1, y1, x2, y2, x, y);
			w2 = orient2D(x2, y2, x0, y0, x, y);
			
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				z = (w0*z0 + w1*z1 + w2*z2) / (float)(w0 + w1 + w2);
				
				int idx = y*buf->width + x;

				if (z < depthBuffer[idx])
				{
					c.red = (c0.red*w0 + c1.red*w1 + c2.red*w2) / (w0 + w1 + w2);
					c.blue = (c0.blue*w0 + c1.blue*w1 + c2.blue*w2) / (w0 + w1 + w2);
					c.green = (c0.green*w0 + c1.green*w1 + c2.green*w2) / (w0 + w1 + w2);
					depthBuffer[idx] = z;
					ColorBufferSetPixelColor(buf, y, x, c);
				}
			}
		}
	}
}


#ifdef TEST
int main(void) {
  Color white = {255, 255, 255};
  Color red = {255, 0, 0};
  Color blue = {0, 255, 0};
  Color green = {0,0,255};

  int width = 64;
  int height = 64;
  ColorBuffer cb;
  ColorBufferCreate(width, height, &cb);

  //initialized with the maximum depth.
  for (int i = 0; i < width*height; ++i)depthBuffer[i] = 999;

  ColorBufferSetPixelColor(&cb, 0, 0, white);
  ColorBufferSetPixelColor(&cb, 1, 0, white);
  ColorBufferSetPixelColor(&cb, 2, 0, white);
  ColorBufferSetPixelColor(&cb, 0, 1, white);
  ColorBufferSetPixelColor(&cb, 1, 1, white);
  ColorBufferSetPixelColor(&cb, 2, 1, white);
  ColorBufferSetPixelColor(&cb, 0, 2, white);
  ColorBufferSetPixelColor(&cb, 1, 2, white);
  ColorBufferSetPixelColor(&cb, 2, 2, white);

  LineSegment line0 = { {0,0,0,red},{32, 32, 0, red} },
	  line1 = { {32,0,0,red},{ 0,32,0,red } };

  DrawLineTask1_1(&cb, line0);
  DrawLineTask1_2(&cb, line1);
  
  Triangle tri0 = { { 0,0,1.f,red },{ 63,0,1.f,blue },{ 32,32,1.f,green } },
			   tri1 = { { 10,0,0.5f,white },{ 53,0,1.5f,white },{ 32,32-8,0.5f,white } };
  DrawTriangleWithDepth(&cb, tri0);
  DrawTriangleWithDepth(&cb, tri1);
 

  ColorBufferSavePPM(cb, "test.ppm");

  ColorBufferDelete(&cb);
  return 0;
}
#endif
