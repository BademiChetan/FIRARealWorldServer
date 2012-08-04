#pragma once
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;

#include "process_image.h"
#include "contours.h"
#include "colors.h"
#include "global_var.h"
#include "our_bot.h"

// calibration conditions - if ( ( Hue range ) && ( Saturation range ) && ( Value range ) )

inline bool check_orange( uchar *ptr, int x ){
    if( ( ptr[3*x] >= 7 && ptr[3*x] < 25 ) && ( ptr[3*x+1] >= 240 && ptr[3*x+1] <= 255 ) && ( ptr[3*x+2] > 180 && ptr[3*x+2] <= 255 ) )
        return true;
    else
        return false;
}


inline bool check_yellow( uchar *ptr, int x ){
    if( (ptr[3*x] > 20 && ptr[3*x] < 35 ) && ( ptr[3*x+1] > 220 && ptr[3*x+1] <= 255 ) && ( ptr[3*x+2] > 190 && ptr[3*x+2] < 230 ) )
        return true;
    else
        return false;
}


inline bool check_blue( uchar *ptr, int x ){	
    if( ( ptr[3*x] > 90 && ptr[3*x] < 110 ) && ( ptr[3*x+1] > 170 && ptr[3*x+1] <= 220 ) && ( ptr[3*x+2] > 140 && ptr[3*x+2] <= 170 ) )
        return true;
    else
        return false;
}

inline bool check_red( uchar *ptr, int x ){
    if( ( ptr[3*x] >= 0 && ptr[3*x] <= 14 ) && ( ptr[3*x+1] > 180 && ptr[3*x+1] <= 230 ) && ( ptr[3*x+2] > 140 && ptr[3*x+2] <= 210 ) )
        return true;
    else
        return false;
}

inline bool check_lgreen( uchar *ptr, int x ){
    if( ( ptr[3*x] > 35 && ptr[3*x] < 80 ) && ( ptr[3*x+1] > 135 && ptr[3*x+1] <= 225 ) && ( ptr[3*x+2] > 100 && ptr[3*x+2] <= 240 ) )
        return true;
    else
        return false;
}
inline bool check_violet( uchar *ptr, int x ){
    if( ( ptr[3*x] > 125 && ptr[3*x] < 150 ) && ( ptr[3*x+1] > 10 && ptr[3*x+1] <= 80 ) && ( ptr[3*x+2] > 120 && ptr[3*x+2] <= 160 ) )
        return true;
    else
        return false;
}

inline bool check_pink( uchar *ptr, int x ){
    if( ( ptr[3*x] > 160 && ptr[3*x] < 180 ) && ( ptr[3*x+1] > 150 && ptr[3*x+1] < 220 ) && ( ptr[3*x+2] > 130 && ptr[3*x+2] < 200 ) )
        return true;
    else
        return false;
}

inline bool check_dgreen( uchar *ptr, int x ){
    if( ( ptr[3*x] > 40 && ptr[3*x] < 60 ) && ( ptr[3*x+1] > 225 && ptr[3*x+1] <= 255 ) && ( ptr[3*x+2] > 200 && ptr[3*x+2] < 220 ) )
        return true;
    else
        return false;
}

inline bool check_jersey( uchar *ptr, int x ){
    if( OUR_JERSEY_COLOR == 'y' )
        return check_yellow( ptr, x );
    else
        return check_blue( ptr, x );
}

inline bool check_opp_jersey( uchar *ptr, int x ){
    if( OPP_JERSEY_COLOR == 'y' )
        return check_yellow( ptr, x );
    else
        return check_blue( ptr, x );
}


// Picks the color given by <color> and stores it as white pixels in the mask

void pick_color( IplImage **mask, CvRect location, char color ){
    int y,x;

    switch( color ){

        case 'r' :
            cvZero( mask[0] );
            cvZero( mask[1] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrf = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//front color
                uchar *ptrb = ( uchar* )( mask[1]->imageData + y * mask[1]->widthStep );	//back color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_red( ptr, x ) )
                        ptrf[x] = 255;

                    if( check_jersey ( ptr, x ) )
                        ptrb[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            threshold( mask[1], mask[1] );
            break;

        case 'g' :
            cvZero( mask[0] );
            cvZero( mask[1] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrf = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//front color
                uchar *ptrb = ( uchar* )( mask[1]->imageData + y * mask[1]->widthStep );	//back color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_lgreen( ptr, x ) )
                        ptrf[x] = 255;

                    if( check_jersey ( ptr, x ) )
                        ptrb[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            threshold( mask[1], mask[1] );
            break;

        case 'd' :
            cvZero( mask[0] );
            cvZero( mask[1] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrf = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//front color
                uchar *ptrb = ( uchar* )( mask[1]->imageData + y * mask[1]->widthStep );	//back color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_dgreen( ptr, x ) )
                        ptrf[x] = 255;

                    if( check_jersey ( ptr, x ) )
                        ptrb[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            threshold( mask[1], mask[1] );
            break;

        case 'p' :
            cvZero( mask[0] );
            cvZero( mask[1] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrf = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//front color
                uchar *ptrb = ( uchar* )( mask[1]->imageData + y * mask[1]->widthStep );	//back color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_pink( ptr, x ) )
                        ptrf[x] = 255;

                    if( check_jersey ( ptr, x ) )
                        ptrb[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            threshold( mask[1], mask[1] );
            break;

        case 'v' :
            cvZero( mask[0] );
            cvZero( mask[1] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrf = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//front color
                uchar *ptrb = ( uchar* )( mask[1]->imageData + y * mask[1]->widthStep );	//back color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_violet( ptr, x ) )
                        ptrf[x] = 255;

                    if( check_jersey ( ptr, x ) )
                        ptrb[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            threshold( mask[1], mask[1] );
            break;

        case 'o' :
            cvZero( mask[0] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrm = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );	//ball color

                for( x = location.x; x<location.x + location.width; x++ ){
                    if( check_orange( ptr, x ) )
                        ptrm[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            break;

        case OPP_JERSEY_COLOR :
            cvZero( mask[0] );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrm = ( uchar* )( mask[0]->imageData + y * mask[0]->widthStep );

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_opp_jersey( ptr, x ) )
                        ptrm[x] = 255;
                }
            }
            threshold( mask[0], mask[0] );
            break;
    }

    //cvShowImage( "back_center", mask[0] );
    //cvShowImage( "front_center", mask[1] );
}



