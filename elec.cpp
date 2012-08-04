// Bazooka is awesome!
#pragma once
#include <SerialStream.h>

using namespace LibSerial;

SerialStream send;
	
void move_forward( int bot_index, int distance ){
    send << ":";
    send << bot_index;
    
        send << 'f';
        
    for ( int i = 100 ; i >=1 ; i /= 10 ){
        send << (char)('0' + (distance / i ) % 10 ) ; 
        send << 's' ;
    }
    usleep(100000);
}


void turn( int bot_index, int angle ){

    bool right = true;

    if ( angle < 0 ){
        right = false; 
        angle *= -1 ; 
    }


    if ( angle > 180 ){
        right ^= 1 ; 
        angle = 360 - angle; 
    }

    printf("I will move by %d degrees in %s direction. \n" , angle, right ? "right" : "left" ) ; 

    for ( int i = 0 ; i < angle ; i ++ ){
        if ( right ){
            send << ":" << bot_index << "r001";
        }else{
            send << ":" << bot_index <<"l001";
        }
    }
    send << "s" ; 
}


