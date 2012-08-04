#pragma once
#include <boost/thread/thread.hpp>
#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <SerialStream.h>

#include "process_image.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "global_var.h"
#include "ball.h"

using namespace cv;
using namespace std;


void update_location( CvRect &location, CvPoint center ){
    location = Rect( center.x - BOUND_RECT, center.y - BOUND_RECT, 2 * BOUND_RECT, 2 * BOUND_RECT );
}

void expand_location( CvRect &location ){
    location = Rect( location.x - BOUND_RECT, location.y - BOUND_RECT, location.width + 2 * BOUND_RECT, location.height + 2 * BOUND_RECT );
}

void limit_location_within_arena( CvRect &location ){	

    if( location.x < goal_rect.x )
        location.x = goal_rect.x;

    if( location.y < pitch.y)
        location.y = pitch.y;

    if( location.width + location.x > pitch.x + pitch.width )
        location.width = pitch.x + pitch.width - location.x;

    if( location.height + location.y > pitch.y + pitch.height )
        location.height = pitch.y + pitch.height - location.y;

}

void updateframe(){

    img = cvQueryFrame( capture );

    cvCvtColor( img, hsv, CV_BGR2HSV );

    boost::thread_group our_bot_group; 
    boost::thread_group opp_bot_group; 


    for( int i = 0; i < NUM_OF_OUR_BOTS; i++ ) {
        our_bot_group.create_thread(boost::bind(&our_bot::update, &bot[i])); 
    }

    for( int i = 0; i < NUM_OF_OPP_BOTS; i++ ) {
        opp_bot_group.create_thread(boost::bind(&opp_bot::update, &o_bot[i])); 
    }

    Ball.update();

    // Join all the threads. 
    our_bot_group.join_all(); 
    opp_bot_group.join_all(); 


    //Not rendering all the frames to decrease the code execution time.
    if( FrameCount % 5 == 0 )
    {
        cvCircle( img, Ball.center, Ball.location.width / 5, CV_RGB( 255, 0, 0 ), 1, 8, 0 );

        cvCircle( img, arena_center, pitch.width / 10, CV_RGB( 180, 180, 255 ), 1, 8, 0 );

        for( int i = 0; i < NUM_OF_OUR_BOTS; i++ ){
            cvCircle( img, bot[i].bot_center, 2, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
            cvRectangle( img, cvPoint( bot[i].location.x, bot[i].location.y ),
                    cvPoint( bot[i].location.x + bot[i].location.width, bot[i].location.y + bot[i].location.height ),
                    cvScalar( 255, 0, 0, 0 ), 1, 4, 0 );
        }

        for( int i = 0; i < NUM_OF_OPP_BOTS; i++ ){
            cvCircle( img, o_bot[i].center, 2, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
            cvRectangle( img, cvPoint( o_bot[i].location.x, o_bot[i].location.y ),
                    cvPoint( o_bot[i].location.x + o_bot[i].location.width, o_bot[i].location.y + o_bot[i].location.height ),
                    cvScalar( 0, 0, 255, 0 ), 1, 4, 0 );
        }

        cvShowImage( "SAHAS", img);

        c = cvWaitKey( 1 );
    }
}
