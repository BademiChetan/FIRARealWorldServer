#pragma once
#include <boost/thread.hpp> 
class our_bot{

    public:
        char color;
        IplImage **mask;
        CvRect location;
        CvPoint front_center, back_center, bot_center;
        double x,y,angle;

        our_bot(); 
        void FindCenter();
        double pos();
        double orientation();
        void update();
        ~our_bot();
};

extern our_bot bot[5];

