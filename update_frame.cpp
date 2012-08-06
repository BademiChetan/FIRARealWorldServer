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

void update_our_bot()
{
    for( int i=0;i < NUM_OF_OUR_BOTS;i++)
        bot[i].update();
}

void update_opp_bot()
{
    for( int i=0;i < NUM_OF_OPP_BOTS;i++)
        o_bot[i].update();
}
void updateframe(){

    img = cvQueryFrame( capture );

    int i;
    
    for(i=0;i<NUM_OF_OPP_BOTS;i++) {
        cvSetImageROI(img,o_bot[i].location);
        cvSetImageROI(hsv,o_bot[i].location);
        cvCvtColor( img, hsv, CV_BGR2HSV );
        cvResetImageROI(img);
        cvResetImageROI(hsv);
    }

   boost::thread our_bot_thread(update_opp_bot); 
   
       for(i=0;i<NUM_OF_OUR_BOTS;i++) {
        cvSetImageROI(img,bot[i].location);
        cvSetImageROI(hsv,bot[i].location);
        cvCvtColor( img, hsv, CV_BGR2HSV );
        cvResetImageROI(img);
        cvResetImageROI(hsv);
        bot[i].update();
    }

    cvSetImageROI(img,Ball.location);
    cvSetImageROI(hsv,Ball.location);
    cvCvtColor( img, hsv, CV_BGR2HSV );
    cvResetImageROI(img);
    cvResetImageROI(hsv);
    //Ball.update();

    our_bot_thread.join(); 


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
