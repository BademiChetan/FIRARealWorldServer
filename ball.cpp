#pragma once
#include <cv.h>
#include <highgui.h>

#include "update_frame.h"
#include "colors.h"
#include "contours.h"
#include "global_var.h"
#include "ball.h"

using namespace  cv;
using namespace std;

ball Ball;

ball::ball(){
    color = 'o';
    center = cvPoint( 0, 0 );
    location = cvRect( 0, 0, 0, 0);
    mask = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
}


void ball::FindCenter( CvPoint &cen ){
    cen = LargestContourCenter( mask );	
}


void ball::update(){

    cvSetImageROI( mask, location );
    CvPoint cen;

    pick_color( &mask, location, color );
    FindCenter( cen );

    cvResetImageROI( mask );

    if( cen.x != 0 ){
        cen = cvPoint( cen.x + location.x, cen.y + location.y );
        center = cen;
        update_location( location, center );
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );

}
