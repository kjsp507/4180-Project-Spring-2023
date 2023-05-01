


**Team Members:**

Joon Kim

Andrew Kahura


### Project Overview
 The features of the home control system are summaries in figure 1. 

 ADD FIGURE 1

 The user will be able to:
 - Read the current time
 - Set a timer to go off at a specific time both physically and remotely (using a mobile phone)
 - Configure the volume for the timer alarm
 - Turn a distant smart lamp on and off remotely
How the user does the above will be detailed further down this documentation.


### Components overview
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
## Project Description

Internet of Things device for controlling various features of a smart home, to be used in a similar way to a Google Home or Alexa, and downscaled feasibly. While navigating this project we referred to the goal we initially established, which was to create a device with a multitude of useful functionalities that could be used in a simple way.
