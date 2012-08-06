
#pragma once
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

CvRect rect;
IplImage *imagen, *screenBuffer;

int x, y, last_x, last_y, drawing;


inline void draw( int x, int y ){
    cvCopy( imagen, screenBuffer );	
    cvRectangle( screenBuffer, cvPoint(last_x,last_y), cvPoint( x, y ), CV_RGB( 255, 0, 0 ), 1 );
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
    //Select mouse Event
    if( event == CV_EVENT_LBUTTONDOWN ){
        last_x = x;
        last_y = y;
        rect.width = x-last_x;
        rect.height = y-last_y;
        rect.x = last_x;
        rect.y = last_y;
        drawing = 1;
    }
    else if( event == CV_EVENT_LBUTTONUP ){
        if( drawing ){
            drawing = 0;
            rect.width = x - last_x;
            rect.height = y - last_y;
        }
    }
    else if( event == CV_EVENT_MOUSEMOVE  &&  flags & CV_EVENT_FLAG_LBUTTON ){
        if( drawing )
            draw( x, y );
    }
}


CvFont Font = cvFont( 1, 1 );

Rect select_rect( CvCapture* cap )
{	
    char c;
    do{
        imagen = cvQueryFrame( cap );
        screenBuffer = cvCloneImage( imagen );
        cvNamedWindow("SAHAS",1);
        while( 1 ){
            cvSetMouseCallback( "SAHAS", on_mouse, NULL);
            cvPutText( screenBuffer, "Select the goal post ends:", cvPoint( 100, 50 ), &Font, cvScalar( 255, 255, 255, 0 ) );
            cvPutText( screenBuffer, "Refresh frame - r", cvPoint( 200, 80 ), &Font, cvScalar( 255, 255, 255, 0 ) );
            cvPutText( screenBuffer, "Done selection - s", cvPoint( 200, 100 ), &Font, cvScalar( 255, 255, 255, 0 ) );
            cvPutText( screenBuffer, "SAHAS - IITM", cvPoint( 260, 30 ), &Font, cvScalar( 0, 0, 255, 0 ) );
            cvShowImage( "SAHAS", screenBuffer );
            c = cvWaitKey( 10 );
            if( c == 27 )
                break;
            else if( c =='r' )
                break;
            else if( c=='s' ){
                return rect;
            }
        }
    }while( c == 'r' );
    cvReleaseImage( &imagen );
    cvReleaseImage( &screenBuffer );
}
