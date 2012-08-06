
class our_bot{

    public:
        char color;
        IplImage **mask;
        CvRect location;
        CvPoint front_center, back_center, bot_center;
        double x,y,angle;
        bool idle; 
        boost::thread *thread_ptr; 

        our_bot(); 
        void FindCenter();
        double pos();
        double orientation();
        void update();
        ~our_bot();
};

extern our_bot bot[5];

