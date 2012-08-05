#pragma once
#define FINAL                                                   // Remove the IP based commands by x corrupting it
/**************Left to do************************
 * 1. Remove the TXChar(' ',1); and quicken the process
 * 2. Find the time spent in the spat business
 * 3. Convert timer Dtimer and SHOW_TIMEout into ifdef
 * 7. Clean buffer
 *              KNOWN ISSUES
 * 1. Lacks backward compatibility as now the is_enc_cmd in AP returns 5 not 1
 * *********************************************/
#define PLAYERS 1

#define STR_DEBUG 1
#define TIMER 0
#define DTIMER 0
#define SHOW_TIMEOUT 1
#define xSEC                                                    // Remove 'x' Corruption for mirror based communication

int ENSURE_TIMEOUT=5;                                           // No. of tries to ensure
bool enforce_cmd=1;                                             // Enforces the command in the event of ack fail
bool ensure_cmd=1;                                              // In cases of error, the bot is made free
bool auto_correct=1;                                            // Auto correction,i.e, in the event of error. The bot is 
                                                                // interrupted and the command is forced.
#define max_enc_value   	255                                     // Encoder maximum value
#define NO_TIMEOUT      	1                                       // The amount of tries to read from AP before giving up
#define TIMEOUT_VAL     	10000                                   // Timeout value in ms
#define TIMEOUT_READ    	5000                                    // Timeout in read
#define SLEEP_TIME      	1000
#define ANGLE_TOL       	10
#define X_TOL           	2
#define Y_TOL           	2
#define NO_TIMEOUT_4_WAIT	30
//bool update_table=1;                                          // Updates the table with the newly read values of the enc/battery

#include "iostream"
#include "stdio.h"
#include "SerialStream.h"   
#include "SerialPort.h"
#include "stdlib.h"
#include "string"
#include "cstring"
#include "math.h"
#include "sys/time.h"
#ifdef FINAL
    #include "cv.h"
    #include "update_frame.h"
    #include "our_bot.h"
    using namespace cv;
#endif

using namespace std;
using namespace LibSerial;

#define HANDSHAKE_CHK ((serial_buffer[1]=='F')&&(serial_buffer[2]=='I'))        // For handshake
/***************Global Variables******************/
SerialPort *pu;                                                 
int usart_fl=0;                                                 // Flag for usart_init
char charval[3];                                                // String to convert the value received into character for TX
unsigned char bot_code[5][11];                                            // Matrix which acts like the semaphore
double maxtime=0;                                               // The maximum delay in the current run. Active only if TIMER is 1
bool bypass_normal_protocol=0;                                  // Flag for generic purposes
char serial_buffer[10];                                         // Our buffer for serial communication
unsigned long int read_timeout=0;                               // Counter for timeouts
/************************************************************************************
 * Bot_code matrix has the following info for each bot and are mapped to their botID:
 * Connection status:   'x' -> Connected
 *                        0  -> Not Connected
 *                       'i' -> About to be interrupted
 * Bot status:          'b' -> Busy
 *                      'f' -> Free
 * Counters for no. of errors, successes and interruptions
 *************************************************************************************/
 /************************************************/

/***************Function prototypes****************/
void elecsleep(unsigned int mseconds);
int usart_init();
void bot_status();
void extract();
void prelude();
void Uinit();
void Uend();
bool rxstring(char *cstr, unsigned int timeout_val, bool print_str);
bool checkbotstat(int botID,char action);
int is_enc_cmd(char action);
void conv_value_char(int value);
int sendenccmd(int botID, char action, int value,unsigned char speed);
void e_sendenccmd(int botID, char action, int value,unsigned char speed);
bool check_bot_free(int botID);
bool make_bot_free(int botID);
void ensure_bot_free(int botID);
void wait_4_bot(int botID);
 /************************************************/

 
/*************************************************
 * FUNCTION NAME: elecsleep()
 * ARGUMENTS    : unsigned int mseconds => mseconds
 * DESCRIPTION  : Sleep for some time
 * RETURN VALUES: None
 ************************************************/
void elecsleep(unsigned int mseconds)
{
    struct timeval begin,end;
    double diff=0,diff_s=0,diff_us=0;
    double useconds=mseconds*1000;
    gettimeofday(&begin,NULL);
    do
    {
        gettimeofday(&end,NULL);
        diff_s=difftime(end.tv_sec,begin.tv_sec);
        diff_us=difftime(end.tv_usec,begin.tv_usec);
        diff=diff_s*1000000+diff_us;
    }while(diff<useconds);
}

/*************************************************
 * FUNCTION NAME: usart_init()
 * DESCRIPTION  : 1. Initialize USART
 *                2. The following are the parameters:
 *                   Baud Rate      => 9600 bps
 *                   Packet Size    => 8 bits
 *                   Parity         => NONE
 *                   StopBits       => 1
 *                   FLOW_CONTROL   => NONE
 * RETURN VALUES: Integer type
 *                0. Success
 *                1. If open failed
 *                2. If port is not available
 ************************************************/
