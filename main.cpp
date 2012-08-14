#pragma once
#define ELEC // Remove x corruption to add elec stuff
#include <cv.h>
#include <highgui.h>
#include <ctime> 
#include <boost/thread.hpp>

using namespace cv;
using namespace std;

#include "SerialStream.h"
#include "select_arena.h"
#include"process_image.h"
#include "elec.h"
#include "contours.h"
#include "colors.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "ball.h"
#include "update_frame.h"
#include "visualization.h"
#include "global_var.h"
#include "algo.h"
#include "bot_actions.h"
#include "kick_off_calibration.h"

//CvCapture *capture = cvCreateCameraCapture(1);
CvCapture *capture = cvCreateFileCapture( "multibot.avi" );

IplImage *img = cvQueryFrame( capture );
IplImage *hsv = cvCreateImage( cvSize( 640, 480), IPL_DEPTH_8U, 3 );
bool ip_done; 
int FrameCount = 0; 
CvRect goal_rect = cvRect( 0, 0, 0, 0 );
CvRect pitch;
CvPoint arena_center;

double prev_x[5][10]; 
double prev_y[5][10]; 
double prev_angle[5][10]; 

void interrupt_near_arena() {
    for (int id = 0; id < NUM_OF_OUR_BOTS; id ++) {
        double x = bot[id].x; 
        double y = bot[id].y; 
        printf("Bot at (%f, %f)\n", x, y); 
        // If it's colliding with the arena, stop the bot. 
        // FIXME: Use global variables. 
        if (x > 215 || x < -215 || y > 175 || y < -175 ) {
        cout << "Interrupting.\n"; 
#ifdef ELEC
            e_sendenccmd(id, 's'); 
#endif
        }
    }

}

void update_locations() {
    for(int i = 0; i < NUM_OF_OUR_BOTS; i++) {
        prev_x[i][FrameCount % 10] = bot[i].x; 
        prev_y[i][FrameCount % 10] = bot[i].y; 
        prev_angle[i][FrameCount % 10] = bot[i].angle; 
    }
}
void image_processing() {
    bot[0].color = BOT0_COLOR;
    bot[1].color = BOT1_COLOR;
    bot[2].color = BOT2_COLOR;
    bot[3].color = BOT3_COLOR;
    bot[4].color = BOT4_COLOR;
    for(int i=0; i<15; i++)
    img = cvQueryFrame( capture );
    //This is selecting the arena part.
    goal_rect = select_rect( capture );
    //Propotionately calculating the total play area.
    pitch = cvRect( goal_rect.x, goal_rect.y - goal_rect.width * 7 / 25, goal_rect.width,goal_rect.width * 18 / 25 );
    arena_center = cvPoint( goal_rect.x + goal_rect.width / 2, goal_rect.y + goal_rect.height / 2 );

    for(int i = 0; i < NUM_OF_OUR_BOTS; i++)
        bot[i].location = pitch;

    for(int i = 0; i < NUM_OF_OPP_BOTS; i++)
        o_bot[i].location = pitch;

    Ball.location = pitch;

    for (int i = 0; i < 10; i ++){
        updateframe(); 
        update_locations(); 
    }


    ip_done = true; 

    while(true) {
        ++ FrameCount; 
        updateframe(); 
        update_locations(); 
        // interrupt_near_arena(); 
        cout << '!' ; 
    }
}

bool check_same_position(int id) {
    bool ret = true; 
    if (FrameCount < 3 )
        return ret; 
    for (int i = FrameCount - 3; i < FrameCount; i ++) {
        // FIXME: Find least count for all this once openGL stuff is written. 
        if ( !fabs(prev_x[id][FrameCount % 10] - prev_x[id][i % 10] ) < 5 ||
                !fabs(prev_y[id][FrameCount % 10] - prev_y[id][i % 10] ) < 5 ||
                !fabs(prev_angle[id][FrameCount % 10] - prev_angle[id][i % 60] ) < 10) {
            ret = false; 
        }
    }
    return ret; 
}

int main( int argc, char** argv ){
    ip_done = false; 

#ifdef ELEC
    Uinit();
#endif

    //OpenGL stuff starts here { 
    glutInit(&argc, argv);  
    /* Select type of Display mode:   
       Double buffer 
       RGBA color
       Alpha components supported 
       Depth buffer */  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
    glutInitWindowSize(ARENA_WIDTH*3,ARENA_HEIGHT*3);  
    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(50, 20);  
    /* Open a window */  
    window = glutCreateWindow("SahaS");  
    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawScene);  
    InitGL(ARENA_WIDTH+20, ARENA_HEIGHT+20);
    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawScene);
    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);
    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);
    boost::thread opengl_thread(DrawScene); 
    // OpenGL stuff ends here } 


    // Fork off thread for IP. 
    boost::thread ip_thread(image_processing); 
    // Wait for IP to finish
    while(!ip_done); 
    cout << "IP is done!\n"; 

    // Algo stuff starts here {

    while (true) {
        main_algo(); 
    }


#ifdef ELEC
    Uend();
#endif

    // Algo stuff ends here } 

    ip_thread.join(); 
    opengl_thread.join();
}
