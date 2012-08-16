#pragma once
#include <queue>
#include <vector>
#include <cmath>
#include <cstdio>
#include <iostream>
class Action {
    public:
        int id;             // bot id
        int magnitude;     // angle or distance
        int direction;     // l, r, F, B 
        int speed; 
        int x, y; 

        Action(int, int, int, int, int, int); 
        void do_action(); 
};

class Coordinate {
    public:
        double x, y; 
        Coordinate() { }
        Coordinate(double x_, double y_) : x(x_), y(y_) { }
}; 

typedef std::queue<Action> action_queue; 
std::vector<Action> hold(int id, double x1, double y1, double angle, double x2, double y2); 
std::vector<Action> defend(int id, double x1, double y1, double angle, double x2, double y2); 
double get_angle_to_point(double x1, double y1, double x2, double y2, bool radian); 
double get_distance_to_point(double x1, double y1, double x2, double y2); 
double angle_from_bot_to_ball(int id); 
std::vector<Action> attack(int id); 
Coordinate getPathWithoutObstacle(int id, double x, double y); 
