CC = cc
CFLAGS = -Iinclude -I/usr/X11R7/include `pkg-config --cflags glut`
LDFLAGS =
LIBS = -L/usr/X11R7/lib `pkg-config --libs glut` -lGLU

OBJS = src/logo.o src/stb_image_write.o

.PHONY: all clean
.SUFFIXES: .c .o

all: ./logo

./logo: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f logo src/*.o
