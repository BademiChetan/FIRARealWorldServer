#include "global_var.h"
#include "cv.h"
#include "bot_actions.h"
#include "our_bot.h"
#include "ball.h"
#include "update_frame.h"
#include "elec.h"
#include "algo.h"
#include <ctime> 
using namespace std; 
using namespace cv; 

Action::Action(int a, int b, int c, int d = 0, int e = 0, int f = 0) {
    id = a ; 
    magnitude = b; 
    direction = c; 
    speed = d; 
    x = e; 
    y = f; 
}


void keyboard_interrupt() {
    char ch; 
    while (true) {
        ch = getchar(); 
        if (ch == 'y')
            break; 
    }
}
// Absolute angle of ray going from (x1,y1) to (x2,y2)
double get_angle_to_point(double x1, double y1, double x2, double y2, bool
        radian = false) {
    double dy = y2 - y1; 
    double dx = x2 - x1;
    double res = 0; 
    if (fabs(dx) < EPSILON) {
        res = dy > 0 ? PI / 2 : - PI / 2; 
    } else if (fabs(dy) < EPSILON) {
        res = dx > 0 ? 0 : PI; 
    } else {
        res = atan(fabs(dy / dx)); 
        if (dx < 0 && dy < 0) 
            res += PI; 
        else if (dx < 0 && dy > 0)
            res = PI - res; 
        else if (dx > 0 && dy < 0)
            res = 2 * PI - res; 
    }
    if (res < 0)
        res += 2 * PI; 
    if (!radian) {
        res *= 180; 
        res /= PI; 
    }
    return res; 
}

double get_distance_to_point(double x1, double y1, double x2, double y2) {
    return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) ); 
}
double angle_from_bot_to_ball(int id) {
    double x = bot[id].x; 
    double y = bot[id].y; 
    double bx = Ball.x; 
    double by = Ball.y; 
    return get_angle_to_point(x, y, bx, by); 
}

double distance_from_bot_to_point(int id, double x, double y) {
    return get_distance_to_point(bot[id].x, bot[id].y, x, y); 
}
double distance_from_bot_to_ball(int id) {
    double x = bot[id].x; 
    double y = bot[id].y; 
    double bx = Ball.x; 
    double by = Ball.y; 
    return get_distance_to_point(x, y, bx, by); 
}
void Action::do_action() {
    printf("Command: %d, %d, %c, %d.\n", id, magnitude, (char)(direction), speed); 
    printf("Pos: (%f, %f)\n", bot[id].x, bot[id].y); 


#ifdef ELEC
    if (speed == 0) {
        cout << "Sending turn\n"; 
        e_sendenccmd(id, direction, magnitude); 
    } else {
        cout << "Sending forward\n"; 
        e_sendenccmd(id, direction, magnitude, speed); 
    }
#endif
    cout << "Done doing action. \n" ; 
}
// Currently at (x1, y1). Hold the ball which is at (x2, y2). 
vector<Action> hold(int id, double x1, double y1, double angle, double x2, double y2) {
    printf("HOLD: id = %d, (%f, %f) < %f, (%f, %f)\n", id, x1, y1, angle, x2, y2); 
    vector<Action> res; 
    double orient = get_angle_to_point(x1, y1, x2, y2); 
    assert(orient >= 0.0 && orient <= 360.0); 
    assert(angle >= 0.0 && angle <= 360.0); 
    int direction = 0; // l => 0 and r => 1
    direction = orient > angle ? 1 : 0; 
    int turn_by = fabs(orient - angle); 
    if (turn_by > 180) {
        turn_by = 360 - turn_by; 
        direction ^= 1; 
    }
    Action turn(id, turn_by, direction ? 'l' : 'r'); 
    res.push_back(turn); 
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); 
    Action run(id, min((int)(distance), MIN_DIST), 'F', MAX_SPEED, x2, y2); 
    res.push_back(run); 
    return res; 
}

// Currently at (x1, y1). Hold the ball which is at (x2, y2). 
vector<Action> defend(int id, double x1, double y1, double angle, double x2, double y2) {
    printf("DEFEND: id = %d, (%f, %f) < %f, (%f, %f)\n", id, x1, y1, angle, x2, y2); 
    vector<Action> res; 
    double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); 
    if (distance <= 1) {
        cout << "Already there!\n"; 
        return res; 
    }
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
    Action run(id, min((int)(distance), MIN_DIST), 'F', MAX_SPEED, x2, y2); 
    res.push_back(run); 
    // Orient ourselves back
    direction = 0; // l => 1 and r => 0
    direction = orient < 90 ? 1 : 0; 
    turn_by = fabs(orient - 90); 
    if (turn_by > 180) {
        turn_by = 360 - turn_by; 
        direction ^= 1; 
    }
    Action turn2(id, turn_by, direction ? 'l' : 'r', -1); 
    res.push_back(turn2); 
    return res; 
}

bool have_the_ball(int id){
    // have the ball with me
    if (distance_from_bot_to_ball(id) <= 8 &&
            angle_from_bot_to_ball(id) <= 30 )
        return true;
    return false; 
}

