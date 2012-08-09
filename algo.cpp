#include <cstdio>
#include <cmath> 
#include <cv.h>
#include <highgui.h>
#include <SerialStream.h>

#include "elec.h"
#include "our_bot.h"
#include "ball.h"
#include "opp_bot.h"
#include "global_var.h"

/*
   All the coordinates are from the center of the field. 
   The opponent's goal ranges from (110,-30) to (110,30). 
   Our goal ranges from (-110,-30) to (-110,30). 

 */




// Pushes a ball to the goal

double distance(double x1,double y1,double x2,double y2){
    return sqrt( ( x2 -x1 ) * ( x2 - x1 ) + ( y2 - y1 ) * ( y2 - y1 )  ) ; 
}

void goal(){
    /* // Assume we're shooting to the centre of the goal to maximize probability. 
    // Are we in contact with the ball? 
    double bx,by;

    bx = (Ball.center.x-arena_center.x)*250/pitch.width;
    by = (Ball.center.y-arena_center.y)*250/pitch.width;

    double botx,boty;
    botx = (bot[0].bot_center.x-arena_center.x)*250/pitch.width;
    boty = (bot[0].bot_center.y-arena_center.y)*250/pitch.width;

    //botx = boty = 0 ; 
    //bx = by = 1 ; 
    if ( distance(bx,by,botx,boty) <= 4 + 1.5 ){
    // We're in contact with the ball, guys! Keep going strong! :D



    // Find the different line of sights. 
    double los1 = atan( ( 110 - by)  / ( - bx ) ) ; 
    los1 = los1 * 180 / PI ; 
    double los2 = atan( ( 110 - boty ) / ( -botx ) ) ; 
    los2 = los2 * 180 / PI ; 

    // The bot and the ball are not aligned towards the goal! Turn the bot appropriately! 
    if ( fabs( los1 - los2 ) >= 2.0 ){
    if ( los2 > los1 ){
    turn(1, (int)(los2 - los1 + 0.5 ) ) ; 
    }
    else{
    turn( 1, (int)(los1 - los2 + 0.5 ) ) ; 
    }
    }

    // Now that we're aligned, push it by some amount. 
    move_forward(1,(int)( normal_velocity / frame_rate ) ) ; 

    }else{
    // We're not yet in contact with the ball. We have some work to do. 

    // Let's move to the ball. 
    double los1 = atan( ( by- boty ) / ( bx - botx ) ) ; 
    double los2;
    los2 = bot[0].orientation();

    if ( fabs( los2 - los1 ) >= 2.0  ){
    if ( los2 > los1 ){
    turn(1, (int)(los2 - los1 + 0.5 ) ) ; 
    }
    else{
    turn( 1, (int)(los1 - los2 + 0.5 ) ) ; 
    }
    }

    move_forward(1, (int)(normal_velocity / frame_rate ) ); 

    }
     */
}
