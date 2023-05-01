**Team Members:**

Joon Kim

Andrew Kahura


## Project Description

Internet of Things device for controlling various features of a smart home, to be used in a similar way to a Google Home or Alexa. Downscaled feasibly. While navigating this project we referred to the goal we initially established which was to create a device with a multitude of useful functionalities that could be used in a simple way.

## Project Overview
 The features of the home control system are summaries in figure 1. 

 ![image](https://user-images.githubusercontent.com/69119033/235550183-ab697346-c801-4825-880d-dd560d9e29cc.png)



 The user will be able to:
 - Read the current time
 - Set a timer to go off at a specific time both physically and remotely (using a mobile phone)
 - Configure the volume for the timer alarm
 - Turn a distant smart lamp on and off remotely
How the user does the above will be detailed further down this documentation.
## Components Overview: Hardware

The home control system makes use of several hardware I/O components which are all connected to an mbed lpc1768 on a breadboard. These components include:
- TMP36 analog temperature sensor
- Speaker
- Class-D amplifier
- Push buttons [x2]
- RGB Rotatory pulse generator
- uLCD -144-G2
- Adafruit Bluetooth BLE module
- Ethernet breakout board

The user will also need access to a mobile phone to make use of the Bluetooth capabilities. A diagram of the wiring of these components to the mbed is shown in Figure 2. A table of the connections between all of the components and the mbed is in Table 1. Figure 3 shows a photo of the wiring on a breadboard. 

 ![image](https://user-images.githubusercontent.com/69119033/235549317-cba5809e-6db4-47c9-a9ce-2c9815b0b4c9.png)


<p align="center">
  <img src="(https://user-images.githubusercontent.com/69119033/235549339-342674c1-670d-4dae-9f43-e1faf07eeb18.png" />
</p>

## Hardware Connection
![image](https://user-images.githubusercontent.com/70723673/235550512-8e37ae18-f2c5-4165-bc52-42a711d9cf47.png)

| mbed   | uLCD Header | uLCD cable |
|--------|-------------|------------|
| 5V=VU  | 5V          | 5V         |
| Gnd    | Gnd         | Gnd        |
| TX=P28 | RX          | TX         |
| RX=P27 | TX          | RX         |
| P30    | Reset       | Reset      |

<img width="161" alt="image" src="https://user-images.githubusercontent.com/70723673/235550527-40609e27-f93f-4a08-8218-65a2815c9f9d.png">

| Mbed LPC1768 | Magjack adapter  |
|--------------|------------------|
| TD+          | P1               |
| TD-          | P2               |
| RD+          | P7               |
| RD-          | P8               |



## Components Overview: Software

Figure x. is a diagram illustrating the software architecture for the system. Data taken from a time server is used to get the current time. Commands from the BLE mobile app can be used to interact with the device’s features. Triggers raised by the system’s inputs (temperature readings and pressing a Bluetooth and/or physical button) are used to generate software outputs to a user’s mobile phone via a notification. The Bluetooth/physical pushbutton trigger also interacts with a remote desk lamp to turn it on an off. This is done by sending commands through to an online digital automation platform (IFTTT) from the mbed.

 ![image](https://user-images.githubusercontent.com/69119033/235549294-4641af95-8992-4d54-8a7a-fe06ebdc87b2.png)

## Project Demonstration

#### Timer and alarm set up
Using the RGB rotary pulse generator, the user can select whether they would like to change the hour, minute, or volume of the time and alarm. Turning it clockwise increments these three variables and turning it counterclockwise decrements them. A button on the rotary pulse generator is used to confirm each variable selection. The Bluetooth app can also be used to set the timer and alarm in a similar way. The “up” arrow increments the hours/minutes/alarm volume. The “down” arrow decrements them. The user can toggle between the three using the “left” or “right” arrow.

Once the timer and alarm volume are selected it will be displayed on the uLCD screen. LED 3 is also used as a status indicator for the alarm (if the LED is on the alarm set and vice versa).
Once the alarm is set it can be disabled using the second pushbutton or the “1” button on the Bluetooth app.

#### Temperature trigger
If the temperature read by the device goes above a certain value (27 degrees Celsius) a trigger is raised and the user is notified via their mobile phone. 

####  Remote light control
The user can turn off a remote desk lamp using either the first pushbutton or the “2” button on the Bluetooth app. This also sends out a notification on the users phone. 

The video linked below demonstrates these features.
