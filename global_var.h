#pragma once
#include <cv.h>
#include <highgui.h>

<<<<<<< HEAD
#define THRESH_AREA 200
=======
#include <cv.h>
#include <highgui.h>

#define THRESH_AREA 50
>>>>>>> a6b315ce05b639b962679ae9336f613166358679

#define OUR_JERSEY_COLOR 'b'

#define OPP_JERSEY_COLOR 'y'

<<<<<<< HEAD
#define NUM_OF_OUR_BOTS 3

#define NUM_OF_OPP_BOTS 0
=======
#define NUM_OF_OUR_BOTS 5

#define NUM_OF_OPP_BOTS 5

#define LEFT_GOAL_COLOR 'b'
>>>>>>> a6b315ce05b639b962679ae9336f613166358679

#define BOT0_COLOR 'd'
#define BOT1_COLOR 'p'
#define BOT2_COLOR 'v'
#define BOT3_COLOR 'g'
#define BOT4_COLOR 'r'

#define BOUND_RECT 30
#define BOT_WIDTH 20

#define ARENA_LENGTH    220
#define ARENA_WIDTH     180
#define GOAL_LENGTH     30
#define GOAL_WIDTH      15
#define BOT_LENGTH      8
#define BALL_DIAMETER   3
#define PI              3.14159265

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
