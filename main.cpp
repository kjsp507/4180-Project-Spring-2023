#include "mbed.h"
#include "EthernetInterface.h"
#include "NTPClient.h"
#include "rtos.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "TCPSocketConnection.h"
#include "ifttt.h"
#include <stdio.h>

EthernetInterface eth;
NTPClient ntp;
Mutex lcd_mutex; //mutex for lcd
TCPSocketConnection socket;
IFTTT iftttLight("light","mViWEbUiLPqhP4PGp0MPQ", &socket); // EventName, Secret Key, socket to use
IFTTT iftttTemp("temp","mViWEbUiLPqhP4PGp0MPQ", &socket); // EventName, Secret Key, socket to use


RawSerial pc(USBTX, USBRX); // tx, rx

uLCD_4DGL uLCD(p28,p27,p30); // serial tx, serial rx, reset pin;
Serial blue(p9,p10); //bluetooth setup
PwmOut speaker(p21);// using PWM for speaker output.
// RGB Setup
PinDetect RPG_A(p16,PullUp);//encoder A and B pins/bits use interrupts
PinDetect RPG_B(p17,PullUp);
PinDetect RPG_PB(p18); //encode pushbutton switch "SW" on PCB

// PushButtons
DigitalIn alarmPB(p22,PullUp);
DigitalIn iftttPB(p23,PullUp);

DigitalOut led1(LED1);//blink every one second
DigitalOut led2(LED2);// check Alarm enabled
DigitalOut led3(LED3);
DigitalOut led4(LED4); //check sending IFTTT

// IFTTT iftttLight(), iftttTemp();
//void setupRPG(), alarmScale(), getTime();
//Alarm
volatile bool alarm_enable    =true; // alarm ON OFF status
volatile bool timeCheck       =false; //compare current time and alarmTime
volatile int alarmMenuSelect  =0; // Select Menu for Min, Hr, and Volume. 0 = neither, 1=min, 2=Hrs, 3=Volume.
volatile int alarmMinute      =00;    // 0-59
volatile int alarmHour        =00;    // 2-23
volatile float speakerVolume  =50.0; //Volume of Speaker, 1.0~100.0 scale
//Temperature 
volatile int max_temp         =0;
volatile int min_temp         =100;
volatile float tempC; //current Temperature.
//Time
time_t ctTime; // realtime
struct tm* currentTime; //Real Rime with struct tm. 

//https://cplusplus.com/reference/ctime/tm/

///////////////////////////////////RPG///////////////////////////////////
//variable for RPG encoder
volatile int old_enc = 0;
volatile int new_enc = 0;
//volatile int enc_count = 50;

//Instead of a slow 16 case statement use a faster table lookup of truth table
//index table with (previous encoder AB value<<2 | current current encoder AB value) 
//to find -1(CCW),0,or 1(CW) change in count each time a bit changes state
//Always want Interrupt routines to run fast!
//const puts data in read only Flash instead of RAM
const int lookup_table[] = {0,1,-1,0,0,0,0,0,0,0,0,0,0,1,-1,0};
//Encoder bit change interrupt service routine
//called whenever one of the two A,B encoder bits change state
void Enc_change_ISR(void)
{
    new_enc = RPG_A<<1 | RPG_B;//current encoder bits
    //check truth table for -1,0 or +1 added to count "lookup_table[old_enc<<2 | new_enc]"
    //add or substract min or Hrs depend on the alarmMenuSelect.
    if(alarmMenuSelect == 1) alarmMinute += lookup_table[old_enc<<2 | new_enc];
    if(alarmMenuSelect == 2) alarmHour += lookup_table[old_enc<<2 | new_enc];
    if(alarmMenuSelect == 3) speakerVolume += lookup_table[old_enc<<2 | new_enc];
    //alarmScale();
    old_enc = new_enc;//refresh encoder status.
    Thread::wait(100);
}