int usart_init()
{
    if(!usart_fl)
        cout<<"Initializing"<<endl;
    if(pu->IsOpen())
        cout<<"Port already open"<<endl;
    else
    {
        if(!usart_fl)
            cout<<"Port not open...trying"<<endl;
 	   	try
        {
            pu->Open(SerialPort::BAUD_9600,
                    SerialPort::CHAR_SIZE_8,
                    SerialPort::PARITY_NONE,
                    SerialPort::StopBits(1),
                    SerialPort::FLOW_CONTROL_NONE
                    );
            usart_fl=0;
        }
        catch(SerialPort::OpenFailed)
        {
            cout<<"USART unavailable\r";
            usart_fl=1;
        }
        catch(SerialPort::AlreadyOpen)
        {
            cout<<"USART already open\r";
            usart_fl=2;
        }
        if(!usart_fl)
            cout<<"Initializing Done"<<endl;
    }
    return usart_fl;
}

/*************************************************
 * FUNCTION NAME: bot_status()
 * DESCRIPTION  : Display the bot_code matrix
 * RETURN VALUES: None
 ************************************************/
void bot_status()
{
    cout<<"**********************************************************Bots Status*****************************************************************"<<endl;
    /*cout<<"ID  "<<"Conx  "<<"Status  "<<"Errors  "<<"Success "<<"Interrupted "<<"No_ack   "<<"Gen_pwm  "<<"Enc_cnt "<<"Turn_pwm "<<"Enc_cnt_turn "<<"Battery "<<endl;
    for(int i=0;i<5;++i)
        cout<<i<<"     "<<bot_code[i][0]<<"      "<<bot_code[i][1]<<"      "<<int(bot_code[i][2])<<"        "<<int(bot_code[i][3])<<"        "<<int(bot_code[i][4])<<"         "<<int(bot_code[i][5])<<"         "<<int(bot_code[i][6])<<"        "<<int(bot_code[i][7])<<"         "<<int(bot_code[i][8])<<"         "<<int(bot_code[i][9])<<"         "<<int(bot_code[i][10])<<endl;*/
    cout<<"ID  "<<"Conx  "<<"Status  "<<"Errors  "<<"Success "<<"Interrupted "<<"No_ack   "<<"Gen_pwm  "<<"Enc_cnt "<<"Turn_pwm "<<"Enc_cnt_turn "<<"Battery_Voltage"<<endl;
    for(int i=0;i<5;++i)
        cout<<i<<"     "<<bot_code[i][0]<<"      "<<bot_code[i][1]<<"\t    "<<int(bot_code[i][2])<<"\t    "<<int(bot_code[i][3])<<"\t       "<<int(bot_code[i][4])<<"\t"<<int(bot_code[i][5])<<"\t  "<<int(bot_code[i][6])<<"\t   "<<int(bot_code[i][7])<<"\t    "<<int(bot_code[i][8])<<"\t      "<<int(bot_code[i][9])<<"\t\t"<<int(bot_code[i][10])<<endl;
    cout<<"**************************************************************************************************************************************"<<endl;
    if(TIMER)
        cout<<"Max time taken :"<<maxtime<<endl;
    cout<<endl;
}

/*************************************************
 * FUNCTION NAME: extract()
 * DESCRIPTION  : 1. Extracts the bot information after performing a successful handshake
 *                2. Updates the bot_code matrix
 *                3. Puts up the C2b message once all bots are connected
 * RETURN VALUES: NONE
 ************************************************/
void extract()
{
    char temp=0;
    char Bn[]="Bn";
    char hndshk[]="2FIRA";
    char C2b[]="C2b";
    int timedout=0,index=0,botID=0;
    do
    {
        cout<<endl;
        do
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                //Waiting for all connections to be made
            if(serial_buffer[0]=='L')
                cout<<'\r';
        }while(!HANDSHAKE_CHK||timedout);
        cout<<"Bot no."<<++index<<" detected"<<endl;
        cout<<"Handshake successful"<<endl;
        while(strcmp(Bn,serial_buffer)||timedout)               
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);         //Bn received
        }
        do
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);
        }while(timedout);                                                   //While the bot ID is not received timed out=>1
        botID=*serial_buffer-48;
        if(bot_code[botID][0]=='x')
        {
            cout<<"\nID "<<botID<<" already taken. Kindly flash another bot ID to this module"<<endl;
            Uend();
            exit(0);
        }
        bot_code[botID][0]='x';
        bot_code[botID][1]='f';
        for(int i=2;i<10;++i)
            bot_code[botID][i]=0;
        cout<<"The bot ID is "<<botID<<endl;
        if(index<PLAYERS)
            cout<<"\nWaiting for the next bot"<<endl<<endl;
    }while(index<PLAYERS);
    while(strcmp(C2b,serial_buffer)||timedout)               
    {
        timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                 //Connected to all bots.
            cout<<strcmp(C2b,serial_buffer)<<endl;
    }
    cout<<"Connected to the bots"<<endl;
}

