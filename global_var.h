#pragma once

#define THRESH_AREA 50

#define OUR_JERSEY_COLOR 'b'

#define OPP_JERSEY_COLOR 'y'

#define NUM_OF_OUR_BOTS 5

#define NUM_OF_OPP_BOTS 0

#define BOT0_COLOR 'p'
#define BOT1_COLOR 'g'
#define BOT2_COLOR 'r'
#define BOT3_COLOR 'v'
#define BOT4_COLOR 'd'

#define BOUND_RECT 30

#define BOT_WIDTH 20


extern CvRect goal_rect;
extern CvPoint arena_center;
extern CvRect pitch;
extern char c;
extern CvCapture *capture;
extern IplImage *img;
extern IplImage *hsv;
extern CvSeq* contours, *finalcon;
extern int FrameCount;
