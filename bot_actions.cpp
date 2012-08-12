#include "cv.h"
#include "bot_actions.h"
#include "our_bot.h"
#include "ball.h"
#include "update_frame.h"
#include "elec.h"
#include <ctime> 
#define EPSILON 1e-6
#define PI      3.141592653589793238
using namespace std; 
using namespace cv; 

Action::Action(int a, int b, int c, int d = 0) {
    id = a ; 
    magnitude = b; 
    direction = c; 
    speed = d; 
}


// Absolute angle of line going from (x1,y1) to (x2,y2)
double get_angle_to_point(double x1, double y1, double x2, double y2, bool
        radian = false) {
    double dy = y2 - y1; 
    double dx = x2 - x1;
    double res = 0; 
    if (fabs(dx) < EPSILON) {
        res = dy > 0 ? PI / 2 : - PI / 2; 
    } else {
        res = atan(fabs(dy / dx)); 
        if (dx < 0 && dy < 0) 
            res += PI; 
        else if (dx < 0 && dy > 0)
            res = PI - res; 
        else if (dx > 0 && dy < 0)
            res = 2 * PI - res; 
    }
    if (!radian) {
        res *= 180; 
        res /= PI; 
    }
    return res; 
}

double angle_from_bot_to_ball(int id) {
    double x = bot[id].x; 
    double y = bot[id].y; 
    double bx, by; 
    Ball.getCenter(bx, by); 
    return get_angle_to_point(x, y, bx, by); 
}

void Action::do_action() {
    printf("Command: %d, %d, %c, %d.\n", id, magnitude, (char)(direction), speed); 
    printf("Pos: (%f, %f)\n", bot[id].x, bot[id].y); 

   // char ch; 
   // while (true) {
   //     ch = getchar(); 
   //     if (ch == 'y')
   //         break; 
   // }

    if (speed == 0) {
        cout << "Sending turn\n"; 
        e_sendenccmd(id, direction, magnitude); 
    } else {
        cout << "Sending forward\n"; 
        e_sendenccmd(id, direction, magnitude, speed); 
    }
}
// Currently at (x1, y1). Hold the ball which is at (x2, y2). 
vector<Action> hold(int id, double x1, double y1, double angle, double x2, double y2) {
    printf("id = %d, (%f, %f) < %f, (%f, %f)\n", id, x1, y1, angle, x2, y2); 
    vector<Action> res; 
    double orient = get_angle_to_point(x1, y1, x2, y2); 
    int direction = 0; // l => 0 and r => 1
    direction = orient > angle ? 1 : 0; 
    int turn_by = fabs(orient - angle); if (turn_by > 180) {
        turn_by = 360 - turn_by; 
        direction ^= 1; 
    }
    Action turn(id, turn_by, direction ? 'l' : 'r'); 
    res.push_back(turn); 
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); 
    Action run(id, min((int)(distance), 40), 'F', 80); 
    res.push_back(run); 
    return res; 
}

// Currently at (x1, y1). Hold the ball which is at (x2, y2). 
vector<Action> defend(int id, double x1, double y1, double angle, double x2, double y2) {
    printf("id = %d, (%f, %f) < %f, (%f, %f)\n", id, x1, y1, angle, x2, y2); 
    vector<Action> res; 
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); 
    if (distance <= 3)
        return res; 
    printf("id = %d, (%f, %f) < %f, (%f, %f)\n", id, x1, y1, angle, x2, y2); 
    // Initial turn towards the point
    double orient = get_angle_to_point(x1, y1, x2, y2); 
    int direction = 0; // l => 1 and r => 0
    direction = orient > angle ? 1 : 0; 
    int turn_by = fabs(orient - angle); 
    if (turn_by > 180) {
        turn_by = 360 - turn_by; 
        direction ^= 1; 
    }
    Action turn1(id, turn_by, direction ? 'l' : 'r'); 
    res.push_back(turn1); 
    // Move towards the point
    Action run(id, min((int)(distance), 40), 'F', 80); 
    res.push_back(run); 
    // Orient ourselves back
    direction = 0; // l => 1 and r => 0
    direction = orient < 90 ? 1 : 0; 
    turn_by = fabs(orient - 90); 
    if (turn_by > 180) {
        turn_by = 360 - turn_by; 
        direction ^= 1; 
    }
    Action turn2(id, turn_by, direction ? 'l' : 'r'); 
    res.push_back(turn2); 
    return res; 
}
