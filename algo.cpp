#include "algo.h" 
using namespace cv;
using namespace std;

queue<Action> bot_queue[5]; 
int still_count[5]; 

void interrupt_near_arena() {
    for (int id = 0; id < NUM_OF_OUR_BOTS; id ++) {
        double x = bot[id].x; 
        double y = bot[id].y; 
        double angle = bot[id].angle; 
        printf("Bot %d at (%f, %f) at an angle %f\n", id, x, y, angle); 
        // If it's colliding with the arena, stop the bot. 
        // FIXME: Use global variables. 

        if (    (x > 110 - 6 && angle >= 270 && angle <= 90)    ||
                (x < - 110 + 6 && angle >= 90 && angle <= 270)  ||
                (y > 90 - 6 && angle >= 0 && angle <= 180)      ||
                (y < - 90 + 6 && angle >= 180 && angle <= 360)  ||
                (y > 110 || y < -100)){

            printf("Interruping bot %d\n", id); 
#ifdef ELEC
            e_sendenccmd(id, 's'); 
#endif
        }

    }
}

void algo(int id) {

    double x = bot[id].x; 
    double y = bot[id].y; 
    double angle = bot[id].angle; 
    double bx = Ball.x; 
    double by = Ball.y; 
    vector<Action> res; 

    // Hold Stuff Starts Here {

   // // Try to go behind the ball from the goal. 
   // double gx = -100; 
   // double gy = 0; 
   // double theta = get_angle_to_point(bx, by, gx, gy); 
   // cout << "Theta in degrees: " << theta << endl; 
   // theta *= PI / 180; 

   // // Get the point on the line from goal to ball 5cm behind it
   // double goto_x = bx - 5 * cos(theta); 
   // double goto_y = by - 5 * sin(theta); 

   // // Check if it lies in the arena
   // if (goto_x < -90 || goto_x > 90 || goto_y < -90 || goto_y > 90) {
   //     goto_x = bx; 
   //     goto_y = by; 
   // }


   // printf("Will hold (%f, %f). Ball is at (%f, %f).\n", goto_x, goto_y, bx, by); 
   // vector<Action> res = hold(id, x, y, angle, goto_x, goto_y); 
   // for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
   //     bot_queue[id].push(*it); 
   // }

    // Hold stuff ends here }


    // Goalie and defender starts here {
    double theta1 = get_angle_to_point(bx, by, +110, -25, true); 
    double theta2 = get_angle_to_point(bx, by, +110, 0, true); 
    double theta3 = get_angle_to_point(bx, by, +110, +25, true); 
    if (id == 0) {
        // Goalie
        double goalie_x = +110 - 6; 
        double goalie_dx = goalie_x - bx; 
        double goalie_y = by + goalie_dx * ( tan(theta1) + tan(theta2) ) / 2; 
        if (goalie_y< 0)
            goalie_y = std::max(goalie_y, - 25.0); 
        else
            goalie_y = std::min(goalie_y, + 25.0); 
        res = defend(id, x, y, angle, goalie_x, goalie_y); 

    } else if (id == 1) {
        // Defender 1
        double defender_x = +110 - 15 - 6; 
        double defender_dx = defender_x - bx; 
        double defender_y = by + defender_dx * ( tan(theta3) + tan(theta2) ) / 2; 
        if (defender_y < 0)
            defender_y = std::max(defender_y, - 40.0); 
        else
            defender_y = std::min(defender_y, + 40.0); 

        res = defend(id, x, y, angle, defender_x, defender_y); 
    }


    // Goalie and defender end here }


    // Attacker stuff starts here {
    else if (id == 2) {
        // res = attack(id); 
    }
    // Attacker stuff ends here } 


    for (vector<Action>::iterator it = res.begin(); it != res.end(); it ++) {
        bot_queue[id].push(*it); 
    }

}
void main_algo() {

    // If any bot is too close to the arena, interrupt. 
    interrupt_near_arena(); 
    cout << "Doing algo stuff here\n"; 
    for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
        if (bot_queue[i].empty()) {
            if (still_count[i] <= 1) {
                still_count[i] ++; 
                continue; 
            }
            still_count[i] = 0; 
            algo(i); 
        }
    }

    for (int i = 0; i < NUM_OF_OUR_BOTS; i ++) {
        printf("Size of queue for %d is %d\n", i, bot_queue[i].size()); 
        if (bot_queue[i].empty())
            continue; 
        if (check_bot_free(i)) {
            Action curr = bot_queue[i].front(); 
            // Required for the "cool" hold
            if (curr.speed == 0) {
                cout << "Turn " << curr.magnitude << ' ' ; 
                curr.do_action(); 
                bot_status(); 
                cout << "Turn " << curr.magnitude << ' ' ; 
            } else if (curr.speed == -1) {
                double dist = get_distance_to_point(bot[i].x, bot[i].y, 
                        curr.x, curr.y); 
                cout << "Orient " << dist << ' ' ; 
                if ( dist < 2) {
                    curr.do_action(); 
                }
                bot_status(); 
                cout << "Orient " << dist << ' ' ; 
            } else {
                double diff = get_angle_to_point(bot[i].x, bot[i].y, 
                        curr.x, curr.y, false) - bot[i].angle; 
                cout << "Forward "  << fabs(diff) << ' ' ; 
                if (fabs(diff) < 15) {
                    curr.do_action();
                }
                bot_status(); 
                cout << "Forward "  << fabs(diff) << ' ' ; 
            }
            bot_queue[i].pop(); 
        }
    }
}
