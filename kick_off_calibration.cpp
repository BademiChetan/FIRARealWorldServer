#pragma once
#include <highgui.h>
#include <cv.h>
#include <math.h>

#include "elec.h"
#include "our_bot.h"
#include "process_image.h"
#include "global_var.h"
#include "update_frame.h"

using namespace std;
#define DIST_TOL 1


void kick_off_calibrate(int botID)
{
    unsigned char a=0,old_encoder_value=180,new_encoder_value=0,CAL_DIST=30;

    CvPoint bot_pos = cvPoint( bot[0].x, bot[0].y );

    cout<<bot_pos.x<<' '<<bot_pos.y<<endl;
    e_sendenccmd( botID, 'R');	

    img = cvQueryFrame(capture);

    float distance=0;
    while((abs(distance-CAL_DIST)>DIST_TOL)&&(new_encoder_value!=old_encoder_value))
    {
        e_sendenccmd( botID, 'F', CAL_DIST, 120 );
        while(!check_bot_free(botID))
        {
            cout<<"Waiting\r"<<endl;
            updateframe();
        }
        updateframe();
        distance = sqrt((bot_pos.x-bot[0].x)*(bot_pos.x-bot[0].x) + (bot_pos.y-bot[0].y)*(bot_pos.y-bot[0].y));
        //Assuming the encoder mapping to be linear, then slope is 200/CAL_DIST.
        old_encoder_value=new_encoder_value;
        new_encoder_value=CAL_DIST*(180/(float)distance);
        e_sendenccmd(botID,'P',new_encoder_value);
        e_sendenccmd( botID, 'F', CAL_DIST, 120 );
        cout<<"The distance"<<distance<<endl;
        cout<<"Existing value:"<<(int)old_encoder_value<<endl;
        cout<<"Proposed value:"<<(int)new_encoder_value<<endl;
        while(getchar()!='c');	
    }
    bot[0].update();





    cvWaitKey(0);
}
