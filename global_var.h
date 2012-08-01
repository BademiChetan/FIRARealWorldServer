#pragma once

#define THRESH_AREA 50

#define OUR_JERSEY_COLOR 'b'

#define OPP_JERSEY_COLOR 'y'

#define NUM_OF_OUR_BOTS 5

#define NUM_OF_OPP_BOTS 5

#define BOT1_COLOR 'r'
#define BOT2_COLOR 'g'
#define BOT3_COLOR 'd'
#define BOT4_COLOR 'p'
#define BOT5_COLOR 'v'

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
