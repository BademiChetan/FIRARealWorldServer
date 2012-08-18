#pragma once
#include <cv.h>
#include <highgui.h>
#include <ctime> 
#include <boost/thread.hpp>
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

using namespace cv;
using namespace std;

CvCapture *capture = cvCreateCameraCapture(1);
// CvCapture *capture = cvCreateFileCapture( "multibot.avi" );

IplImage *img = cvQueryFrame( capture );
IplImage *hsv = cvCreateImage( cvSize( 640, 480), IPL_DEPTH_8U, 3 );
bool ip_done; 
int FrameCount = 0; 
CvRect goal_rect = cvRect( 0, 0, 0, 0 );
CvRect pitch;
CvPoint arena_center;
bool interrupt; 
bool reset; 

double prev_x[NUM_OF_OUR_BOTS + NUM_OF_OPP_BOTS + 1][FRAME_BACKUP]; 
double prev_y[NUM_OF_OUR_BOTS + NUM_OF_OPP_BOTS + 1][FRAME_BACKUP]; 
double prev_angle[NUM_OF_OUR_BOTS + NUM_OF_OUR_BOTS + 1][FRAME_BACKUP]; 
Coordinate home_positions[5]; 

void update_locations() {
    for(int i = 0; i < NUM_OF_OUR_BOTS; i++) {
        prev_x[i][FrameCount % FRAME_BACKUP] = bot[i].x; 
        prev_y[i][FrameCount % FRAME_BACKUP] = bot[i].y; 
        prev_angle[i][FrameCount % FRAME_BACKUP] = bot[i].angle; 
    }
}

void image_processing() {
    bot[0].basecolor = BOT0_BASECOLOR; bot[0].lcolor = BOT0_LCOLOR; bot[0].rcolor = BOT0_RCOLOR;
    bot[1].basecolor = BOT1_BASECOLOR; bot[1].lcolor = BOT1_LCOLOR; bot[1].rcolor = BOT1_RCOLOR;
    bot[2].basecolor = BOT2_BASECOLOR; bot[2].lcolor = BOT2_LCOLOR; bot[2].rcolor = BOT2_RCOLOR;
    bot[3].basecolor = BOT3_BASECOLOR; bot[3].lcolor = BOT3_LCOLOR; bot[3].rcolor = BOT3_RCOLOR;
    bot[4].basecolor = BOT4_BASECOLOR; bot[4].lcolor = BOT4_LCOLOR; bot[4].rcolor = BOT4_RCOLOR;

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
#ifdef IP_DEBUG
        cout << '!' ; 
        cout << "Frame Count = " << FrameCount << endl;
#endif 
    }
}

void home() {
    cout << "Home.\n"; 
    for (int j = 0; j < 30; j++){
        for (int i = 0; i < 5; i ++) {
            if (get_distance_to_point(bot[i].x, bot[i].y, 
                        home_positions[i].x, home_positions[i].y) > 2) {
                vector<Action> temp = hold(i, bot[i].x, bot[i].y, bot[i].angle, 
                        home_positions[i].x, home_positions[i].y, 40); 
                for (vector<Action>::iterator it = temp.begin(); it !=
                        temp.end(); it ++) {
                    it -> do_action(); 
                }
            }
        }
    }
    double required_angle = SIGN == -1 ? 0 : 180; 
    for (int j = 0; j < 30; j++){
        for (int i = 0; i < 5; i ++) {
            if (fabs(bot[i].angle - required_angle)  > 5 ) {
                double turn = bot[i].angle - required_angle; 
                e_sendenccmd(i, turn > 0 ? 'r' : 'l', fabs(turn)); 
            }
        }
    }
    cout << "We're home!\n"; 
}

void free_kick() {
    cout << "Free kick.\n"; 
}

void penalty() {
    cout << "Penalty.\n";
}
void listen_for_interrupt() {
    while (true) {
        cout << "Coming here?\n"; 
        char ch = 'y';
        while (true) {
            cout << "And here?\n"; 
            ch = getchar(); 
            cout << "Got " << ch << endl; 
            if (ch == 'g' || ch == 'f' || ch == 'p')
                break;
        }

        // interrupt all the bots
        interrupt = true; 
        for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
#ifdef ELEC
            e_sendenccmd(i, 's'); 
#endif
        }
        interrupt = false; 
        reset = true; 
        if (ch == 'g') {
            home(); 
        } else if (ch == 'f') {
            free_kick(); 
        } else if (ch == 'p') {
            penalty(); 
        }
        reset = false; 
        usleep(100000); 
    }
}
void ai() {
    while(true) {
        main_algo(); 
    }
}
int main( int argc, char** argv ){
    interrupt = false; 
    reset = false; 
    ip_done = false; 

    // Calculate the home positions
    home_positions[0] = Coordinate(SIGN * 104, 0); 
    home_positions[1] = Coordinate(SIGN * 80,   SIGN * 60) ; 
    home_positions[2] = Coordinate(SIGN * 80,   SIGN * - 60); 
    home_positions[3] = Coordinate(SIGN * 30,   SIGN * 60); 
    home_positions[4] = Coordinate(SIGN * 30,   SIGN * - 60); 

    if (SIGN < 0) {
        swap(home_positions[1], home_positions[2]); 
        swap(home_positions[3], home_positions[4]); 
    }


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
    // boost::thread opengl_thread(DrawScene); 
    // OpenGL stuff ends here } 


    // Thread which keeps listening for interrupt
    // boost::thread interrupt_thread(listen_for_interrupt); 


    // Fork off thread for IP. 
    boost::thread ip_thread(image_processing); 
    // Wait for IP to finish
    while(!ip_done); 
    cout << "IP is done!\n"; 
    //boost::thread algo_thread(ai); 


    struct timeval begin,end;
    double diff=0;
    gettimeofday(&begin,NULL);
    home(); 
    gettimeofday(&end,NULL);
    diff=difftime(end.tv_sec,begin.tv_sec);
    cout << "Home position took " << diff << " seconds. \n"; 


#ifdef ELEC
    Uend();
#endif


    ip_thread.join(); 
    // opengl_thread.join();
    // interrupt_thread.join(); 
    //algo_thread.join(); 
}
