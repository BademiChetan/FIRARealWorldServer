#pragma once
#define xELEC // Remove x corruption to add elec stuff
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

double prev_x[5][10]; 
double prev_y[5][10]; 
double prev_angle[5][10]; 
int still_count[5]; 

void algo(int id) {

    double x = bot[id].x; 
    double y = bot[id].y; 
    double angle = bot[id].angle; 
    double bx, by; 
    Ball.getCenter(bx, by); 

    // Hold Stuff Starts Here {

   // // Try to go behind the ball from the goal. 
   // double gx = -100; 
   // double gy = 0; 
   // double theta = get_angle_to_point(bx, by, gx, gy); 
   // cout << "Theta in degrees: " << theta << endl; 
   // theta *= PI / 180; 

   // // Get the point on the line from goal to ball 5cm behind it
   // double goto_x = bx - 5 * cos(theta); 
   // double goto_y = by - 5 * sin(theta); 

   // // Check if it lies in the arena
   // if (goto_x < -90 || goto_x > 90 || goto_y < -90 || goto_y > 90) {
   //     goto_x = bx; 
   //     goto_y = by; 
   // }


   // printf("Will hold (%f, %f). Ball is at (%f, %f).\n", goto_x, goto_y, bx, by); 
   // vector<Action> res = hold(id, x, y, angle, goto_x, goto_y); 
   // for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
   //     bot_queue[id].push(*it); 
   // }

    // Hold stuff ends here }


    // Goalie and defender starts here {
    double theta1 = get_angle_to_point(bx, by, +110, -25, true); 
    double theta2 = get_angle_to_point(bx, by, +110, 0, true); 
    double theta3 = get_angle_to_point(bx, by, +110, +25, true); 
    vector<Action> res; 
    if (id == 0) {
        // Goalie
        double goalie_x = +110 - 7.5; 
        double goalie_dx = goalie_x - bx; 
        double goalie_y = by + goalie_dx * ( tan(theta1) + tan(theta2) ) / 2; 
        res = defend(id, x, y, angle, goalie_x, goalie_y); 

    } else {
        // Defender 1
        double defender_x = +110 - 25; 
        double defender_dx = defender_x - bx; 
        double defender_y = by + defender_dx * ( tan(theta1) + tan(theta2) ) / 2; 
        res = defend(id, x, y, angle, defender_x, defender_y); 
    }

    for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
        bot_queue[id].push(*it); 
    }

    // Goalie and defender end here }

}
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

    updateframe(); 
    update_locations(); 


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
    memset(still_count, 0, sizeof(still_count)); 

#ifdef ELEC
    Uinit();
#endif

    // Fork off thread for IP. 
    boost::thread ip_thread(image_processing); 
    // Wait for IP to finish
    while(!ip_done); 
    cout << "IP is done!\n"; 

    // Elec stuff starts here {

//    while( c != 27 ){
//        for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
//            if (bot_queue[i].empty()) {
//                if (still_count[i] <= 10) {
//                    still_count[i] ++; 
//                    continue; 
//                }
//                still_count[i] = 0; 
//                algo(i); 
//            }
//        }
//
//        for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
//            if (check_bot_free(i) && !bot_queue[i].empty()) {
//                Action curr = bot_queue[i].front(); 
//               // Required for the "cool" hold
//                if (curr.speed == 0) {
//                    curr.do_action(); 
//                } else {
//                    if (fabs(angle_from_bot_to_ball(i) - bot[i].angle) < 15)
//                        curr.do_action();
//                }
//                bot_queue[i].pop(); 
//            }
//        }
//    }


#ifdef ELEC
    Uend();
#endif

    // Elec stuff ends here } 
    ip_thread.join(); 
}
