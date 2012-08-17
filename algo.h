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
double average_velocity(int); 

class Line {
    // ax + by + c = 0
    public:
        double a, b, c;
        // Default constructor
        Line() { }
        // Line passing through two points
        Line(double x1, double y1, double x2, double y2) {
            a = y2 - y1; 
            b = x1 - x2; 
            c = - a * x1 - b * y1; 
        }
        // Line passing through a point and parallel/perpendicular to a line
        Line(double x, double y, Line &line, bool parallel = true) {
            if (parallel) {
                a = line.a; 
                b = line.b; 
            } else {
                a = line.b; 
                b = - line.a; 
            }
            c = - a * x - b * y; 
        }
        // Line parallel to a given line, with dist above and below
        Line(double dist, Line &line) {
            a = line.a; 
            b = line.b; 
            c = line.c + dist * sqrt(line.a * line.a + line.b * line.b); 
        }
        // Line passing through a point and some slope
        Line(double x, double y, double slope) {
            b = -1 ; 
            a = slope; 
            c = y - slope * x; 
            printf("Slope: input => (%f, %f, %f) output => (%f %f %f)\n", x, y, slope, a, b, c); 
        }

        // Perpendicular distance of point from line
        double distance_to_point(double x, double y) {
            return fabs(a * x + b * y + c) / sqrt(a * a + b * b); 
        }

        // Intersection of current line and given line
        Coordinate intersection_point(Line &line) {
            printf("My line: %f %f %f\n", a, b, c); 
            printf("Other line: %f %f %f\n", line.a, line.b, line.c); 
            Coordinate res; 
            res.x = (line.c * a - c * line.a) / (b * line.a - a * line.b); 
            res.y = (line.c * b - c * line.b) / (a * line.b - b * line.a); 
            return res; 
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
