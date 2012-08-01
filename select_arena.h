
#pragma once
#include <cv.h>
#include <highgui.h>

inline void draw( int x, int y );

void on_mouse( int event, int x, int y, int flags, void* param );

CvRect select_rect( CvCapture* cap );

