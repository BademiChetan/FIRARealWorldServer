#include <vector>

using namespace std;

CvPoint ClosestFrontCenter( IplImage* frame, CvPoint closest_to );

vector <CvPoint> FindAllCenter( IplImage* frame );

CvPoint LargestContourCenter( IplImage *frame );

