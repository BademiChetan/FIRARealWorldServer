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

CvPoint bot_pos;
    

void calb_dist(int botID)
{
    float distance=0;
	int i=0,wait_times=30;
    unsigned char old_encoder_value=180,new_encoder_value=0,CAL_DIST=30;
    bot_pos = cvPoint( bot[botID].x, bot[botID].y );
    old_encoder_value=bot_code[botID][7];    
    while(abs(distance-CAL_DIST)>DIST_TOL)
    {
        e_sendenccmd( botID, 'F', CAL_DIST, 120 );
        while(!check_bot_free(botID)&&(i<wait_times))
        {
            cout<<"Waiting\r"<<endl;
            updateframe();
        }
		if(i==wait_times)
			continue;
        updateframe();
        distance = sqrt((bot_pos.x-bot[botID].x)*(bot_pos.x-bot[botID].x) + (bot_pos.y-bot[botID].y)*(bot_pos.y-bot[botID].y));
        new_encoder_value=CAL_DIST*(old_encoder_value/(float)distance);		//Assuming the encoder mapping to be linear, then slope is 200/CAL_DIST.
        cout<<"The distance  : "<<distance<<endl;
        cout<<"Existing value: "<<(int)old_encoder_value<<endl;
        cout<<"Proposed value: "<<(int)new_encoder_value<<endl;
        old_encoder_value=new_encoder_value;
        e_sendenccmd(botID,'P',new_encoder_value);
		bot_pos = cvPoint( bot[botID].x, bot[botID].y );
    }
}

void calb_angle(int botID)
{
    double diff_angle=0;
    unsigned char ne=0,oe=0,CAL_ANGLE=45;
    char action='l';
    double bot_angle=bot[botID].angle;
    oe=bot_code[botID][9];
    bot_status();
    cout<<"OE:"<<oe<<endl;
    while(abs(diff_angle-CAL_ANGLE)>ANGLE_TOL)
    {
        action=(bot[botID].angle<0)?'r':'l';
    	e_sendenccmd( botID,action, CAL_ANGLE);
        while(!check_bot_free(botID))
        {
            cout<<"Waiting\r"<<endl;
            updateframe();
        }
        updateframe();
        diff_angle =bot_angle-bot[botID].angle;
        bot_angle=bot[botID].angle;
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
    calb_dist(botID);
	cout<<"Testing if the bot goes for 20 cm"<<endl;
    while(getchar()!='c');	
	e_sendenccmd(botID,'F',20,150);
    while(!check_bot_free(botID))
    {
        cout<<"Waiting\r"<<endl;
        updateframe();
    }
    updateframe();
    cout<<"The distance  : "<<sqrt((bot_pos.x-bot[botID].x)*(bot_pos.x-bot[botID].x) + (bot_pos.y-bot[botID].y)*(bot_pos.y-bot[botID].y))<<endl;
//    calb_angle(botID);
    cout<<"Done calibration..testing"<<endl;
    e_sendenccmd(botID,'l',180);
    bot[botID].update();
}