// selecting MenuButtons (Min, Hr, Volume)
//debounced RPG pushbutton switch callback
void PB_callback(void)
{
    if(alarmMenuSelect < 3 && alarmMenuSelect >= 0)
        alarmMenuSelect += 1;
    else
        alarmMenuSelect = 0;
    // led2= alarm_enable;
}
void setupRPG(){
    printf("RPG Start\r\n");
    //debounce RPG center pushbutton
    RPG_PB.mode(PullDown);
    RPG_PB.attach_deasserted(&PB_callback);
    RPG_PB.setSampleFrequency();
    // generate an interrupt on any change in either encoder bit (A or B)
    RPG_A.attach_deasserted(&Enc_change_ISR);
    RPG_B.attach_deasserted(&Enc_change_ISR);
    RPG_A.setSampleFrequency(100);
    RPG_B.setSampleFrequency(100);
    printf("RPG set\r\n");
}
void alarmScale(){
    // Hr 0-23, Min 0-59, Volume 0.0~100.0
    if(alarmMinute < 0) alarmMinute = 59;
    if(alarmMinute > 59) alarmMinute = 0; 
    if(alarmHour < 0) alarmHour = 23;
    if(alarmHour > 23) alarmHour = 0;
    if(speakerVolume <= 0.0) speakerVolume = 0;
    if(speakerVolume >= 100.0) speakerVolume = 100.0;
}

//////////////////////////////Temperature////////////////////////////////
//Setup a new class for TMP36 sensor
class TMP36{
    public:
        TMP36(PinName pin);
        TMP36();
        operator float ();
        float read();
    private:
    //class sets up the AnalogIn pin
        AnalogIn _pin;
    };

    TMP36::TMP36(PinName pin) : _pin(pin)
    {
    // _pin(pin) means pass pin to the AnalogIn constructor
    }

    float TMP36::read()
    {
    //convert sensor reading to temperature in degrees C
        return ((_pin.read()*3.3)-0.500)*100.0;
    }
    //overload of float conversion (avoids needing to type .read() in equations)
    TMP36::operator float ()
    {
    //convert sensor reading to temperature in degrees C
        return ((_pin.read()*3.3)-0.500)*100.0;
}
//use the new class to set p15 to analog input to read and convert TMP36 sensor's voltage output
TMP36 myTMP36(p15);
//also setting unused analog input pins to digital outputs reduces A/D noise a bit
//see http://mbed.org/users/chris/notebook/Getting-best-ADC-performance/
void temperature(void const *args){
    while(1) {

        lcd_mutex.lock();
        tempC = myTMP36;
        //set min and max temperature. 
        if(tempC > max_temp) max_temp = tempC;
        if(tempC < min_temp) min_temp = tempC;
        uLCD.locate(1,2);
        uLCD.color(GREEN);
        uLCD.printf(" Temp = %3.2F C",myTMP36.read());
        lcd_mutex.unlock();
        Thread::wait(100);
    }
}

//////////////////////////////////TIME///////////////////////////////////////////////
//get time using NTP from pool.ntp.org
void getTime(){

        eth.init(); //Use DHCP
        wait(1);
        printf("Getting IP Address\r\n");
        if(eth.connect(60000)!=0) {
            printf("DHCP error - No IP\r\n");
            wait(10);
        } else {
            printf("IP is %s\n", eth.getIPAddress());
            wait(1);
        }
        printf("Trying to update time...\r\n");
        if (ntp.setTime("0.pool.ntp.org") == 0) {
            printf("Set time successfully\r\n"); 
            //chage timezone to EST
            set_time(time(NULL)-3600*4);   
            ctTime = time(NULL);
            currentTime = localtime(&ctTime);
            printf("%s\r\n", ctime(&ctTime));
            printf("Current Time (EST)\r\n)");
            wait(1);
        } else {
            printf("NTP Error\r\n");
        }
        eth.disconnect();
}

