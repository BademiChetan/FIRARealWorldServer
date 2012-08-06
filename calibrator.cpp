#include <cv.h>
#include <highgui.h>
#include<iostream>
// Define our callback which we will install for
// mouse events.
//  ./calibrator c images.jpg

using namespace std;
int color=1;//change for printing color

void my_mouse_callback(
int event, int x, int y, int flags, void* param
);
CvRect box;
int drawing_box=0;

//bool drawing_box = false;
// A litte subroutine to draw a box onto an image

void draw_box( IplImage* img, CvRect rect ) {
cvRectangle (
img,
cvPoint(box.x,box.y),
cvPoint(box.x+box.width,box.y+box.height),
cvScalar(0xff,0x00,0x00,0),1,8,0
/* red */
);
}
int main( int argc, char* argv[] ) {
box = cvRect(-1,-1,0,0);
//int drawing_box=1;

IplImage* image; //=cvLoadImage(argc>1?argv[1]:"photo-A.jpg",1);
CvCapture *capture=cvCreateCameraCapture(1);
//CvCapture *capture= cvCreateFileCapture(argc>1?argv[1]:"ball speed.mpeg");
image=cvQueryFrame(capture);
char c=0;
for(;c!='s';)
{
c=cvWaitKey(20);
image=cvQueryFrame(capture);
cvShowImage( "Calibrator", image);
}
/*= cvCreateImage(
cvSize(600,600),
IPL_DEPTH_8U,
3
);*/
//cvZero( image );
IplImage* temp = cvCloneImage( image );
cvNamedWindow( "Calibrator",CV_WINDOW_AUTOSIZE );
// Here is the crucial moment that we actually install
// the callback. Note that we set the value ‘param’ to
// be the image we are working with so that the callback
// will have the image to edit.
//

cvSetMouseCallback(
"Calibrator",
my_mouse_callback,
(void*) image
);
// The main program loop. Here we copy the working image
// to the ‘temp’ image, and if the user is drawing, then
// put the currently contemplated box onto that temp image.
// display the temp image, and wait 15ms for a keystroke,
// then repeat...
//
while( 1 ) {
cvCopyImage( image, temp );
if( drawing_box ) draw_box( temp, box );
cvShowImage( "Calibrator", temp );
if( cvWaitKey( 15 )==27 ) break;
}
// Be tidy
//
cvReleaseImage( &image );

cvReleaseImage( &temp );
cvDestroyWindow( "Calibrator" );
}
// This is our mouse callback. If the user
// presses the left button, we start a box.
// when the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//
void my_mouse_callback(
int event, int x, int y, int flags, void* param
) {

int m,n;
//int drawing_box=1;
IplImage* image = (IplImage*) param;
switch( event ) {
case CV_EVENT_MOUSEMOVE: {
if( drawing_box ) {
box.width = x-box.x;
box.height = y-box.y;
}
}
break;
case CV_EVENT_LBUTTONDOWN: {
drawing_box = 1;
box = cvRect(x, y, 0, 0);
}
break;
case CV_EVENT_LBUTTONUP: {
drawing_box = 0;
if(box.width<0) {
box.x+=box.width;
box.width *=-1;
}
if(box.height<0) {
box.y+=box.height;
box.height*=-1;
}
draw_box(image, box);
cout<<"\n\t("<<' '<<box.x<<' '<<box.y<<' '<<box.width<<' '<<box.height<<") "<<endl;
cout<<endl;
if(color==1)
{
IplImage *hsvimg=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
cvCvtColor(image,hsvimg,CV_BGR2HSV);
for(n=box.y;n<box.y+box.height;n++)
	{
	uchar *ptr=(uchar*)(hsvimg->imageData+n*hsvimg->widthStep);
	for(m=box.x;m<box.x+box.width;m++)
		{
		printf("(%d,%d,%d)\t",ptr[3*m],ptr[3*m+1],ptr[3*m+2]);		
		}
	}
}
cout<<endl;
}
break;
}
}

