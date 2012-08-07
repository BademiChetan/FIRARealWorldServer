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

    double bx = Ball.center.x; 
    double by = Ball.center.y; 

    // if not very close to the ball, go to the ball
    if ( fabs(bx - x) > 1 && fabs(y - by) > 1 ) {
        vector<Action> res = hold(id, x, y, angle, bx, by); 
        for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
            bot_queue[id].push(*it); 
        }
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
#ifdef ELEC
        bot_status(); 
#endif
        updateframe(); 
        double x2, y2; 
        double x1 = bot[0].x; 
        double y1 = bot[0].y; 
        double angle = bot[0].angle; 
        Ball.getCenter(x2, y2); 
        printf("%d:  Bot at (%lf, %lf) and oriented at %lf\n", FrameCount, x1, y1, angle); 
        printf("%d:  Ball at (%lf, %lf)\n", FrameCount, x2, y2); 
        FrameCount ++; 
        // Just turn towards the wall
        double orient = get_angle_to_point(x1, y1, x2, y2); 
        cout << orient << endl; 
        if (check_bot_free(0)) {
            printf("Turning now...\n"); 
            e_sendenccmd(0, orient > angle ? 'l' : 'r', (int)(fabs(angle - orient))); 
        }

       // for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
       //     if (bot_queue[i].empty()) {
       //         algo(i); 
       //     }
       // }

#ifdef ELEC
       // for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
       //     if (check_bot_free(i)) {
       //         Action curr = bot_queue[i].front(); 
       //         curr.do_action(); 
       //         bot_queue[i].pop(); 
       //     }
       // }
#endif

    }

#ifdef ELEC
    Uend();
#endif
}
