C = gcc

LDLIBS = -lglut -lGL -lGLU -lXmu -lX11  -lm
INC_PATH = /usr/local/include
LIB_PATH = /usr/local/lib

all: rtviewer

rtviewer: MainGL.c Scene.h Scene.c Geometry.h Geometry.c RayTrace.h RayTrace.c
	$(C) -I$(INC_PATH) -L$(LIB_PATH) $(LDLIBS) -o rtviewer MainGL.c Scene.c Geometry.c RayTrace.c

clean:
	rm rtviewer 
