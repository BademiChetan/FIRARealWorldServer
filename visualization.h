#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <unistd.h>

#define ARENA_WIDTH 250.0
#define ARENA_HEIGHT 180.0

/* The number of our GLUT window */
extern int window;

void DrawScene();

void ReSizeGLScene(int , int );

void InitGL( int, int );

void keyPressed( unsigned char, int, int );
