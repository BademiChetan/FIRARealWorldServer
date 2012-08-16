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
    public:
        double a, b, c;
        // Line passing through two points
        Line(double x1, double y1, double x2, double y2) {
            a = y2 - y1; 
            b = x1 - x2; 
            c = a * x1 - b * y1; 
        }
        // Line passing through a point and parallel to a line
        Line(double x1, double y1, Line &line) {
            a = line.a; 
            b = line.b; 
            c = - a * x1 - b * y1; 
        }
        // Line parallel to a given line, with dist above and below
        Line(double dist, Line &line) {
            a = line.a; 
            b = line.b; 
            c = line.c + dist * sqrt(line.a * line.a + line.b * line.b); 
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

//double distance_to_line(double x, double y, Line &line); 