/*************************************************
 * FUNCTION NAME: prelude()
 * DESCRIPTION  : Is the prelude for the match and sets-up the environment for extraction of bot IDs. 
 * RETURN VALUES: NONE
 ************************************************/
void prelude()
{
    char w4b[]="W4b";
    char next[]="N";
    char bn,bi;
    int i=1,timedout=0;
    do
    {
        pu->WriteByte('c');
        timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);
        if(*serial_buffer=='P')
        {
            cout<<"Communication working"<<endl;
            pu->WriteByte('s');
        }
    }while((*serial_buffer=='P')||timedout);
    timedout=0;
    while(strcmp(w4b,serial_buffer)||(timedout))
        timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                                  //Waiting for all connections to be made
    cout<<"Waiting for the bots"<<endl;
    extract();
    cout<<"Command beginning...waiting for next cmd prompt"<<endl<<endl;
    while(strcmp(next,serial_buffer)||(timedout))
        timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                                  //Waiting for all connections to be made
    cout<<"Next cmd prompt received"<<endl<<endl;
}

/*************************************************
 * FUNCTION NAME: Uinit()
 * DESCRIPTION  : The final abstracted function to be called to initialize the USART link 
 *                with the computer. It calles usart_init() and prelude.
 * RETURN VALUES: NONE
 ************************************************/
void Uinit()
{
    pu = new SerialPort("/dev/ttyACM0");
    while(usart_init());
    prelude();
}

/*************************************************
 * FUNCTION NAME: Uend()
 * DESCRIPTION  : The final abstracted function to be called to end the USART link 
 *                with the computer. It calles bot_status() and closes the link.
 * RETURN VALUES: NONE
 ************************************************/
void Uend()
{
    bot_status();
    pu->Close();
    delete pu;
}

/*************************************************
 * FUNCTION NAME: rxstring()
 * ARGUMENTS    : char *cstr                => The received string is written into this
 *                unsigned int timeout_val  => The timeout value in milli seconds
 *                bool print_str            => Whether to print the string or not
 * DESCRIPTION  : 1. Reads the strings AP sends 
 *                2. Updates the bot_code matrix if xD is received
 * RETURN VALUES: BOOL 
 *                0. Success
 *                1. Time_out
 ************************************************/
bool rxstring(char *cstr, unsigned int len, bool print_str)
{
    char temp=0;
    int i=0;
    while(temp!='\n')
    {
        try
        {
            temp=pu->ReadByte(len);
        }
        catch(SerialPort::ReadTimeout &e)
        {
            read_timeout++;
            if(SHOW_TIMEOUT)
                cout<<"Read Timeout "<<read_timeout<<endl;
            return 1;
        }
        if((temp!='\n')&&(temp!=0)&&(temp!=' '))
        {
            cstr[i]=temp;
            ++i;
            //cout<<i++<<temp<<'x'<<endl;
        }
    }
    cstr[i]='\0';
    if(print_str)
        puts(cstr);
    if(cstr[1]=='D')
    {
        if(STR_DEBUG)
            cout<<"Bot "<<cstr[0]<<" is free now"<<endl;
        i=cstr[0]-48;
        bot_code[i][1]='f';                                     //Received D and hence marking the bot free if it was busy
        if(bot_code[i][0]=='i')                                 //Bot was going to be interrupted so if D was received success
        {
            bot_code[i][0]='x';
            bot_code[i][4]++;
        }
    }
    if((cstr[1]=='n')&&(cstr[2]=='o')&&(cstr[3]=='a')&&(cstr[4]=='c')&&(cstr[5]=='k'))
    {
        if(STR_DEBUG)
            cout<<"No ack received"<<endl;
        bot_code[cstr[0]-48][1]='n';
        bot_code[cstr[0]-48][5]++;
        return 1;
    }
    return 0;
}

/*************************************************
 * FUNCTION NAME: checkbotstat()
 * ARGUMENTS    : 1. int botID      => The bot ID for which the check needs to be made
 *                2. char action    => To allow the interrupt action to pass through
 * DESCRIPTION  : 1. Go through the bot_code and determine the status of the bot concerned.
 *                2. Allow the interrupt action to pass through
 *                3. Used inside the sendenccmd
 * RETURN VALUES: BOOL
 *                0. Success
 *                1. Failed
 ************************************************/
bool checkbotstat(int botID,char action)
{
    if((botID<0)||(botID>4))
    {
        cout<<"Invalid ID"<<endl;
        return 1;
    }
    else if(bot_code[botID][0]==0)
    {
        cout<<"Bot "<<botID<<" still not connected"<<endl;
        return 1;
    }
    else
    {
        if(bot_code[botID][1]=='b')
        {
            if(action=='R')
                return 0;
            else if(action=='s')
                bot_code[botID][0]='i';
            else
            {
                cout<<"Bot "<<botID<<" still busy"<<endl;
                return 1;
            }
        }
    }
    return 0;
}

