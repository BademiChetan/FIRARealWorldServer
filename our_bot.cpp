#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include "global_var.h"
#include "process_image.h"
#include "colors.h"
#include "contours.h"
#include "our_bot.h"
#include "update_frame.h"

using namespace cv;
using namespace std;

our_bot bot[5];

our_bot::our_bot(){
    x = 0, y = 0, angle = 0;
    mask = new IplImage* [2];
    location = cvRect(arena_center.x - BOUND_RECT, arena_center.y - BOUND_RECT,
            2* BOUND_RECT, 2 * BOUND_RECT);
    mask[0] = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
    mask[1] = cvCreateImage( cvSize(640,480), IPL_DEPTH_8U, 1 );
}

our_bot::~our_bot(){
    cvReleaseImage( mask );
    delete mask;
}


double our_bot::pos(){
    CvPoint center;
    center = cvPoint( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );


    x = ( ( center.x - arena_center.x ) *250 ) / goal_rect.width;		//center wrt the arena center.
    y=-1*((center.y-arena_center.y)*250)/goal_rect.width;
    //printf("botx=%f boty=%f\n",x,y);
}

double our_bot::orientation(){

    angle = angl( cvPoint( 640, back_center.y ), back_center, front_center );

    if( angle < 0 )
        angle = -1 * angle;
    else if( angle == 1000)		//case where the bot is not found in the bounding box.
        angle = 0;
    else if( angle > 0 )
        angle = 360 - angle;

    //printf("angle%f\n",angle);
}


void our_bot::FindCenter(){
    vector <CvPoint> BackCenter = FindAllCenter( mask[1] );

    front_center = cvPoint( 0, 0 );
    back_center = cvPoint( 0, 0 );

    for( int i = 0; i < BackCenter.size(); i++ ){
        front_center = ClosestFrontCenter( mask[0], BackCenter[i] );
        if( front_center.x != 0 ){
            back_center = BackCenter[i];
            break;
        }

    }

}


void our_bot::update(){
    //Setting the tracking box as the region of interest.
    cvSetImageROI( mask[0], location );
    cvSetImageROI( mask[1], location );

    pick_color( mask, location, color );

    //The cvShowImage lines are for debuging.
    //cvShowImage( "back_center", mask[0] );
    //cvShowImage( "front_center", mask[1] );

    FindCenter();

    cvResetImageROI( mask[0] );
    cvResetImageROI( mask[1] );

    if( front_center.x != 0 && back_center.x != 0 ){
        //Taking care for the relative position changes due to ROI.
        front_center = cvPoint( front_center.x + location.x, front_center.y + location.y );
        back_center = cvPoint( back_center.x + location.x, back_center.y + location.y );
        bot_center = cvPoint( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
        update_location( location, bot_center );
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );
    pos();
    orientation();
}
