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
#define ANGLE_TOL 1

void calb_dist(int botID)
{
    float distance=0;
    unsigned char old_encoder_value=180,new_encoder_value=0,CAL_DIST=30;
    old_encoder_value=bot_code[botID][7];    
    CvPoint bot_pos = cvPoint( bot[0].x, bot[0].y );
    while(abs(distance-CAL_DIST)>DIST_TOL)
    {
        e_sendenccmd( botID, 'F', CAL_DIST, 120 );
        while(!check_bot_free(botID))
        {
            cout<<"Waiting\r"<<endl;
            updateframe();
        }
        updateframe();
        distance = sqrt((bot_pos.x-bot[0].x)*(bot_pos.x-bot[0].x) + (bot_pos.y-bot[0].y)*(bot_pos.y-bot[0].y));
        new_encoder_value=CAL_DIST*(old_encoder_value/(float)distance);		//Assuming the encoder mapping to be linear, then slope is 200/CAL_DIST.
	cout<<"Div"<<(old_encoder_value/(float)distance)<<endl;
        cout<<"The distance  : "<<distance<<endl;
        cout<<"Existing value: "<<(int)old_encoder_value<<endl;
        cout<<"Proposed value: "<<(int)new_encoder_value<<endl;
        old_encoder_value=new_encoder_value;
        while(getchar()!='c');	
	e_sendenccmd(botID,'P',new_encoder_value);
	bot_pos = cvPoint( bot[0].x, bot[0].y );
    }
}

void calb_angle(int botID)
{
    double diff_angle=0;
    unsigned char ne=0,oe=0,CAL_ANGLE=45;
    double bot_angle=bot[0].angle;
    oe=bot_code[botID][9];
    bot_status();
    cout<<"OE:"<<oe<<endl;
    while(abs(diff_angle-CAL_ANGLE)>ANGLE_TOL)
    {
        e_sendenccmd( botID, 'l', CAL_ANGLE);
        while(!check_bot_free(botID))
        {
            cout<<"Waiting\r"<<endl;
            updateframe();
        }
        updateframe();
        diff_angle = bot_angle-bot[0].angle;
        if(diff_angle==0)
	{
	    oe=oe*3;
            e_sendenccmd(botID,'T',oe);
            cout<<"Division by zero error has been avoided"<<endl; 
            continue;
	}
        ne=CAL_ANGLE*(oe/(double)diff_angle);		//Assuming the encoder mapping to be linear, then slope is 200/CAL_DIST.
	cout<<"Div"<<(oe/(double)diff_angle)<<endl;
        cout<<"The angle     : "<<diff_angle<<endl;
        cout<<"Existing value: "<<(int)oe<<endl;
        cout<<"Proposed value: "<<(int)ne<<endl;
        oe=ne;
        while(getchar()!='c');	
	e_sendenccmd(botID,'T',ne);
    }
}

void kick_off_calibrate(int botID)
{
    unsigned char a=0;
    e_sendenccmd( botID, 'R');	
    img = cvQueryFrame(capture);
    e_sendenccmd(botID,'E');
//    calb_dist(botID);
    calb_angle(botID);
    cout<<"Done calibration..testing"<<endl;
    e_sendenccmd(botID,'l',180);
    bot[0].update();
}
