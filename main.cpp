#pragma once
#define ELEC // Remove x corruption to add elec stuff
#include <cv.h>
#include <highgui.h>
#include <ctime> 

using namespace cv;
using namespace std;

#include "SerialStream.h"
#include "select_arena.h"
#include "process_image.h"
#include "elec.h"
#include "contours.h"
#include "colors.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "ball.h"
#include "update_frame.h"
#include "global_var.h"
#include "bot_actions.h"
#include "kick_off_calibration.h"

CvCapture *capture = cvCreateCameraCapture(1);
//CvCapture *capture = cvCreateFileCapture( "multibot.avi" );

IplImage *img = cvQueryFrame( capture );
IplImage *hsv = cvCreateImage( cvSize( 640, 480), IPL_DEPTH_8U, 3 );
int FrameCount = 0; 
CvRect goal_rect = cvRect( 0, 0, 0, 0 );
CvRect pitch;
CvPoint arena_center;
queue<Action> bot_queue[5]; 

void algo(int id) {
    double x = bot[id].x; 
    double y = bot[id].y; 
    double angle = bot[id].angle; 
    
    double bx, by; 
    Ball.getCenter(bx, by); 

    // if not very close to the ball, go to the ball
    if ( fabs(bx - x) > 1  || fabs(y - by) > 1 ) {
        vector<Action> res = hold(id, x, y, angle, bx, by); 
        for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
            bot_queue[id].push(*it); 
        }
    } else {
        printf("Already holding the ball dude! WTF!\n"); 
    }
}

int main( int argc, char** argv ){

#ifdef ELEC
    Uinit();
#endif

    int i = 0;	

    bot[0].color = BOT0_COLOR;
    bot[1].color = BOT1_COLOR;
    bot[2].color = BOT2_COLOR;
    bot[3].color = BOT3_COLOR;
    bot[4].color = BOT4_COLOR;

    img = cvQueryFrame( capture );
    //This is selecting the arena part.
    goal_rect = select_rect( capture );
    //Propotionately calculating the total play area.
    pitch = cvRect( goal_rect.x, goal_rect.y - goal_rect.width * 7 / 25, goal_rect.width,goal_rect.width * 18 / 25 );
    arena_center = cvPoint( goal_rect.x + goal_rect.width / 2, goal_rect.y + goal_rect.height / 2 );

    for( i = 0; i < NUM_OF_OUR_BOTS; i++ )
        bot[i].location = pitch;

    for( i = 0; i < NUM_OF_OPP_BOTS; i++ )
        o_bot[i].location = pitch;

    Ball.location = pitch;
    int frames=0;

    while( c != 27 ){

        updateframe(); 

        if (bot_queue[0].empty()) {
            algo(0); 
        } else {
            printf("."); 
        }

       // for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
       //     if (bot_queue[i].empty()) {
       //         algo(i); 
       //     }
       // }

#ifdef ELEC
        for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
            if (check_bot_free(i)) {
                Action curr = bot_queue[i].front(); 
                curr.do_action(); 
                bot_queue[i].pop(); 
            }
        }
#endif

    }

#ifdef ELEC
    Uend();
#endif
}
