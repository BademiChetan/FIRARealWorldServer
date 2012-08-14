
class ball
{
    public:
        IplImage *mask;
        CvRect location;
        CvPoint center;
        char color;
        double x,y;
        void update();
        void FindCenter( CvPoint &cen );
        void getCenter(double &x, double &y); 
        ball();
};


extern ball Ball;
