#pragma once
#define xELEC									//Remove x corruption to remove elec stuff
#include <cv.h>
#include <highgui.h>

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
#include "kick_off_calibration.h"

CvCapture *capture = cvCreateCameraCapture(1);
//CvCapture *capture = cvCreateFileCapture( "multibot.avi" );

IplImage *img = cvQueryFrame( capture );
IplImage *hsv = cvCreateImage( cvGetSize( img ), IPL_DEPTH_8U, 3 );
int FrameCount = 0;
CvRect goal_rect = cvRect( 0, 0, 0, 0 );
CvRect pitch;
CvPoint arena_center;

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
#ifdef ELEC
   /* e_sendenccmd(0,'F',50,80);
    while(!check_bot_free(0))
    {
        updateframe();
        cout<<++frames<<endl;
    }
    cout<<"Medium version"<<endl;
    frames=0;
    e_sendenccmd(0,'F',50,120);
    while(!check_bot_free(0))
    {
        updateframe();
        cout<<++frames<<endl;
    }*/
    cout<<"Faster version"<<endl;
    frames=0;
    e_sendenccmd(0,'F',150,254);
    while(!check_bot_free(0))
    {
        updateframe();
        cout<<++frames<<endl;
    }
    exit(0);
#endif

    while( c != 27 ){


        updateframe();


    cout<<"bot values = "<<bot[0].x<<' '<<bot[0].y<<' '<<bot[0].angle<<endl;


        FrameCount++;

        cout<<FrameCount<<endl;

        //For debugging - checking frame by frame.
        //c=' ';
        if( c == ' ' ){
            c = cvWaitKey( 0 );
            c = 0;
        }
    }

#ifdef ELEC
    Uend();
#endif
}
