#include "iostream"
#include "stdio.h"
#include "SerialStream.h"   
#include "SerialPort.h"
#include "stdlib.h"
#include "string"
#include "cstring"
#include "math.h"
#include "sys/time.h"

#define WT_BOT      1
#define WT_OBOT     2
#define WT_GOTOXY   -100
#define WT_WALL     1
#define DIST        10
#define ZERO        1e-6

float dist[5][2];
void gotoxyov(int botID, int x, int y, int o, char v);

/*************************************************
 * FUNCTION NAME: gotoxyov() 
 * ARGUMENTS    : int botID     => Bot ID
 *                int x_final   => Final x coordinate
 *                int y_final   => Final y coordinate
 *                int o         => Final orientation
 *                char v        => Velocity preferred
 * DESCRIPTION  : Simulate columb code and get the bot reach the desired location without hitting the walls and the bots (ours and 
 *                there)
 * RETURN VALUES: 
 ************************************************/
void getdistance()
{
    for(int i=0;i<5;++i)
    {
        if(i!=botID)
        {
            d[i][0]=pow((bot[botID].x-bot[i].x),2)+pow((bot[botID].y-bot[i].y),2);
            d[i][1]=pow((bot[botID].x-o_bot[i].x),2)+pow((bot[botID].y-o_bot[i].y),2);
        }
    }
    d[botID][1]=pow((o_bot[botID].x-bot[botID].x),2)+pow((bot[botID].y-o_bot[botID].y),2);
    d[botID][0]=pow((bot[botID].x-x_final),2)+pow((bot[botID].y-y_final),2);
}

/*************************************************
 * FUNCTION NAME: gotoxyov() 
 * ARGUMENTS    : int botID     => Bot ID
 *                int x_final   => Final x coordinate
 *                int y_final   => Final y coordinate
 *                int o         => Final orientation
 *                char v        => Velocity preferred
 * DESCRIPTION  : Simulate columb code and get the bot reach the desired location without hitting the walls and the bots (ours and 
 *                there)
 * RETURN VALUES: 
 ************************************************/
void gotoxyov(int botID, int x_final, int y_final, int o, char v)
{
    float icap=0,jcap=0;
    char angle=0;
    getdistance();
    for(int i=0;i<5;++i)
    {
        if(i!=botID)                                                    // All bots taken care of except the botID walas
        {
            icap+=WT_BOT*(bot[botID].x-bot[i].x)/d[i][0]+WT_OBOT*(bot[botID].x-o_bot[i].x)/d[i][1];
            jcap+=WT_BOT*(bot[botID].x-bot[i].y)/d[i][0]+WT_OBOT*(bot[botID].y-o_bot[i].y)/d[i][1]; 
        }
    }
    icap+=WT_OBOT*(bot[botID].x-o_bot[botID].x)/d[i][botID];
    jcap+=WT_OBOT*(bot[botID].y-o_bot[botID].y)/d[i][botID];                       // Opponent bot with the same ID
    icap+=WT_GOTOXY*(bot[botID].x-x_final)/d[botID][0];
    jcap+=WT_GOTOXY*(bot[botID].y-y_final)/d[botID][0];                            // Opponent bot with the same ID
    if(fabs(icap)<ZERO)
        ang=atan(jcap/icap)-bot[botID].angle;
    else 
        ang = (jcap > 0 ? 90 :270) - bot[botID].angle;
    e_sendenccmd(botID,'l',ang);
    wait_for_bot(botID);
    e_sendenccmd(botID,'F',DIST,v);
}
