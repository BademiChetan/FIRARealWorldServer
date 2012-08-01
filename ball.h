
class ball
{
    public:
		IplImage *mask;
        CvRect location;
        CvPoint center;
        char color;
    
        void update();
        void FindCenter( CvPoint &cen );
        ball();
};


extern ball Ball;