//comapre alarm time and current time to activate alarm. Also check alarmPB to change status of alarm enable.
void checkAlarm(void const *args){
    while(1){
        if(alarmHour==currentTime->tm_hour && alarmMinute==currentTime->tm_min){
            timeCheck = true;
        }
        else {
            timeCheck = false;
        }
        if(alarm_enable && timeCheck){
        }
        if(alarmPB == 0){
            alarm_enable = !alarm_enable;
        }
        Thread::wait(1000);
    }
}
//Alarm Sound
void siren(void const *args){
    while(1){
        speaker = speakerVolume/100.0;
        if(alarm_enable && timeCheck){ //turn on only time matched and alarm enabled
            speaker.period(1.0/969.0);
            Thread::wait(500);
            speaker.period(1.0/800.0);
            Thread::wait(500);
            speaker = 0;
        }
        else {
        speaker = 0;
        }
        Thread::wait(10);
        speaker = 0;
    }
}
//display time on uLCD
void displayTime(void const *args){
    while(1){
        lcd_mutex.lock();
        uLCD.locate(2,5);
        uLCD.color(WHITE);
        uLCD.printf("NOW : %02d:%02d:%02d", currentTime->tm_hour,currentTime->tm_min, currentTime->tm_sec);
        lcd_mutex.unlock();
        Thread::wait(100);
    }
}