/*************************************************
 * FUNCTION NAME: is_enc_cmd()
 * ARGUMENTS    : char action   => To determine the course of action to be taken while executing the command
 * DESCRIPTION  : Determine the course of action to be taken while executing the command
 * RETURN VALUES: INTEGER
 *                0. NFS style
 *                1. Encoder based command
 *                2. Undefined action
 ************************************************/
int is_enc_cmd(char action)
{
    switch(action)
    {
        case 'E':{
                     return 3;
                 }
        case 'w':
        case 'a':
        case 'd':
        case 's':{
                    return 0;
                 }
        case 'f':
        case 'r':
        case 'l':
        case 'b':
        case 'p':
        case 'P':
        case 't':
        case 'T':{
                     return 5;
                 }
        case 'F':
        case 'B':
        case 'V':{
                     return 4;
                 }
        case 'R':{
                     return 1;
                 }
    }
    return 2;
}

/*************************************************
 * FUNCTION NAME: conv_value_char()
 * ARGUMENTS    : int value => The value for conversion
 * DESCRIPTION  : Converts integers into a character string stored in charval for TX
 * RETURN VALUES: NONE
 ************************************************/
void conv_value_char(int value)
{
    charval[0]=(value/100+48);
    value=value%100;
    charval[1]=(value/10+48);
    value=value%10;
    charval[2]=value+48;
}

/*************************************************
 * FUNCTION NAME: sendenccmd() 
 * ARGUMENTS    : int botID     => BotID to which you want to direct the command
 *                char action   => The action to be ordered
 *                bool ensure   => 1 means ensure that the bot is free and then transmit the command
 *                                 By default at 0, meaning no guarantees
 *                int value     => Value for the action
 *                char speed    => Value2 for the action
 * DESCRIPTION  : Algorithm followed
 *                ------------------
 *                1. Check the validity of the command and status of the bot
 *                2. Setup a clean connection between the AP and the comp
 *                3. Send the action or action+value based on the nature of action
 *                4. Wait for the mirrored cmd
 *                5. Wait for 'N' to signal that AP is ready for the next commmand
 * RETURN VALUES: 
 ************************************************/
