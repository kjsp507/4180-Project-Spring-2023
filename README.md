


# **Project: Smart Home Control**

<img width="700" src="https://user-images.githubusercontent.com/70723673/235605315-0ffa1f89-26ff-4e4e-978d-a9cfa78427da.jpg"/>


## **Team Members:**

Joonseo Kim

Andrew Kahura

## Table of Contents

- [I. Project Description](#i-project-description)
- [II .Project Overview](#ii-project-overview)
- [III. Components Overview: Hardware](#iii-components-overview-hardware)
- [IV. Hardware Description](#iv-hardware-description)
- [V. Components Overview: Software](#v-components-overview-software)
- [VI. Project Demonstration](#vi-project-demonstration)
- [VII. Home Control System Code](#vii-home-control-system-code)
- [VIII. Presentation Slide File](#viii-presentation-slide-file)


## I. Project Description

Internet of Things device for controlling various features of a smart home, to be used in a similar way to a Google Home or Alexa. Downscaled feasibly. While navigating this project we referred to the goal we initially established which was to create a device with a multitude of useful functionalities that could be used in a simple way.

## II .Project Overview
 The features of the home control system are summaries in figure 1. 

![image](https://user-images.githubusercontent.com/69119033/235550817-3aff8e0c-e2a8-4899-985c-7acbc40d6e28.png)


**Figure 1.** Feature Overview



 The user will be able to:
 - Read the current time
 - Set a timer to go off at a specific time both physically and remotely (using a mobile phone)
 - Configure the volume for the timer alarm
 - Turn a distant smart lamp on and off remotely
How the user does the above will be detailed further down this documentation.


## III. Components Overview: Hardware

The home control system makes use of several hardware I/O components which are all connected to an mbed lpc1768 on a breadboard. These components include:
- TMP36 analog temperature sensor
- Speaker
- Class-D amplifier
- Push buttons [x2]
- RPG Rotatory pulse generator
- uLCD -144-G2
- Adafruit Bluetooth BLE module
- Ethernet breakout board

The user will also need access to a mobile phone to make use of the Bluetooth capabilities. A diagram of the wiring of these components to the mbed is shown in Figure 2. A table of the connections between all of the components and the mbed is in Section II. Figure 3 shows a photo of the wiring on a breadboard. The reason added push buttons and RPG, wired devices other than wireless control give redundancy to control the mbed when the wireless device is not operating.

![image](https://user-images.githubusercontent.com/69119033/235551094-ae4615d3-d4b7-4230-b529-5e49da16d60e.png)


**Figure 2.** Hardware Schematic


 <img src="https://user-images.githubusercontent.com/69119033/235549339-342674c1-670d-4dae-9f43-e1faf07eeb18.png" width = "600"/>
 
 
 **Figure 3.** Wiring on breadboard


## IV. Hardware Description.


### uLCD-144-G2
<img src="https://os.mbed.com/media/uploads/4180_1/8185.png" width="200"/>


|  **Mbed LPC1768** | uLCD Header | uLCD cable |
|:--------:|:-------------:|:------------:|
| 5V=VU  | 5V          | 5V         |
| Gnd    | Gnd         | Gnd        |
| TX=P28 | RX          | TX         |
| RX=P27 | TX          | RX         |
| P30    | Reset       | Reset      |

Using uLCD to provide a visual representation of the system's features and information.


### Ethernet Breakout Board
<img  alt="image" src="https://user-images.githubusercontent.com/70723673/235550527-40609e27-f93f-4a08-8218-65a2815c9f9d.png" width="150">

| **Mbed LPC1768** | **Magjack adapter** |
|:------------------:|:----------------------:|
| TD+              | P1                   |
| TD-              | P2                   |
| RD+              | P7                   |
| RD-              | P8                   |

Mbed has  built-in Ethernet controller and physical layer driver chip for internet connection, but it require magjact adaptor. 
Using Ethernet cable (RJ45) to get connection for NTP for getting current time and sending HTTP GET web request using TCP socket for IFTTT IoT control.
Resister device with MAC address is required if using Campus Network. 

### Speaker & Class-D Amp
<img width="300" src="https://user-images.githubusercontent.com/69119033/235577054-4750c7be-b964-4319-9d6b-cb18992bdaaa.png"/>


| **Mbed LPC1768**| **TPA2005D1**     | **Speaker** |
|:----------:|:-------------------:|:-------------:|
| gnd      | pwr - (gnd), in - |             |
| Vout     | pwr +             |             |
| p21      | in +              |             |
|          | out +             | +           |
|          | out -             | -           |

Class-D amplifiers provide powerful sound for speakers by using audio signals from mbed.
Additional POT provides easy and direct control of volume.

### TMP 36 Analog Temperature Sensor
<img src = "https://user-images.githubusercontent.com/69119033/235578489-392b36e0-cb8c-43a5-96dd-44d02887c632.png" width = "150"/>


| **Mbed LPC1768**| LM61 (or TMP36) |
|:----------:|:---------------:|
| GND        | GND             |
| Vout(3.3V) | Vs              |
| p15        | Vout            |

To get the right temperature in Celsius when using an analog temperature sensor, it is necessary to convert the sensor reading.
```c
((_pin.read()*3.3)-0.500)*100.0; //using 3.3V
```
Check this [CookBook](https://os.mbed.com/users/4180_1/notebook/lm61-analog-temperature-sensor/) for more information.

### Pushbuttons
<img src="https://user-images.githubusercontent.com/69119033/235581016-0955c4b1-2368-46fb-ac28-8d1ff2b4f542.png" width ="150" />

| **Mbed LPC1768**| Pushbutton 1 | Pushbutton 2 |
|:--------------:|:--------------:|:--------------:|
| gnd          | gnd          | gnd          |
| pin22        | Out          |              |
| pin23        |              | Out          |

Using mbed internal pullup resistor. Connect ground and mbed directly without physical resistor.
Push button 1 (p23) used to trigger turning on light using IFTTT.
Push button 2 (p22) used to turn on and off alarm. 


### Adafruit Bluetooth module
<img src ="https://user-images.githubusercontent.com/69119033/235577235-f549b4bd-98d4-42f0-85b4-f19d589d8052.png" width = "250"/>


|       Mbed LPC1768      |  Adafruit BLE |
|:---------------:|:-------------:|
| gnd             | gnd           |
| VU(5v)          | Vin (3.3-16V) |
| nc              | RTS           |
| Gnd             | CTS           |
| p9 (Serial RX) | TXO            |
| p10 (Serial TX) | RXI           |




Using Bluetooth, the mbed can be controlled wirelessly. 
The Bluefruit Connect app is utilized to establish a connection and send control signals to the mbed. Blue light from Bluetooth module indicated its connected.


### RBG Rotary Pulse Generator
<img src = "https://user-images.githubusercontent.com/69119033/235577871-181157ae-e974-4b91-a801-67a5056cd895.png" width = "200"/>


| Mbed LPC1768 | RPG breakout         |
|--------------|----------------------|
| gnd          | C encoder common     |
| Vout 3.3     | + LED+ for RGB LED   |
| p16          | A encoder output bit |
| p17          | B encoder output bit |
| p18          | SW pushbutton        |

<img src = "https://os.mbed.com/media/uploads/4180_1/rpgtt.png" width = "300"/>
<img src = "https://os.mbed.com/media/uploads/4180_1/incremental_directional_encoder.gif" width = "300"/>


``` c
const  int lookup_table[] = {0,1,-1,0,0,0,0,0,0,0,0,0,0,1,-1,0};
```
An interrupt routine is called whenever one of the two bits changes state and the truth table in an array is checked to determine how to change the count (i.e.+1, -1, 0). The truth table and more information can be found  from [here](https://os.mbed.com/users/4180_1/code/RPG_demo//file/5c21ef62c975/main.cpp/).  Modified middle of table becuase each turn of RPG is give four changes of  two bits. 
``` c
PinDetect  RPG_A(p16,PullUp);//encoder A and B pins/bits use interrupts
PinDetect  RPG_B(p17,PullUp);
PinDetect  RPG_PB(p18); //encode pushbutton switch "SW" on PCB
// generate an interrupt on any change in either encoder bit (A or B)
RPG_A.attach_deasserted(&Enc_change_ISR);
RPG_B.attach_deasserted(&Enc_change_ISR);
```
Used the PinDetect library for the interrupt routine, and all pins used the mbed internal pull-up resistor with PullUp mode. 
The clockwise direction increments the selected menu (minutes, hours, and volume), while the counterclockwise direction decrements it. Pressing the center push button changes the current menu.

### Sonoff basicr2
<img width="200" src="https://user-images.githubusercontent.com/70723673/235715948-80b282dd-ec8c-489a-bae3-38e33863f12a.png"/>

To add more, using Sonoff basicr2 wifi wireless smart switch to control desk lamp.


## V. Components Overview: Software

Figure 4. is a diagram illustrating the software architecture for the system. 

Data taken from a time server (pool.ntp.org), and used NTP (Network Time Protocal) to get the current time.  Using mbed's built-in  Ethernet controller and physical layer driver chip for internet connection. 

Commands from the BLE mobile app can be used to interact with the device’s features. BLE using serial to communicate with mbed. In case losing wireless BLE connection, mbed also can be control with hard wired RPG and Pushbuttons.

Triggers raised by the system’s inputs (temperature readings and pressing a Bluetooth and/or physical button) are used to generate  **HTTP GET web request** to trigger push to a user’s mobile phone via a notification. The Bluetooth/physical pushbutton trigger also interacts with an WiFi smart swtich remote desk lamp to turn it on an off. This is done by sending commands through to an online digital automation platform (IFTTT) from the mbed. The Mbed sends a HTTP GET web request to trigger an event with an Event that has up to 3 JSON values using a TCP socket to IFTTT. 

After temperature reach certain temperature, mbed will send HTTP web request to IFTTT server with event name "tempWarning" with current temperature JSON value . It will trigger to send notification to user's mobile phone.
When user pressing a Bluetooth and/or physical button, mbed send turn on light trigger to IFTTT server. It sends HTTP GET web request with event name "turnOnLight". After that, IFTTT server will send other web request to WiFi smart swtich which control the desk lamp. Then, smart switch will turn on/off the relay inside of board.  

![image](https://user-images.githubusercontent.com/70723673/235715536-a0114115-be8c-4341-812a-18725f609a20.png)


**Figure 4.** Software architecture

## VI. Project Demonstration

### Timer and alarm set up
Using the RPG rotary pulse generator, the user can select whether they would like to change the hour, minute, or volume of the time and alarm. Turning it clockwise increments these three variables and turning it counterclockwise decrements them. A button on the rotary pulse generator is used to confirm each variable selection. The Bluetooth app can also be used to set the timer and alarm in a similar way. The “up” arrow increments the hours/minutes/alarm volume. The “down” arrow decrements them. The user can toggle between the three using the “left” or “right” arrow.

Once the timer and alarm volume are selected it will be displayed on the uLCD screen. LED 3 is also used as a status indicator for the alarm (if the LED is on the alarm set and vice versa).
Once the alarm is set it can be disabled using the second pushbutton or the “1” button on the Bluetooth app.

| **Button** | **Action**    |
|:----------:|:-------------:|
| 1          | On/Off Alarm  |
| 2          | Turn on light |
| Up         | Increment     |
| Down       | Decrement     |
| Left       | Next Menu     |
| Right      | Previous Menu |


### Temperature trigger
If the temperature read by the device goes above a certain value (27 degrees Celsius) a trigger is raised and the user is notified via their mobile phone. 

Can be tested fuction with curl command
```
curl -X POST -H "Content-Type: application/json" -d '{"value1":"28.23"}' https://maker.ifttt.com/trigger/tempWarning/with/key/PutYourKeyHere
```

###  Remote light control

<img width="500" src="https://user-images.githubusercontent.com/70723673/235715948-80b282dd-ec8c-489a-bae3-38e33863f12a.png"/>


<img width="500" src="https://user-images.githubusercontent.com/70723673/235716810-dacdf48c-e7ac-4985-9a9e-7a8cb091ee5f.jpg"/>

Using WiFi smart switch to turn On/Off the desk lamp. Smart switch is also connced to IFTTT server. 
The user can turn on a IoT remote desk lamp using either the first pushbutton or the “2” button on the Bluetooth app. This also sends out a notification on the users phone. Users can control desk lamp wirelessly. 

Also can be tested with curl command 

```
curl -X POST -H "Content-Type: application/json" -d '{"value1":"TRUE"}' https://maker.ifttt.com/trigger/turnOnLight/with/key/PutYourKeyHere
```

The video linked below demonstrates these features.

Click on the image to go to YouTube. or click this [link](https://youtu.be/J63613xiZbI).
[![Demo](https://user-images.githubusercontent.com/70723673/235655051-a7649578-0bc3-40a4-9295-f6108aff9cbb.jpg)](https://youtu.be/J63613xiZbI)




## VII. Home Control System Code

The code for the system can be in [main](https://github.com/kjsp507/4180-Project-Spring-2023/blob/main/main.cpp). Relevant libraries are also included in the repository.

## VIII. Presentation Slide File
The presentation slide file can be find in [here](https://github.com/kjsp507/4180-Project-Spring-2023/blob/main/ECE%204180%20Project%20Presentation.pptx).
