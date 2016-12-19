#ifdef __APPLE__
#include <GLUT/glut.h>
#else
//#include <GL/glut.h>
#endif

#include <GL/freeglut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMAGE_HEIGHT 64 
#define IMAGE_WIDTH 64

#define FISH_IMAGE_WIDTH 256
#define FISH_IMAGE_HEIGHT 256

static GLfloat theta[] = { 0.0,0.0,0.0 };

static GLfloat vertices[][3] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ 1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

static GLubyte checkImage[IMAGE_HEIGHT][IMAGE_WIDTH][4];
static GLubyte fishImageData[FISH_IMAGE_HEIGHT*FISH_IMAGE_WIDTH * 3];
static GLubyte bandaiData[FISH_IMAGE_HEIGHT*FISH_IMAGE_WIDTH * 3];

static GLuint texNames[3];//0 = check, 1 = fish.

//add
void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < IMAGE_HEIGHT; ++i){
		for (j = 0; j < IMAGE_WIDTH; ++j){
			c = (((i&0x8)==0)^((j&0x8)==0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;//r
			checkImage[i][j][1] = (GLubyte)c;//g
			checkImage[i][j][2] = (GLubyte)c;//b
			checkImage[i][j][3] = (GLubyte)c;//a
		}
	}
}
//texture tetrahedron
void drawTextureTetrahedron(void)
{
	static GLfloat vertices[][3] = {
		{0,3,0},//0
		{-1,0,1},//1
		{0,0,-1},//2
		{1,0,1},//3
	};
	static GLint faces[4][3] = {
		{1,3,0},//0
		{2,1,0},//1
		{3,2,0},//2
		{1,2,3},//3
	};
	static GLfloat uvs[4][3][2] = {
		{ {0,0},{1,0},{0.5,1} },
		{ {0,1},{0,0},{0.5,1} },
		{ {1,0},{1,1},{0.5,1} },
		{ {0,0},{0,0},{0,0} },
	};

	int i;

	glBindTexture(GL_TEXTURE_2D, texNames[2]);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < 4; ++i)
	{
		if (i == 3)glDisable(GL_TEXTURE);
		glTexCoord2fv(uvs[i][0]);
		glVertex3fv(vertices[faces[i][0]]);
		glTexCoord2fv(uvs[i][1]);
		glVertex3fv(vertices[faces[i][1]]);
		glTexCoord2fv(uvs[i][2]);
		glVertex3fv(vertices[faces[i][2]]);
	}
	glEnd();
	glEnable(GL_TEXTURE);
}

//texture cube
void drawTextureCube(void)
{
	static GLfloat vertices[][3] = {
	{ -1.0,-1.0,-1.0 },//0
	{ 1.0,-1.0,-1.0 },//1
	{ 1.0,1.0,-1.0 },//2
	{ -1.0,1.0,-1.0 },//3
	{ -1.0,-1.0,1.0 },//4
	{ 1.0,-1.0,1.0 },//5
	{ 1.0,1.0,1.0 },//6
	{ -1.0,1.0,1.0 } //7
	};

	static GLfloat uvs[6][4][2] = { 
	{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },//face0
	{ { 0,0 },{ 2,0 },{ 2,2 },{ 0,2 } },//face1
	{ { 0,0 },{ 2,0 },{ 2,2 },{ 0,2 } },//face2
	{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },//face3
	{ { 0,0 },{ 2,0 },{ 2,2 },{ 0,2 } },//face4
	{ { 0,0 },{ 2,0 },{ 2,2 },{ 0,2 } }//face5
	};

	static GLint faces[6][4] = { 
		{ 4,5,6,7 },{ 0,4,7,3 },{ 3,7,6,2 },
		{ 6,5,1,2 },{2, 1, 0, 3},{ 0,1,5,4 } };
	int i;

	glBindTexture(GL_TEXTURE_2D, texNames[1]);

	for (i = 0; i < 6; ++i)
	{
		if (i == 3)glBindTexture(GL_TEXTURE_2D, texNames[0]);

		if (i == 1 || i == 4)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else if (i == 2 || i == 5)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glBegin(GL_POLYGON);
		glTexCoord2fv(uvs[i][0]);
		glVertex3fv(vertices[faces[i][0]]);
		glTexCoord2fv(uvs[i][1]);
		glVertex3fv(vertices[faces[i][1]]);
		glTexCoord2fv(uvs[i][2]);
		glVertex3fv(vertices[faces[i][2]]);
		glTexCoord2fv(uvs[i][3]);
		glVertex3fv(vertices[faces[i][3]]);
		glEnd();
	}
}


void readRawImage(const char *fileName, int size, GLubyte *data) {
	FILE * file;

	file = fopen(fileName, "rb");
	if (file == NULL) {
		printf("\n Cannot open file \n");
		exit(0);
	}

	fread(data, size, 1, file);
	fclose(file);
}


void myDisplay(void) {
	GLfloat light0Ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light0Position[] = { 2.0, 3.0, 6.0, 1.0 };
	GLfloat light0Diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light0Specular[] = { 0.5, 0.5, 0.5, 1.0 };

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();

	glTranslatef(0.0f, 0.f, -5.0f);
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	/* Textured cube */
	//drawTextureTetrahedron();
	drawTextureCube();
	glPopMatrix();

	glutSwapBuffers();
}


void rotateCube(int axis) {
	theta[axis] += 5.0;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	glutPostRedisplay();
}


void myKeyboard(unsigned char k, int x, int y) {
	if (k == '1') rotateCube(0);
	else if (k == '2') rotateCube(1);
	else if (k == '3') rotateCube(2);
	else if (k == 27) exit(0);
}


void myReshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void initialize(void) {
	/* Specify textures */
	//check image
	makeCheckImage();

	//fish image
	readRawImage("fish.raw", FISH_IMAGE_HEIGHT*FISH_IMAGE_WIDTH*3, fishImageData);

	//bandai
	readRawImage("bandai.raw", 256 * 256 * 3, bandaiData);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//make empty texture.
	glGenTextures(3, texNames);

	//make check texture and set parameters.
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_WIDTH, IMAGE_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	//make fish texture and set parameters.
	glBindTexture(GL_TEXTURE_2D, texNames[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FISH_IMAGE_WIDTH, FISH_IMAGE_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, fishImageData);

	//make bandai texture and set parameters.
	glBindTexture(GL_TEXTURE_2D, texNames[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FISH_IMAGE_WIDTH, FISH_IMAGE_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, bandaiData);

	//Enable texture mapping
	glEnable(GL_TEXTURE_2D);
}


int main(int argc, char **argv) {
	GLfloat globalAmbient[] = { 0.5, 0.5, 0.5, 1.0 };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Texture mapping");

	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	initialize();

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT_AND_BACK);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyboard);

	glutMainLoop();

	return 0;
}