int sendenccmd(int botID, char action, int value=0, unsigned char speed=0)
{
    int timedout=1,timecount=0;
    char temp=0;
    char done[]="xD";
    char recv='R';
    done[0]=char(botID+48);
    char next[]="N";
    char failed[]="F";
    if(checkbotstat(botID,action))
    {
        cout<<endl;
        ensure_bot_free(botID);                                              //A second chance
        if(checkbotstat(botID,action))
          return 1;
    }
    if(is_enc_cmd(action)==2)                                       
    {
        cout<<"Unknown action command provided"<<action<<endl<<endl;
        return 1;
    }
    else if(is_enc_cmd(action)==3)                                            // Display the encoder values
    {
        int speed_default=0,enc_count=0,enc_count_turn=0,turn=0;
        cout<<"Displaying the encoder values"<<endl;
        bot_code[botID][1]='b';
        while(!((temp>='0')&&(temp<='4')))
        {
            pu->WriteByte(botID+48);
            try
            {
                temp=pu->ReadByte(TIMEOUT_READ);
                if(STR_DEBUG)
                    cout<<"Trying to link to bot "<<botID<<". It spat out the character "<<(int)temp<<endl;
            }
            catch(SerialPort::ReadTimeout &e)
            {
                cout<<"nope";
            }
        }
        pu->WriteByte(action);
        timecount=0;
        while(((serial_buffer[0]!=recv)||(timedout))&&(timecount<NO_TIMEOUT))
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                       // Waiting for 'R'
            if(timedout)
            {
                timecount++;
                if(bot_code[botID][1]=='n')
                {
                    bot_code[botID][1]='f';
                    return 2;
                }
            }
        }
        if(timecount==NO_TIMEOUT)
        {
            bot_code[botID][1]='f';                                      // Freeing up the bot
            if(STR_DEBUG)
            {
                cout<<"Freeing up the bot as TX failed"<<endl;
                cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'R'"<<endl;
            }
            return 1;                                                    // If no mirrored command then time out 
        }
        while((!((serial_buffer[0]==(botID+48))&&(serial_buffer[1]=='E')))&&(timecount<NO_TIMEOUT))
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                               // Waiting for 'N' 
            if(timedout)
            {
                timecount++;
                if(bot_code[botID][1]=='n')
                {
                    bot_code[botID][1]='f';
                    return 2;
                }
            }
        }
        if(timecount==NO_TIMEOUT)
        {
            if(STR_DEBUG)
            {
                cout<<"Timed out with bot "<<botID<<" while waiting for the correct encoder info."<<endl;
            }
            return 1;
        }
        speed_default=serial_buffer[2];
        enc_count=serial_buffer[3];
        turn=serial_buffer[4];
        enc_count_turn=serial_buffer[5];
        timecount=0;
        while((strcmp(next,serial_buffer)||timedout)&&(timecount<NO_TIMEOUT))
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                               // Waiting for 'N' 
            if(timedout)
            {
                timecount++;
                if(bot_code[botID][1]=='n')
                {
                    bot_code[botID][1]='f';
                    return 2;
                }
            }
        }
        if(timecount==NO_TIMEOUT)
        {
            if(STR_DEBUG)
            {
                cout<<"Timed out with bot "<<botID<<" while waiting for 'N' after retreival"<<endl;
            }
            return 1;
        }
        if(STR_DEBUG)
        {
            cout<<"Speed:"<<speed_default<<endl;
            cout<<"Enc_Count:"<<enc_count<<endl;
            cout<<"Turn:"<<turn<<endl;
            cout<<"Enc_count_turn:"<<enc_count_turn<<endl;
        }
        //if(update_table)
        bot_code[botID][6]=speed_default;
        bot_code[botID][7]=enc_count;
        bot_code[botID][8]=turn;
        bot_code[botID][9]=enc_count_turn;
    }
    else if(is_enc_cmd(action)==4)                              //Encoder based command the way bademi wants 
    {
        if(value>max_enc_value)
        {
            cout<<"Provided encoder value is too large:"<<action<<value<<endl<<endl;
            return 1;
        }
        else if(!(speed && value))
        {
            cout<<"We need non-zero values for the encoder"<<endl;
            cout<<speed<<value<<endl;
            return 1;
        }
        else if(value>max_enc_value)
        {
            cout<<"We can't go more than 255 cm at a stretch using this command"<<endl<<endl;
            return 1;
        }
        else
        {
            if(value=='\n')                
                value=9;                                        //Can't send \n as we will have problems while receiving
            if(speed=='\n')
                speed=11;                                        //Can't send \n as we will have problems while receiving
            bot_code[botID][1]='b';
            int speed_disp=speed;
            char dist=value;
            if(speed_disp<0)
                speed_disp+=256;
            if(STR_DEBUG)
            {
                if(action!='V')
                    cout<<"Speed moderated encoder based command:"<<botID<<action<<value<<" at a speed level of "<<speed_disp<<endl;
                else
                {

                    cout<<"Differential speed based command:"<<botID<<action<<" at a speed level of \n LEFT MOTOR:"<<(int)dist<<endl<<"RIGHT MOTOR:"<<(int)speed<<endl;
                }
            }
            else
            {
                if(action!='V')
                    cout<<"Speed moderated encoder based command:"<<botID<<action<<value<<'"'<<speed_disp<<endl;
                else
                    cout<<"Differential speed based command:"<<botID<<action<<value<<'"'<<speed_disp<<endl;
            }
            while(!((temp>='0')&&(temp<='4')))
            {
                pu->WriteByte(botID+48);
                try
                {
                    temp=pu->ReadByte(TIMEOUT_READ);
                    if(STR_DEBUG)
                        cout<<"Trying to link to bot "<<botID<<". It spat out the character"<<(int)temp<<endl;
                }
                catch(SerialPort::ReadTimeout &e)
                {
                    cout<<"nope";
                }
            }
            pu->WriteByte(action);
            timecount=0;
            do
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                // AP will send out the decision as a sync 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }while((*serial_buffer!='4')&&(timecount<NO_TIMEOUT));              //Essential as the decision taken by AP takes time
            if(timecount==NO_TIMEOUT)
            {
                bot_code[botID][1]='f';                                      // Freeing up the bot
                if(STR_DEBUG)
                {
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for the decision."<<endl;
                }
                return 1;                                                    // If no mirrored command then time out 
            }
            pu->WriteByte(dist);
            pu->WriteByte(speed);
            cout<<"Next cmd sent"<<endl;
            timecount=0;
#ifdef SEC
            while(((serial_buffer[0]!=recv)||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                       // Waiting for 'R'
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                bot_code[botID][1]='f';                                      // Freeing up the bot
                if(STR_DEBUG)
                {
                    cout<<"Freeing up the bot as TX failed"<<endl;
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'R'"<<endl;
                }
                return 1;                                                    // If no mirrored command then time out 
            }
            timecount=0;
            while((((serial_buffer[0]!=botID+48)&&(serial_buffer[1]!=action)&&(serial_buffer[2]!=value)&&(serial_buffer[3]!=speed))||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                       // Waiting for the mirrored cmd 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                if(STR_DEBUG)
                {
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for mirror"<<endl;
                }
                return 1;
            }
            timecount=0;
            cout<<"Mirrored cmd received"<<endl<<endl;
#endif
            while((strcmp(next,serial_buffer)||timedout)&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                        // Waiting for 'N' 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                if(bot_code[botID][1]=='b')                                // If the bot is busy, only then show timeout. Else it
                                                                           // is obviously going to timeout
                {
                    if(STR_DEBUG)
                    {
                        cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'N'"<<endl;
                    }
                    return 1;
                }
            }
            cout<<"Next cmd prompt received"<<endl<<endl;
        }
    }
    else if(is_enc_cmd(action)==5)                              //Encoder based command hence value!=0
    {
        if(value>max_enc_value)
        {
            cout<<"We can't go more than 255 cm at a stretch using this command"<<endl<<endl;
            return 1;
        }
        else
        {
            bot_code[botID][1]='b';
            cout<<"Encoder based command:"<<botID<<action<<value<<endl;
            int val=0;
            while(!((temp>='0')&&(temp<='4')))
            {
                pu->WriteByte(botID+48);
                try
                {
                    temp=pu->ReadByte(TIMEOUT_READ);
                    if(STR_DEBUG)
                        cout<<"Trying to link to bot "<<botID<<". It spat out the character "<<(int)temp<<endl;
                }
                catch(SerialPort::ReadTimeout &e)
                {
                    cout<<"nope";
                }
            }
            pu->WriteByte(action);
            conv_value_char(value);
            do
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);               // AP will send out the decision as a sync 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }while((*serial_buffer!='5')&&(timecount<NO_TIMEOUT));              //Essential as the decision taken by AP takes time
            if(timecount==NO_TIMEOUT)
            {
                bot_code[botID][1]='f';                                      // Freeing up the bot
                if(STR_DEBUG)
                {
                    cout<<"Freeing up the bot as TX failed"<<endl;
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for the decision."<<endl;
                }
                return 1;                                                    // If no mirrored command then time out 
            }
            pu->WriteByte(charval[0]);
            pu->WriteByte(charval[1]);
            pu->WriteByte(charval[2]);
            cout<<"Next cmd sent"<<endl;
            timecount=0;
#ifdef SEC
            while(((serial_buffer[0]!=recv)||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                       // Waiting for 'R'
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                bot_code[botID][1]='f';                                      // Freeing up the bot
                if(STR_DEBUG)
                {
                    cout<<"Freeing up the bot as TX failed"<<endl;
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'R'"<<endl;
                }
                return 1;                                                    // If no mirrored command then time out 
            }
            timecount=0;
            while((((serial_buffer[0]!=botID+48)&&(serial_buffer[1]!=action)&&(value!=val))||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                       // Waiting for the mirrored cmd 
                val=(serial_buffer[2]-48)*100+(serial_buffer[3]-48)*10+(serial_buffer[4]-48);
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                if(STR_DEBUG)
                {
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for mirror"<<endl;
                }
                return 1;
            }
            timecount=0;
            cout<<"Mirrored cmd received"<<endl<<endl;
#endif
            while((strcmp(next,serial_buffer)||timedout)&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                        // Waiting for 'N' 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                if(bot_code[botID][1]=='b')                                  // If the bot is busy, only then show timeout. Else it
                                                                              // is obviously going to timeout
                {
                    if(STR_DEBUG)
                    {
                        cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'N'"<<endl;
                    }
                    return 1;
                }
            }
            cout<<"Next cmd prompt received"<<endl<<endl;
        }
    }
    else if(is_enc_cmd(action)==1)                                            // Retrieve the done command
    {
        while(!((temp>='0')&&(temp<='4')))
        {
            pu->WriteByte(botID+48);
            try
            {
                temp=pu->ReadByte(TIMEOUT_READ);
                if(STR_DEBUG)
                    cout<<"Trying to link to bot "<<botID<<". It spat out the character "<<(int)temp<<endl;
            }
            catch(SerialPort::ReadTimeout &e)
            {
                cout<<"nope";
            }
        }
        pu->WriteByte(action);
        while((strcmp(next,serial_buffer)||timedout)&&(timecount<NO_TIMEOUT))
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                               // Waiting for 'N' 
            if(timedout)
            {
                timecount++;
                if(bot_code[botID][1]=='n')
                {
                    bot_code[botID][1]='f';
                    return 2;
                }
            }
        }
        if(timecount==NO_TIMEOUT)
        {
            if(STR_DEBUG)
            {
                cout<<"Timed out with bot "<<botID<<" while waiting for 'N' after retreival"<<endl;
            }
            return 1;
        }
    }
    else                                                                      // NFS style commands
    {
        struct timeval begin,end;
        double diff=0,diff_s=0,diff_us=0;
        if(DTIMER)
        {
            gettimeofday(&begin,NULL);
        }
        if((action=='s')&&(bot_code[botID][1]=='b'))
            bypass_normal_protocol=1;                                         // We are going to interrupt and hence no mirror
        else
            bypass_normal_protocol=0;
        bot_code[botID][1]='b';
        cout<<"NFS style:"<<botID<<action<<endl;
        while(!((temp>='0')&&(temp<='4')))
        {
            pu->WriteByte(botID+48);
            try
            {
                temp=pu->ReadByte(TIMEOUT_READ);
                if(STR_DEBUG)
                    cout<<"Trying to link to bot "<<botID<<". It spat out the character "<<(int)temp<<endl;
            }
            catch(SerialPort::ReadTimeout &e)
            {
                cout<<"nope";
            }
        }
        if(DTIMER)
        {
            gettimeofday(&end,NULL);
            diff_s=difftime(end.tv_sec,begin.tv_sec);
            diff_us=difftime(end.tv_usec,begin.tv_usec);
            diff=diff_s*1000000+diff_us;
            cout<<"Time taken for linking up: "<<diff<<" us"<<endl;
            begin=end;
        }
        pu->WriteByte(action);
        cout<<"Next cmd sent"<<endl;
        timecount=0;
#ifdef SEC
        if(!bypass_normal_protocol)
        {
            while(((serial_buffer[0]!=recv)||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                              // Waiting for 'R' 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                bot_code[botID][1]='f';
                if(STR_DEBUG)
                {
                    cout<<"Freeing up the bot as TX failed"<<endl;
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'R'"<<endl;
                }
                return 1;
            }
            timecount=0;
            if(DTIMER)
            {
                gettimeofday(&end,NULL);
                diff_s=difftime(end.tv_sec,begin.tv_sec);
                diff_us=difftime(end.tv_usec,begin.tv_usec);
                diff=diff_s*1000000+diff_us;
                cout<<"Time taken for packet to come back: "<<diff<<" us"<<endl;
                begin=end;
            }
            while((!((serial_buffer[0]==(botID+48))&&(serial_buffer[1]==action))||(timedout))&&(timecount<NO_TIMEOUT))
            {
                timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                              // Waiting for the mirror 
                if(timedout)
                {
                    timecount++;
                    if(bot_code[botID][1]=='n')
                    {
                        bot_code[botID][1]='f';
                        return 2;
                    }
                }
            }
            if(timecount==NO_TIMEOUT)
            {
                if(STR_DEBUG)
                {
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for mirror"<<endl;
                }
                return 1;
            }
            timecount=0;
            cout<<"Mirrored cmd received"<<endl<<endl;
            if(DTIMER)
            {
                gettimeofday(&end,NULL);
                diff_s=difftime(end.tv_sec,begin.tv_sec);
                diff_us=difftime(end.tv_usec,begin.tv_usec);
                diff=diff_s*1000000+diff_us;
                cout<<"Time taken for mirror: "<<diff<<" us"<<endl;
                begin=end;
            }
        }
#endif
        while((strcmp(next,serial_buffer)||timedout)&&(timecount<NO_TIMEOUT))
        {
            timedout=rxstring(serial_buffer,TIMEOUT_VAL,STR_DEBUG);                               // Waiting for 'N' 
            if(timedout)
            {
                timecount++;
                if(bot_code[botID][1]=='n')
                {
                    bot_code[botID][1]='f';
                    return 2;
                }
            }
        }
        if(timecount==NO_TIMEOUT)
        {
            if(bot_code[botID][1]=='b')
            {
                if(STR_DEBUG)
                {
                    cout<<"Timed out while instructing bot "<<botID<<" while waiting for 'N'"<<endl;
                }
                return 1;
            }
        }
        if((bypass_normal_protocol)&&(bot_code[botID][1]=='f'))
            cout<<"Command at bot "<<botID<<" successfully interrupted"<<endl<<endl;
        else if(bypass_normal_protocol)
        {
            cout<<"Interruption at bot "<<botID<<" unsuccessful"<<endl<<endl;
            return 1;
        }
        cout<<"Next cmd prompt received"<<endl<<endl;
        if(DTIMER)
        {
            gettimeofday(&end,NULL);
            diff_s=difftime(end.tv_sec,begin.tv_sec);
            diff_us=difftime(end.tv_usec,begin.tv_usec);
            diff=diff_s*1000000+diff_us;
            cout<<"Time taken for next cmd: "<<diff<<" us"<<endl;
            begin=end;
        }
    }
    return 0;
}

/*************************************************
 * FUNCTION NAME: e_sendenccmd()
 * ARGUMENTS    : Same as sendenccmd()
 * DESCRIPTION  : Calls sendenccmd and also updates the bot_code matrix based on the return value
 * RETURN VALUES: NONE
 ************************************************/
void e_sendenccmd(int botID, char action, int value=0, unsigned char speed=0)
{
    struct timeval begin,end;
    double diff=0,diff_s=0,diff_us=0;
    char res=sendenccmd(botID,action,value,speed);
#ifdef FINAL
    double bx=0,by=0,ba=0;
    updateframe();
    bx=bot[botID].x;
    by=bot[botID].y;
    ba=bot[botID].angle;
#endif
    if(TIMER)
    {
        gettimeofday(&begin,NULL);
    }
    if(res==1)
    {
        cout<<"Bot "<<botID<<" commits error no. "<<int(++bot_code[botID][2])<<endl<<endl;
        if(auto_correct)
        {
            if(STR_DEBUG)
                cout<<"Trying again as the bot was found to be busy..."<<endl;
            ensure_bot_free(botID);
            while(sendenccmd(botID,action,value,speed));
        }
    }
    else if((enforce_cmd)&&(res==2))                                             //No ack
    {
        if(STR_DEBUG)
            cout<<"Trying again as the ack failed in last attempt..."<<endl;
        do
        {
#ifdef FINAL
            updateframe();
            if((abs(ba-bot[botID].angle)>ANGLE_TOL)||(abs(bx-bot[botID].x)>X_TOL)||(abs(by-bot[botID].y)>Y_TOL))
            {
                cout<<"We have successfully detected a rogue packet!!!"<<endl;
                break;
            }
#endif
        }while(sendenccmd(botID,action,value,speed));
        //The tally has not been added to success as it is known that eventually it has to succeed
    }
    else
    {
        ++bot_code[botID][3];
        if(TIMER)
        {
            gettimeofday(&end,NULL);
            diff_s=difftime(end.tv_sec,begin.tv_sec);
            diff_us=difftime(end.tv_usec,begin.tv_usec);
            diff=diff_s*1000000+diff_us;
            if(diff_s)
                cout<<"Maybe an error ==> diff_s:"<<diff_s<<"    diff_us:"<<diff_us<<endl;
            cout<<"Time taken: "<<diff<<" us"<<endl<<endl;
            if(diff>maxtime)
                maxtime=diff;
        }
    }
}

/*************************************************
 * FUNCTION NAME: check_bot_free()
 * ARGUMENTS    : int botID     => Bot no. to be checked
 * DESCRIPTION  : Goes through the buffers maintained by Access Point to see if xD has come
 * RETURN VALUES: 0.Busy
 *                1.Free
 ************************************************/
bool check_bot_free(int botID)
{
    if(bot_code[botID][1]=='b')
    {
        sendenccmd(botID,'R');
        if(bot_code[botID][1]=='f')
        {
            if(STR_DEBUG)
                cout<<"Bot made free\n"; 
        }
        else
            return 0;
    }
    else if(STR_DEBUG)
        cout<<"Bot is already free\n";
    return 1;
}

/*************************************************
 * FUNCTION NAME: make_bot_free()
 * ARGUMENTS    : int botID     => Bot no. to be checked
 * DESCRIPTION  : Sends the interrupting command
 * RETURN VALUES: 0.Busy
 *                1.Free
 ************************************************/
bool make_bot_free(int botID)
{
    if(bot_code[botID][1]=='b')
    {
        sendenccmd(botID,'s');
        if(bot_code[botID][1]=='f')
        {
            if(STR_DEBUG)
                cout<<"Bot made free\n"; 
        }
        else
            return 0;
    }
    else if(STR_DEBUG)
        cout<<"Bot is already free\n";
    return 1;
}
/*************************************************
 * FUNCTION NAME: ensure_bot_free()
 * ARGUMENTS    : int botID     => Bot no. to be checked
 * DESCRIPTION  : Call check_bot_free till bot becomes free
 * RETURN VALUES: NONE
 ************************************************/
void ensure_bot_free(int botID)
{
    elecsleep(SLEEP_TIME);
    if(STR_DEBUG)
        cout<<"Trying to ensure bot is free\n";
    int ensure_timeout=0;
    if((bot_code[botID][0]=='x')||(bot_code[botID][0]=='i'))            //Ensure it is an existing bot
    {
        while((bot_code[botID][1]=='b')&&(ensure_timeout<ENSURE_TIMEOUT))
        {
            ensure_timeout++;
            check_bot_free(botID);
        }
        if(!check_bot_free(botID))
            while(!make_bot_free(botID));
    }
    else if(STR_DEBUG)
    {
        cout<<"Bot doesn't exist"<<endl;
    }
    if(STR_DEBUG)
        bot_status();
}

/*************************************************
 * FUNCTION NAME: wait_4_bot()
 * ARGUMENTS    : int botID     => Bot no. to be checked
 * DESCRIPTION  : Call check_bot_free till bot becomes free
 * RETURN VALUES: NONE
 ************************************************/
void wait_4_bot(int botID)
{
    unsigned char wait_timeout=0;
    if(STR_DEBUG)
        cout<<"\nWaiting for the bot "<<botID<<endl;
    while(!check_bot_free(botID))
    {	
        if(wait_timeout>NO_TIMEOUT_4_WAIT)
	{
		cout<<"URGENT!!! Report to Bazooka that we need to change the NO_TIMEOUT_4_WAIT"<<endl;
		cout<<"Press any key to continue the program...The command under execution will be interrupted for you. :)"<<endl;
		ensure_bot_free(botID);
		break;
	}
    }
}

#ifndef FINAL
int main()
{	
	Uinit();
    int botID=3;
    e_sendenccmd(3,'P',230);
    e_sendenccmd(3,'T',70);
    bot_status();
    for(int i=0;i<5;++i)
    {
        e_sendenccmd(3,'F',' ',100);
        wait_4_bot(3);
        e_sendenccmd(3,'r',90);
        wait_4_bot(3);
    }
    Uend();
}
#endif
