#include <vector>

using namespace std;

CvPoint ClosestFrontCenter( IplImage* frame, CvPoint closest_to, int & AREA );

vector <CvPoint> FindAllCenter( IplImage* frame, vector <int> & AREA );

CvPoint LargestContourCenter( IplImage *frame );