//bluetooth setup. 
/* 
1        TurOnOff alarm_enable
2        
Up(5)    increment
Down(6)  decrement
Left(7)  Next Menu 
Right(8) Previous Menu
*/
void bluetooth(void const *args){
    char bnum=0;
    char bhit=0;
    while(1) {
        lcd_mutex.lock();
        if(blue.readable()){
            if (blue.getc()=='!') {
                if (blue.getc()=='B') { //button data packet
                    bnum = blue.getc(); //button number
                    bhit = blue.getc(); //1=hit, 0=release
                    if (blue.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                        //myled = bnum - '0'; //current button number will appear on LEDs
                        switch (bnum) {
                            case '1': //number button 1
                                if (bhit=='1') {
                                    alarm_enable = !alarm_enable;
                                } else {
                                    //add release code here
                                }                                
                                break;
                            case '2': //number button 2
                                if (bhit=='1') {
                                    //add hit code here
                                    led4 = 1;
                                    uLCD.locate(2,13);
                                    uLCD.color(BLUE);
                                    // uLCD.printf("LIGHT %07S","Trigger");
                                    uLCD.printf("%15S","LIGHT Triggered");
                                    iftttLight.addIngredients("TurnOnLight!","GET1","data1");
                                    iftttLight.trigger(IFTTT_GET);
                                    led4 = 0;
                                    wait(2);
                                    uLCD.locate(2,13);
                                    uLCD.printf("%15S","");
                                } else {
                                    //add release code here
                                }
                                break;
                            case '5': //Up Arrow
                                if (bhit=='1') {
                                    switch(alarmMenuSelect) {
                                        case 0:
                                        //None
                                            break;
                                        case 1:
                                        alarmMinute += 1;
                                            break;
                                        case 2:
                                        alarmHour += 1;
                                            break;
                                        case 3:
                                        speakerVolume += 1;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                break;
                            case '6': //Down Arrow
                                if (bhit=='1') {
                                    switch(alarmMenuSelect) {
                                        case 0:
                                        //None
                                            break;
                                        case 1:
                                        alarmMinute -= 1;
                                            break;
                                        case 2:
                                        alarmHour -= 1;
                                            break;
                                        case 3:
                                        speakerVolume -= 1;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                break;
                            case '7': //Left Arrow
                                if (bhit=='1'){
                                    if(alarmMenuSelect <= 3 && alarmMenuSelect > 0) alarmMenuSelect -= 1;
                                    else alarmMenuSelect = 3;
                                }                                
                                break;
                            case '8': //Right Arrow
                                if (bhit=='1'){
                                    if(alarmMenuSelect < 3 && alarmMenuSelect >= 0) alarmMenuSelect += 1;
                                    else alarmMenuSelect = 0;
                                } 
                                break;
                            default:
                                break;
                        }
                    }
                }
            }   
        }
        lcd_mutex.unlock();
        alarmScale();
        Thread::wait(10);
    }
}


void displayMenu(void const *args){
    while(1){
        lcd_mutex.lock();
        uLCD.locate(2,11);
        uLCD.color(GREEN);
        switch(alarmMenuSelect) {
            case 0:
            uLCD.printf("% 6s", "");
                break;
            case 1:
            uLCD.printf("% 6s", "Min");
                break;
            case 2:
            uLCD.printf("% 6s", "Hour");
                break;
            case 3:
            uLCD.printf("% 6s", "Volume");
                break;
            default:
                break;
        }
        lcd_mutex.unlock();
        Thread::wait(100);
    }
}


// IFTTT Setup
void iftttSetup(){
        // eth.init(); //Use DHCP
    eth.connect();
    TCPSocketConnection socket;

    printf("IFTTT IP Address is %s \r\n", eth.getIPAddress());
    printf("IFTTT is ready \r\n");
}



// Push iftttPB will trigger turn on light
void turnOnLight(void const *args){
    
   char c[50]; //size of the number
   
    while(1){
        lcd_mutex.lock();
        if(iftttPB == 0){
            led4 = 1;
            uLCD.locate(2,13);
            uLCD.color(BLUE);
            // uLCD.printf("LIGHT %07S","Trigger");
            uLCD.printf("%15S","LIGHT Triggered");
            iftttLight.addIngredients("TurnOnLight!","GET1","data1");
            iftttLight.trigger(IFTTT_GET);
            led4 = 0;
            wait(2);
            uLCD.locate(2,13);
            uLCD.printf("%15S","");
        }
        
        if(tempC>27){
            led4 = 1;
            sprintf(c, "%g", tempC);
            uLCD.locate(2,14);
            uLCD.color(RED);
            uLCD.printf("%15S","Temp Warning!!");
            iftttTemp.addIngredients(c,"GET1","data1");
            iftttTemp.trigger(IFTTT_GET);
            lcd_mutex.unlock();
            led4 = 0;
            wait(5);
            lcd_mutex.lock();
            uLCD.locate(2,14);
            uLCD.printf("%15S","");
        }
        
        lcd_mutex.unlock();
        Thread::wait(1000);
    }
}


//display TEST
void displayAlarm(void const *args){
    while(1){
        lcd_mutex.lock();
        uLCD.locate(2,8);
        uLCD.color(WHITE);
        uLCD.printf("Alarm : %02d:%02d", alarmHour, alarmMinute);
        uLCD.locate(2,9);
        if(alarm_enable&&timeCheck) 
            uLCD.printf("Alarm: %3S","ON");
        else
            uLCD.printf("Alarm: %3S","OFF");
        uLCD.locate(2,10);
        uLCD.printf("Volume: %03.0f", speakerVolume);
        lcd_mutex.unlock();
        //Enc_change_ISR();
        Thread::wait(100);
    }
}

int main()
{
    printf("//////start/////\r\n");
    uLCD.baudrate(3000000); //jack up baud rate to max for fast display
    //blue.baud(119200);
    uLCD.cls();
    getTime();
    setupRPG();// run RPG encoder. Inc and Dec, change menu (min, hr, volume)
    pc.baud(9600);
    // iftttSetup();
    eth.connect();
    printf("IP Address is %s \r\n", eth.getIPAddress());
    // Initialize ifttt object, add up to 3 optional values, trigger event.
    


    //Threads
    Thread threadblue(bluetooth);
    Thread threadTemperature(temperature);
    Thread threadTime(displayTime);
    Thread threadAlarmCheck(checkAlarm);
    Thread threadDisplayAlarm(displayAlarm);
    Thread threadSiren(siren);
    Thread threadDisplayMenu(displayMenu);
    Thread threadTurnOnLight(turnOnLight);
    
    //initial setup for LEDs
    led1 = 1;
    led2 = 0;
    while(1){
        led1 = !led1;
        led3 = alarm_enable;
        ctTime = time(NULL);
        currentTime = localtime(&ctTime);
        printf("Temp =%5.2F C \n\r", tempC);
        printf("Time: %02d:%02d:%02d\r\n", currentTime->tm_hour,currentTime->tm_min, currentTime->tm_sec);
        printf("Alarm : %02d:%02d\r\n", alarmHour, alarmMinute);
        printf("Volume: %03.0f\r\n", speakerVolume);
        printf("Menu: %1d\r\n\n", alarmMenuSelect);
        // printf("%02d\r\n",enc_count);
        Thread::wait(1000);
    }
}
