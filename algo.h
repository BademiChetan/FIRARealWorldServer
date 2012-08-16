#pragma once
#include <queue> 
#include <cv.h>
#include <highgui.h>
#include <ctime> 
#include <boost/thread.hpp>
#include "SerialStream.h"
#include "select_arena.h"
#include"process_image.h"
#include "elec.h"
#include "contours.h"
#include "colors.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "ball.h"
#include "update_frame.h"
#include "global_var.h"
#include "kick_off_calibration.h"
#include "bot_actions.h"
void main_algo(); 
void algo(int); 

class Line {
    double m, c; 
    Line(double a, double b) {
        m = a; 
        c = b; 
    }
};

class Circle {
    double x, y, r; 
    Circle(double a, double b, double c) {
        x = a; 
        y = b; 
        r = c; 
    }
};


