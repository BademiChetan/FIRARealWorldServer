#pragma once
#include<cv.h>
#include<highgui.h>
#include<math.h>


void threshold( IplImage* src, IplImage* dst ){
    cvDilate( src, dst, NULL, 1 );
    cvSmooth( dst, dst, CV_MEDIAN, 3, 0, 0, 0 );
}

inline double distanc( CvPoint pt1, CvPoint pt2 ){
    return ( sqrt( ( pt1.x - pt2.x ) * ( pt1.x - pt2.x ) + ( pt1.y - pt2.y ) * ( pt1.y - pt2.y ) ) );
}


int angl( CvPoint dst, CvPoint cen, CvPoint front ){

    float a = ( float )( ( dst.x - cen.x ) * ( front.y - cen.y ) - ( front.x - cen.x ) * ( dst.y - cen.y ) );	//angle through cross product.
    float b = ( float )( distanc( cen, dst ) * distanc( cen, front ) );
    float result = 0;
    float angle = 1000;

    if( b != 0 ){
        result = asin( a / b );

        if(distanc( cen, dst ) > distanc( front, dst ) ){
            angle = ( ( result * 90 / 1.57 ) );
        }
        else if( result > 0 ){
            angle= ( ( 180 - result * 90 / 1.57 ) );
        }
        else if( result < 0 ){
            angle = ( -180 - result * 90 / 1.57 );
        }
    }
    if( angle == 1000 ){
        if( front.y > cen.y )
            angle = 180;
        else if( cen.y > front.y )
            angle = 0;
        else
            printf( " < " );
    }
    if (angle == 1000)
        angle = -1; 
    return angle;
}
