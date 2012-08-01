#pragma once
#include <cv.h>
#include <highgui.h>
#include <math.h>


void threshold( IplImage* src, IplImage* dst );

inline double distanc( CvPoint pt1, CvPoint pt2 );

int angl( CvPoint dst, CvPoint cen, CvPoint front );


