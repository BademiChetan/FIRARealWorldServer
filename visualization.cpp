/********************
  FIRA Visualization
            -Vasanth (Team SahaS)
********************/

#pragma once

#include "visualization.h"
#include "global_var.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "ball.h"

using namespace std;

/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(42.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)	   
        Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(42.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void DrawBall(double x, double y)
{
    glPushMatrix();
    glTranslatef(x,y,-24.2);
    glColor3f(1.0f,0.5f,0.0f); 
    glutSolidSphere(0.217f, 10, 10);
    glPopMatrix();
}

void DrawColor( char color ) {
    if( color == 'b' )
        glColor3f( 0.0f, 0.0f, 1.0f );
    else
        glColor3f( 1.0f, 1.0f, 0.0f );
  /*  GLfloat ret[3];
    if( color == 'b' ) {
        ret[0] = 0.0f;
        ret[1] = 0.0f;
        ret[2] = 1.0f;
    }
    else {
        ret[0] = 1.0f;
        ret[1] = 1.0f;
        ret[2] = 0.0f;
    }*/


}


void DrawBot( double x, double y, double angle, char color ) {
    
    //convert to opengl's angle
    if( angle != -1.0 )
    angle -= 90;

    //Bot
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(angle,0,0,1);

    glBegin(GL_QUADS);

    DrawColor( color );

    glVertex3f(0.4f, 0.4f,-24.2f);
    glVertex3f(-0.4f, 0.4f,-24.2f);
    glVertex3f(-0.4f, -0.4f,-24.2f);
    glVertex3f(0.4f, -0.4f,-24.2f);	

    glEnd(); 


    //Arrow mark to indicate the direction
    if(angle != -1.0) {
    glBegin(GL_QUADS);

    glColor3f(1.0f,0.0f,0.0f); 

    glVertex3f(0.1f, 0.2f,-24.19999f);
    glVertex3f(-0.1f, 0.2f,-24.19999f);
    glVertex3f(-0.1f, -0.2f,-24.19999f);
    glVertex3f(0.1f, -0.2f,-24.19999f);	

    glEnd();


    //Arrow towards the front
    glBegin(GL_TRIANGLES);

    glColor3f(1.0f,0.0f,0.0f); 

    glVertex3f(-0.2f, 0.2f,-24.19999f);
    glVertex3f(0.2f, 0.2f,-24.19999f);	
    glVertex3f(0.0f, 0.4f,-24.19999f);
    }
    glEnd();
    glPopMatrix();

}

void DrawLine(int x1, int y1, int z1, int x2, int y2, int z2, float width) {
    
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f( 1, 1, 1 );
    glVertex3f( x1, y1, z1 );
    glVertex3f( x2, y2, z2 );
    glColor3f( 1, 1, 1 );
    glVertex3f( x1+width/2, y1+width/2, z1+width/2 );
    glVertex3f( x2+width/2, y2+width/2, z2+width/2 );
    glColor3f( 1, 1, 1 );
    glVertex3f( x1+width, y1+width, z1+width );
    glVertex3f( x2+width, y2+width, z2+width );
    glEnd();

}

void GoalColor(char color) {
if(color == 'b')
    glColor3f(0.0f,0.0f,0.2f);
else if(color == 'y')
    glColor3f(0.2f,0.2f,0.0f); 
}


//Arena is drawn anti-clockwise.
void DrawArena() {
        
    char right_goal_color = 'b';

    if(LEFT_GOAL_COLOR == 'b')
        right_goal_color = 'y';

    glLoadIdentity();

    glBegin(GL_POLYGON);

    glColor4f(0.0f,0.0f,0.0f,0.5f); 
    //starting with the bottom left corner of the arena.
    glVertex3f(-11.0f, -8.3f, -25.1f);
    glVertex3f(-10.3f, -9.0f, -25.1f);
    glVertex3f(10.3f, -9.0f, -25.1f);
    glVertex3f(11.0f, -8.3f, -25.1f);

    //Right goal-post to left goal-post
    glVertex3f(11.0f, 8.3f, -25.1f);    
    glVertex3f(10.3f, 9.0f, -25.1f);
    glVertex3f(-10.3f, 9.0f, -25.1f);
    glVertex3f(-11.0f, 8.3f, -25.1f);

    glEnd();

    //Right side goal-post
    glBegin(GL_QUADS);
    GoalColor(right_goal_color);

    glVertex3f(11.0f, -2.0f, -25.0f);
    glVertex3f(12.5f, -2.0f, -25.0f);	
    glVertex3f(12.5f, 2.0f, -25.0f);	
    glVertex3f(11.0f, 2.0f, -25.0f);

    glEnd();

    //Left-goal post
    glBegin(GL_QUADS);
    GoalColor(LEFT_GOAL_COLOR);

    glVertex3f(-11.0f, 2.0f, -25.0f);
    glVertex3f(-12.5f, 2.0f, -25.0f);    
    glVertex3f(-12.5f, -2.0f, -25.0f);
    glVertex3f(-11.0f, -2.0f, -25.0f);

    glEnd();
    
    //Side wall starts here

    //Goal Posts
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-12.5f, 2.0f, -25.0f);
    glVertex3f(-12.5f, -2.0f, -25.0f);    
    glVertex3f(-12.5f, -2.0f, -24.5f);
    glVertex3f(-12.5f, 2.0f, -24.5f);

    glEnd();
    
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-12.5f, 2.0f, -25.0f);
    glVertex3f(-12.5f, 2.0f, -24.5f);    
    glVertex3f(-11.0f, 2.0f, -24.5f);
    glVertex3f(-11.0f, 2.0f, -25.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-12.5f, -2.0f, -25.0f);
    glVertex3f(-12.5f, -2.0f, -24.5f);    
    glVertex3f(-11.0f, -2.0f, -24.5f);
    glVertex3f(-11.0f, -2.0f, -25.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(12.5f, 2.0f, -25.0f);
    glVertex3f(12.5f, 2.0f, -24.5f);    
    glVertex3f(11.0f, 2.0f, -24.5f);
    glVertex3f(11.0f, 2.0f, -25.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(12.5f, -2.0f, -25.0f);
    glVertex3f(12.5f, -2.0f, -24.5f);    
    glVertex3f(11.0f, -2.0f, -24.5f);
    glVertex3f(11.0f, -2.0f, -25.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(12.5f, 2.0f, -25.0f);
    glVertex3f(12.5f, -2.0f, -25.0f);    
    glVertex3f(12.5f, -2.0f, -24.5f);
    glVertex3f(12.5f, 2.0f, -24.5f);

    glEnd();
    //Goal Post side wall ends
    
    //Other parts of the arena
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-11.0f, 2.0f, -25.0f);
    glVertex3f(-11.0f, 8.3f, -25.0f);    
    glVertex3f(-11.0f, 8.3f, -24.5f);
    glVertex3f(-11.0f, 2.0f, -24.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-11.0f, -2.0f, -25.0f);
    glVertex3f(-11.0f, -8.3f, -25.0f);    
    glVertex3f(-11.0f, -8.3f, -24.5f);
    glVertex3f(-11.0f, -2.0f, -24.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(11.0f, 2.0f, -25.0f);
    glVertex3f(11.0f, 8.3f, -25.0f);    
    glVertex3f(11.0f, 8.3f, -24.5f);
    glVertex3f(11.0f, 2.0f, -24.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(11.0f, -2.0f, -25.0f);
    glVertex3f(11.0f, -8.3f, -25.0f);    
    glVertex3f(11.0f, -8.3f, -24.5f);
    glVertex3f(11.0f, -2.0f, -24.5f);

    glEnd();
    
    //The isoceles triangular cut part
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-11.0f, -8.3f, -25.0f);
    glVertex3f(-10.3f, -9.0f, -25.0f);    
    glVertex3f(-10.3f, -9.0f, -24.5f);
    glVertex3f(-11.0f, -8.3f, -24.5f);

    glEnd();
    
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(11.0f, -8.3f, -25.0f);
    glVertex3f(10.3f, -9.0f, -25.0f);    
    glVertex3f(10.3f, -9.0f, -24.5f);
    glVertex3f(11.0f, -8.3f, -24.5f);

    glEnd();
    
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-11.0f, 8.3f, -25.0f);
    glVertex3f(-10.3f, 9.0f, -25.0f);    
    glVertex3f(-10.3f, 9.0f, -24.5f);
    glVertex3f(-11.0f, 8.3f, -24.5f);

    glEnd();
    
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(11.0f, 8.3f, -25.0f);
    glVertex3f(10.3f, 9.0f, -25.0f);    
    glVertex3f(10.3f, 9.0f, -24.5f);
    glVertex3f(11.0f, 8.3f, -24.5f);

    glEnd();
    
    //The top line
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-10.3f, 9.0f, -25.0f);
    glVertex3f(10.3f, 9.0f, -25.0f);    
    glVertex3f(10.3f, 9.0f, -24.5f);
    glVertex3f(-10.3f, 9.0f, -24.5f);

    glEnd();

    //The Bottom line
    glBegin(GL_QUADS);

    glColor3f(1.0f,1.0f,1.0f); 
    glVertex3f(-10.3f, -9.0f, -25.0f);
    glVertex3f(10.3f, -9.0f, -25.0f);    
    glVertex3f(10.3f, -9.0f, -24.5f);
    glVertex3f(-10.3f, -9.0f, -24.5f);

    glEnd();
    // Side wall ends here
    
    //Central Vertical line
    DrawLine( 0.0f, 9.0f, -25.0f, 0.0f, -9.0f, -25.0f, 0.03f );

    //Right side goal Lines
//    DrawLine( 11.0f, 2.5f, -25.0f, 11.0f, -2.5f, -25.0f, 0.03f );
//    DrawLine( 9.5f, 2.5f, -25.0f, 9.5f, -2.5f, -25.0f, 0.03f );
//    DrawLine( 9.5f, 2.5f, -25.0f, 11.0f, 2.5f, -25.0f, 0.03f );
//    DrawLine( 11.0f, -2.5f, -25.0f, 9.5f, -2.5f, -25.0f, 0.03f );

//    //Left side goal Lines
//    DrawLine( -11.0f, 2.5f, -25.0f, -11.0f, -2.5f, -25.0f, 0.03f );
//    DrawLine( -9.5f, 2.5f, -25.0f, -9.5f, -2.5f, -25.0f, 0.03f );
//    DrawLine( -9.5f, 2.5f, -25.0f, -11.0f, 2.5f, -25.0f, 0.03f );
//    DrawLine( -11.0f, -2.5f, -25.0f, -9.5f, -2.5f, -25.0f, 0.03f );

//    DrawLine( 7.5f, 4.0f, -25.0f, 7.5f, -4.0f, -25.0f, 0.03f);
//
//    //Left side vertical Lines
//    DrawLine( -7.5f, 4.0f, -25.0f, -7.5f, -4.0f, -25.0f, 0.03f);
//    DrawLine( -9.5f, 2.5f, -25.0f, -9.5f, -2.5f, -25.0f, 0.03f);
//    DrawLine( -11.0f, 2.0f, -25.0f, -11.0f, -2.0f, -25.0f, 0.03f);
//
//    //Right side horizontal Lines
//    DrawLine( 7.5f, 4.0f, -25.0f, 11.0f, 4.0f, -25.0f, 0.03f);
//    DrawLine( 7.5f, -4.0f, -25.0f, 11.0f, -4.0f, -25.0f, 0.03f);
//
//    //Penalty area horizontal lines
//    DrawLine( 11.0f, 4.0f, -25.0f, 7.5f, 4.0f, -25.0f, 0.03f);
//    DrawLine( -11.0f, 4.0f, -25.0f, -7.5f, 4.0f, -25.0f, 0.03f);
//    DrawLine( -11.0f, -4.0f, -25.0f, -7.5f, -4.0f, -25.0f, 0.03f);
//    DrawLine( 11.0f, -4.0f, -25.0f, 7.5f, -4.0f, -25.0f, 0.03f);
//
//    //Goal area horizontal lines
//    DrawLine( 11.0f, 2.5f, -25.0f, 9.5f, 2.5f, -25.0f, 0.03f);
//    DrawLine( -11.0f, 2.5f, -25.0f, -9.5f, 2.5f, -25.0f, 0.03f);
//    DrawLine( -11.0f, -2.5f, -25.0f, -9.5f, -2.5f, -25.0f, 0.03f);
//    DrawLine( 11.0f, -2.5f, -25.0f, 9.5f, -2.5f, -25.0f, 0.03f);
//
}

/* The main drawing function.*/ 
void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset The View
    
    DrawArena();

    double x=0.0,y=0.0;
    
    for(int i=0; i<NUM_OF_OUR_BOTS; i++) {
        DrawBot( (double)bot[i].x/10, (double)bot[i].y/10,
                (double)bot[i].angle , OUR_JERSEY_COLOR );
    }
    
    for(int i=0; i<NUM_OF_OPP_BOTS; i++) {
        DrawBot( (double)o_bot[i].x/10, (double)o_bot[i].y/10,
                (double)0.0, OPP_JERSEY_COLOR);
    }

    DrawBall( (double)Ball.x/10, (double)Ball.y/10 );

    // swap the buffers to display, since double buffering is used.
    glutSwapBuffers();


    glutMainLoop();  
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* sleep to avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) 
    { 
        /* shut down our window */
        glutDestroyWindow(window); 

        /* exit the program...normal termination. */
        exit(0);                   
    }
}

