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


