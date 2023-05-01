**Team Members:**

Joon Kim

Andrew Kahura


## Project Description

Internet of Things device for controlling various features of a smart home, to be used in a similar way to a Google Home or Alexa, and downscaled feasibly. While navigating this project we referred to the goal we initially established, which was to create a device with a multitude of useful functionalities that could be used in a simple way.

## Project Overview
 The features of the home control system are summaries in figure 1. 

 ADD FIGURE 1

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
## Components Overview: Software

Figure x. is a diagram illustrating the software architecture for the system. Data taken from a time server is used to get the current time. Commands from the BLE mobile app can be used to interact with the device’s features. Triggers raised by the system’s inputs (temperature readings and pressing a Bluetooth and/or physical button) are software outputs to a user’s mobile phone via a notification. The Bluetooth/physical pushbutton trigger also interacts with a remote desk lamp to turn it on an off. This is done by sending commands through to an online digital automation platform (IFTTT) from the mbed.
## Project Demonstration

#### Timer and alarm set up
Using the RGB rotary pulse generator, the user can select whether they would like to change the hour, minute, or volume of the time and alarm. Turning it clockwise increments these three variables and turning it counterclockwise decrements them. A button on the rotary pulse generator is used to confirm each variable selection. The Bluetooth app can also be used to set the timer and alarm in a similar way. The “up” arrow increments the hours/minutes/alarm volume. The “down” arrow decrements them. The user can toggle between the three using the “left” or “right” arrow.

Once the timer and alarm volume are selected it will be displayed on the uLCD screen. LED 3 is also used as a status indicator for the alarm (if the LED is on the alarm set and vice versa).
Once the alarm is set it can be disabled using the second pushbutton or the “1” button on the Bluetooth app.

#### Temperature trigger
If the temperature read by the device goes above a certain value (27 degrees Celsius) a trigger is raised and the user is notified via their mobile phone. 

####  Remote light control
The user can turn off a remote desk lamp using either the first pushbutton or the “2” button on the Bluetooth app. This also sends out a notification on the users phone. 
