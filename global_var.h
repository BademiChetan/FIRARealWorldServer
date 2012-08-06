#pragma once
#include <cv.h>
#include <highgui.h>

#define THRESH_AREA 50

#define OUR_JERSEY_COLOR 'b'

#define OPP_JERSEY_COLOR 'y'

#define NUM_OF_OUR_BOTS 1

#define NUM_OF_OPP_BOTS 2

#define BOT0_COLOR 'r'
#define BOT1_COLOR 'p'
#define BOT2_COLOR 'g'
#define BOT3_COLOR 'v'
#define BOT4_COLOR 'd'

#define BOUND_RECT 30

#define BOT_WIDTH 20

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
