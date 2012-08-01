#include <cv.h>

class opp_bot{
    public:
		static int total_no_of_objects;
		IplImage *mask;
        CvRect location;
        CvPoint center;
        char color;
        void update();
        void FindCenter(CvPoint &cen);
        opp_bot();
        int OBJECT_NUM;
};

extern opp_bot o_bot[5];

