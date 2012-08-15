#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <unistd.h>
#include "global_var.h"


/* The number of our GLUT window */
extern int window;

void DrawScene();

void ReSizeGLScene(int , int );

void InitGL( int, int );

void keyPressed( unsigned char, int, int );
