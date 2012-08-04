#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include"contours.h"
#include"global_var.h"
#include"process_image.h"

using namespace cv;
using namespace std;

double conarea=0;
char c=0;

//CvCapture *capture= cvCreateCameraCapture(1);

CvSeq* contours = 0, *BallContour = 0, *BackContour = 0, *FrontContour = 0, *finalcon = 0;


CvMemStorage* storage =  cvCreateMemStorage(0);
CvMemStorage* Ballstr =  cvCreateMemStorage(0);
CvMemStorage* finalcon_str =  cvCreateMemStorage(0);
CvMemStorage* str = cvCreateMemStorage(0);
CvMemStorage* boxfromseq_str = cvCreateMemStorage(0);
CvMemStorage* ballbox_str = cvCreateMemStorage(0);

CvPoint ClosestFrontCenter( IplImage* frame, CvPoint closest_to ){
    int a = 0, b = 0;
    int j = 0;
    int min_dis = 10000;
    int dis = 0;
    CvPoint center = cvPoint( 0, 0 ), final_cen = cvPoint( 0, 0 );
    CvBox2D contour_box;

    //Finds all the external contours in the mask given by <frame>
    cvFindContours( frame, storage, &contours, sizeof( CvContour ), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );
    //cvDrawContours( img, contours, cvScalar( 255, 255, 255, 0 ), cvScalar( 255, 255, 255, 0 ) , 1, 5, 8, cvPoint( 0, 0 ) );

    for( CvSeq* con = contours; con != NULL; con = con->h_next ){
        if( cvContourArea( con, CV_WHOLE_SEQ ) > THRESH_AREA ){
            int b = con->total - 1;

            FrontContour = cvSeqSlice( con, cvSlice( 0, b ), str, 0 );

            if( FrontContour ){
                contour_box = cvMinAreaRect2( FrontContour, boxfromseq_str );
                center = cvPoint( ( int ) contour_box.center.x, ( int ) contour_box.center.y );
                dis = distanc( center, closest_to );
                if( dis < min_dis && dis <= BOT_WIDTH ){
                    min_dis = dis;
                    final_cen = center;
                }
            }
        }
    }

    return final_cen;
}


vector <CvPoint> FindAllCenter( IplImage* frame ){

    int a = 0, b = 0;
    int j = 0;
    int area = 0;
    vector <CvPoint> center;
    CvBox2D contour_box;

    cvFindContours( frame, finalcon_str, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    for( CvSeq* con = contours; con != NULL; con = con->h_next ){
        area = cvContourArea( con, CV_WHOLE_SEQ );

        if( area > THRESH_AREA ){
            int b = con->total - 1;
            BackContour = cvSeqSlice( con, cvSlice( 0, b ), str, 0 );

            //cvDrawContours( img, BackContour, cvScalar( 255, 255, 0, 0 ), cvScalar( 255, 255, 255, 0 ), 1, 5, 8, cvPoint( 0, 0 ) );

            if( BackContour ){
                contour_box = cvMinAreaRect2( BackContour, boxfromseq_str );
                center.push_back( cvPoint( ( int ) contour_box.center.x, ( int ) contour_box.center.y ) );
            }
        }
    }

    return center;

}

CvPoint LargestContourCenter( IplImage* frame ){
    CvSeq *ballcontours = 0, *ballfinalcon = 0;
    conarea = 0;
    int area = 0;
    CvPoint center = cvPoint( 0, 0);

    cvFindContours( frame, storage, &ballcontours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    for( CvSeq* con = ballcontours; con != NULL; con = con->h_next ){
        area = cvContourArea( con, CV_WHOLE_SEQ );

        if( area > THRESH_AREA && area > conarea ){
            conarea = area;

            int b = con->total - 1;

            ballfinalcon = cvSeqSlice( con, cvSlice( 0, b ), str, 0 );
            //cvDrawContours( img, finalcon, cvScalar( 255, 255, 0, 0 ), cvScalar( 255, 255, 255, 0 ), 1, 5, 8, cvPoint( 0, 0 ) );

        }
    }
    CvBox2D contour_box;
    if( ballfinalcon ){
        contour_box = cvMinAreaRect2( ballfinalcon, boxfromseq_str );
        cvClearSeq( ballfinalcon );
        center=cvPoint( ( int ) contour_box.center.x, ( int ) contour_box.center.y );
    }
    else
        center = cvPoint( 0, 0 );

    if( contours )
        cvClearSeq( contours );

    return center;
}


