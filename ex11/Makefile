C = gcc

LDLIBS = -lglut -lGL -lGLU -lXmu -lX11  -lm
INC_PATH = /usr/local/include
LIB_PATH = /usr/local/lib

all: rtviewer

rtviewer: MainGL.c Geometry.h Geometry.c RayTrace.h RayTrace.c Distance.h Distance.c Parameters.h Parameters.c Constants.h
	$(C) -I$(INC_PATH) -L$(LIB_PATH) $(LDLIBS) -o rtviewer MainGL.c Geometry.c RayTrace.c Distance.c Parameters.c

clean:
	rm rtviewer 
