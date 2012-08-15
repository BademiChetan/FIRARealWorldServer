#pragma once
#include <cv.h>
#include <highgui.h>

//#define THRESH_AREA 5
#define THRESH_AREA 5        //without boundaries

//#define BASE_AREA_THRESH 30
#define BASE_AREA_THRESH 160     //without boundaries

#define OUR_JERSEY_COLOR 'b'

#define OPP_JERSEY_COLOR 'y'

#define LEFT_GOAL_COLOR 'y'

#define NUM_OF_OUR_BOTS 3

#define NUM_OF_OPP_BOTS 0

#define BOT0_BASECOLOR 'g'
#define BOT0_LCOLOR OUR_JERSEY_COLOR
#define BOT0_RCOLOR 'v'

#define BOT1_BASECOLOR OUR_JERSEY_COLOR
#define BOT1_LCOLOR 'v'
#define BOT1_RCOLOR 'g'

#define BOT2_BASECOLOR OUR_JERSEY_COLOR
#define BOT2_LCOLOR 'g'
#define BOT2_RCOLOR 'v'

#define BOT3_BASECOLOR 'g'
#define BOT3_LCOLOR 'v'
#define BOT3_RCOLOR OUR_JERSEY_COLOR

#define BOT4_BASECOLOR 'v'
#define BOT4_LCOLOR 'g'
#define BOT4_RCOLOR OUR_JERSEY_COLOR



#define BOUND_RECT 30
#define BOT_WIDTH 20

#define ARENA_WIDTH     250
#define ARENA_HEIGHT    180
#define GOAL_LENGTH     30
#define GOAL_WIDTH      15
#define BOT_LENGTH      8
#define BALL_DIAMETER   3
#define PI              3.14159265
#define MAX_SPEED       140
#define MIN_DIST        100
#define EPSILON         1e-6

/**********************
  co-ordinates :
    x-axis - right from the center (+ve)
    y-axis - upwards from the center (+ve)
    angle  - starts from the +ve x-axis and increases anticlockwise (0-360).
  ********************/
extern CvRect goal_rect;
extern CvPoint arena_center;
extern CvRect pitch;
extern char c;
extern CvCapture *capture;
extern IplImage *img;
extern IplImage *hsv;
extern CvSeq* contours, *finalcon;
extern int FrameCount; 
extern int still_count[5]; 
