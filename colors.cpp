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

//TT Ball orange
/*
inline bool check_orange(uchar *ptr,int x)
{
	if((ptr[3*x]>15&&ptr[3*x]<25)&&ptr[3*x+1]>220&&ptr[3*x+1]<=255&&ptr[3*x+2]>170&&ptr[3*x+2]<245)
		return true;
	else
		return false;
}
*/

//Golf ball orange (very close to Red)
inline bool check_orange(uchar *ptr,int x)
{
	if((ptr[3*x]>0&&ptr[3*x]<8)&&ptr[3*x+1]>100&&ptr[3*x+1]<=240&&ptr[3*x+2]>80&&ptr[3*x+2]<225)
		return true;
	else
		return false;
}


inline bool check_yellow(uchar *ptr,int x)
{
    if((ptr[3*x]>10&&ptr[3*x]<35)&&(ptr[3*x+1]>150&&ptr[3*x+1]<=255)&&(ptr[3*x+2]>150&&ptr[3*x+2]<255))
        return true;
    else
        return false;
}


inline bool check_blue(uchar *ptr,int x)
{	
    if((ptr[3*x]>90&&ptr[3*x]<120)&&(ptr[3*x+1]>20&&ptr[3*x+1]<=230)&&(ptr[3*x+2]>20&&ptr[3*x+2]<230))
        return true;
    else
        return false;
}


inline bool check_green(uchar *ptr,int x)
{
    if((ptr[3*x]>40&&ptr[3*x]<70)&&(ptr[3*x+1]>20&&ptr[3*x+1]<=230)&&(ptr[3*x+2]>20&&ptr[3*x+2]<=230))
        return true;
    else
        return false;
}

inline bool check_violet(uchar *ptr,int x)
{
    if((ptr[3*x]>140&&ptr[3*x]<180)&&(ptr[3*x+1]>20&&ptr[3*x+1]<230)&&(ptr[3*x+2]>20&&ptr[3*x+2]<=230))
        return true;
    else
        return false;
}

inline bool check_jersey(uchar *ptr,int x){
	if(OUR_JERSEY_COLOR == 'y')
		return check_yellow(ptr, x);
	else
		return check_blue(ptr, x);
}

inline bool check_opp_jersey(uchar *ptr,int x){
	if(OPP_JERSEY_COLOR == 'y')
		return check_yellow(ptr, x);
	else
		return check_blue(ptr, x);
}


// Picks the color given by <color> and stores it as white pixels in the mask

void pick_basecolor( IplImage * newmask, CvRect location, char color ){
    int y,x;

    switch( color ){


        case 'g' :
			cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_green( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;

       

        case 'v' :
            cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_violet( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;
	
	case OUR_JERSEY_COLOR :

	cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_jersey( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;


        case 'o' :
            cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_orange( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;

        case OPP_JERSEY_COLOR :
            cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_opp_jersey( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;
    }

}


void pick_frontcolor( IplImage * newmask, CvRect location, char color ){
    int y,x;

    switch( color ){


        case 'g' :
			cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	//base color

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_green( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;

        case 'v' :
            cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_violet( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;
	
	case OUR_JERSEY_COLOR :

	cvZero( newmask );

            for( y = location.y; y < location.y + location.height; y++ ){
                uchar *ptr = ( uchar* )( hsv->imageData + y * hsv->widthStep );
                uchar *ptrb = ( uchar* )( newmask->imageData + y * newmask->widthStep );	

                for( x = location.x; x < location.x + location.width; x++ ){
                    if( check_jersey( ptr, x ) )
                        ptrb[x] = 255;			
                }
            }
            threshold( newmask, newmask );
            break;
    }






    //cvShowImage( "back_center", mask[0] );
    //cvShowImage( "front_center", mask[1] );
}



