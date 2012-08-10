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
// CvCapture *capture = cvCreateFileCapture( "multibot.avi" );

IplImage *img = cvQueryFrame( capture );
IplImage *hsv = cvCreateImage( cvSize( 640, 480), IPL_DEPTH_8U, 3 );
bool ip_done; 
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

    // TODO: Move this to elec.cpp 
    // If it's colliding with the arena, stop the bot. 
    if (x > ARENA_LENGTH - BOT_LENGTH / 2 ||
            x < - ARENA_LENGTH + BOT_LENGTH / 2 ||
            y > ARENA_WIDTH - BOT_LENGTH / 2 || 
            y < - ARENA_WIDTH + BOT_LENGTH / 2) {
#ifdef ELEC
        e_sendenccmd(id, 's'); 
#endif
    }


    vector<Action> res = hold(id, x, y, angle, bx, by); 
    for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
        bot_queue[id].push(*it); 
    }

}
void image_processing() {
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

    for(int i = 0; i < NUM_OF_OUR_BOTS; i++)
        bot[i].location = pitch;

    for(int i = 0; i < NUM_OF_OPP_BOTS; i++)
        o_bot[i].location = pitch;

    Ball.location = pitch;

    updateframe(); 
    ip_done = true; 

    while(true) {
        updateframe(); 
        cout << '!' ; 
    }
}

int main( int argc, char** argv ){
    ip_done = false; 

#ifdef ELEC
    Uinit();
#endif

    // Fork off thread for IP. 
    boost::thread ip_thread(image_processing); 
    // Wait for IP to finish
    while(!ip_done); 
    cout << "IP is done!\n"; 

    e_sendenccmd(1, 'F', 100, 155); 



    // Elec stuff starts here {


   // while( c != 27 ){
   //     for (int i = 1; i <= 1; i ++) {
   //         if (bot_queue[i].empty()) {
   //             cout << bot[i].x << ' ' << bot[i].y << ' ' << bot[i].angle << endl; 
   //             algo(i); 
   //         }
   //     }

   //     for (int i = 1; i <= 1; i ++) {
   //         if (check_bot_free(i)) {
   //             Action curr = bot_queue[i].front(); 
   //             curr.do_action(); 
   //             bot_queue[i].pop(); 
   //         }
   //     }

   // }


#ifdef ELEC
    Uend();
#endif

    // Elec stuff ends here } 
    ip_thread.join(); 
}