Coordinate getPathWithoutObstacle(int id, double x2, double y2) {
    return Coordinate(x2, y2); 
   // double x1 = bot[id].x; 
   // double y1 = bot[id].y; 
   // // find line to the ball
   // Line to_ball(x1, y1, x2, y2); 
   // Line above(BOT_RADIUS, to_ball); 
   // Line below(-BOT_RADIUS, to_ball); 

}

void reflect_from_arena(double &x, double &y) {
    // MAX_X - BOT_RADIUS and MAX_Y - BOT_RADIUS
    if (x < -104)
        x = -104; 
    if (x > 104)
        x = 104; 
    if (y < -84)
        y = -84; 
    if (y > 84)
        y = 84; 
}

std::vector<Action> attack(int id) {
    // Do I have the ball? 
    if (have_the_ball(id)) {
        Coordinate fp = getPathWithoutObstacle(id, 110, 0); 
        return hold(id, bot[id].x, bot[id].y, bot[id].angle, fp.x, fp.y); 
    }

    double x = bot[id].x; 
    double y = bot[id].y; 
    double angle = bot[id].angle; 
    double bx = Ball.x; 
    double by = Ball.y; 

    // Try to go behind the ball from the goal. 
    double gx = +110; 
    double gy = 0; 
    double theta = get_angle_to_point(bx, by, gx, gy); 
    theta *= PI / 180; 

    // Get the point on the line from goal to ball 5cm behind it
    double goto_x = bx - 8 * cos(theta); 
    double goto_y = by - 8 * sin(theta); 


    printf("Before reflecting (%f, %f)\n", goto_x, goto_y); 
    reflect_from_arena(goto_x, goto_y); 
    printf("After reflecting (%f, %f)\n", goto_x, goto_y); 

    // Fine line joining current position to the position behind the ball
    Line to_point(x, y, goto_x, goto_y); 
    Line below(-BOT_RADIUS, to_point); 
    Line above(+BOT_RADIUS, to_point); 

    if (above.distance_to_point(Ball.x, Ball.y) < BALL_RADIUS || 
        below.distance_to_point(Ball.x, Ball.y) < BALL_RADIUS || 
        to_point.distance_to_point(Ball.x, Ball.y) < BALL_RADIUS) {
        double a, b, c; 
        a = above.distance_to_point(Ball.x, Ball.y);
        b = below.distance_to_point(Ball.x, Ball.y);
        c = to_point.distance_to_point(Ball.x, Ball.y);

        cout << "Distances(a, b, c) " << a << ' ' << b << ' ' << c << endl; 

        cout << "OBSTACLE!\n"; 
        // Point P is the point of intersection of transverse tangents
        double px = BALL_RADIUS * x + BOT_RADIUS * Ball.x; 
        px /= ( BALL_RADIUS + BOT_RADIUS); 
        double py = BALL_RADIUS * y + BOT_RADIUS * Ball.y; 
        py /= ( BALL_RADIUS + BOT_RADIUS); 

        printf("(px, py) = (%f, %f)\n", px, py); 
        Line to_ball(x, y, Ball.x, Ball.y); 
        double angle = get_angle_to_point(x, y, Ball.x, Ball.y, true); 
        double angle_to_point = get_angle_to_point(x, y, goto_x, goto_y, true); 
        printf("Angle to the ball  = %f\n", angle * 180 / PI); 
        printf("Angle to point = %f\n", angle_to_point * 180 / PI); 
        double low = 0; 
        double high = 89 * PI / 180.0; 
        double mid; 
        while (high - low > 0.01) {
            mid = (high + low) / 2.0; 
            cout << "Mid = " << mid * 180 / PI << endl; 
            Line temp(px, py, tan(angle + mid)); 
            double dist = temp.distance_to_point(Ball.x, Ball.y); 
            cout << "Dist = " << dist << endl; 
            if (dist >= BALL_RADIUS) {
                high = mid; 
            } else {
                low = mid; 
            }
        }
        printf("Mid = %f\n", mid * 180 / PI); 
        Line final; 
        if (fabs(angle + mid - angle_to_point) < 
                fabs(angle - mid - angle_to_point)) {
            printf("Angle of tangent to X axis = %f\n", (angle + mid) * 180 / PI); 
            final = Line(x, y, tan(angle + mid)); 
        } else {
            printf("Angle of tangent to X axis = %f\n", (angle - mid) * 180 / PI); 
            final = Line(x, y, tan(angle - mid)); 
        }
        Line perpendicular(Ball.x, Ball.y, final, false); 
        Coordinate ans = final.intersection_point(perpendicular); 
        goto_x = ans.x; 
        goto_x = ans.y; 
    }
    printf("After obstacle stuff: (%f, %f)\n", goto_x, goto_y); 

    printf("Will hold (%f, %f). Ball is at (%f, %f).\n", goto_x, goto_y, bx, by); 
    keyboard_interrupt(); 
    return hold(id, x, y, angle, goto_x, goto_y); 
}
