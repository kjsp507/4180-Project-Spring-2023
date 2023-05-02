**Team Members:**

Joon Kim

Andrew Kahura

- [I. Project Description](#i-project-description)
- [II .Project Overview](#ii-project-overview)
- [III. Components Overview: Hardware](#iii-components-overview--hardware)
- [IV. Hardware Connection](#iv-hardware-connection)
- [V. Components Overview: Software](#v-components-overview--software)
- [VI. Project Demonstration](#vi-project-demonstration)
- [VII. Home Control System Code](#vii-home-control-system-code)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

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
- RGB Rotatory pulse generator
- uLCD -144-G2
- Adafruit Bluetooth BLE module
- Ethernet breakout board

The user will also need access to a mobile phone to make use of the Bluetooth capabilities. A diagram of the wiring of these components to the mbed is shown in Figure 2. A table of the connections between all of the components and the mbed is in Section II. Figure 3 shows a photo of the wiring on a breadboard. 

![image](https://user-images.githubusercontent.com/69119033/235551094-ae4615d3-d4b7-4230-b529-5e49da16d60e.png)


**Figure 2.** Hardware Schematic


 <img src="https://user-images.githubusercontent.com/69119033/235549339-342674c1-670d-4dae-9f43-e1faf07eeb18.png" width = "600"/>
 
 
 **Figure 3.** Wiring on breadboard


## IV. Hardware Connection


#### uLCD-144-G2
<img src="https://os.mbed.com/media/uploads/4180_1/8185.png" width="200"/>


|  **Mbed LPC1768** | uLCD Header | uLCD cable |
|--------|-------------|------------|
| 5V=VU  | 5V          | 5V         |
| Gnd    | Gnd         | Gnd        |
| TX=P28 | RX          | TX         |
| RX=P27 | TX          | RX         |
| P30    | Reset       | Reset      |


#### Ethernet Breakout Board
<img  alt="image" src="https://user-images.githubusercontent.com/70723673/235550527-40609e27-f93f-4a08-8218-65a2815c9f9d.png" width="150">

| **Mbed LPC1768** | **Magjack adapter** |
|------------------|----------------------|
| TD+              | P1                   |
| TD-              | P2                   |
| RD+              | P7                   |
| RD-              | P8                   |

#### Speaker & Class-D Amp
<img width="300" src="https://user-images.githubusercontent.com/69119033/235577054-4750c7be-b964-4319-9d6b-cb18992bdaaa.png"/>


| **Mbed** | **TPA2005D1**     | **Speaker** |
|----------|-------------------|-------------|
| gnd      | pwr - (gnd), in - |             |
| Vout     | pwr +             |             |
| p21      | in +              |             |
|          | out +             | +           |
|          | out -             | -           |

#### TMP 36 Analog Temperature Sensor
<img src = "https://user-images.githubusercontent.com/69119033/235578489-392b36e0-cb8c-43a5-96dd-44d02887c632.png" width = "150"/>


|    mbed    | LM61 (or TMP36) |
|:----------:|:---------------:|
| GND        | GND             |
| Vout(3.3V) | Vs              |
| p15        | Vout            |

### Pushbuttons

| mbed         | Pushbutton 1 | Pushbutton 2 |
|--------------|--------------|--------------|
| gnd          | gnd          | gnd          |
| pin22        | Out          |              |
| pin23        |              | Out          |
<img src="https://user-images.githubusercontent.com/69119033/235581016-0955c4b1-2368-46fb-ac28-8d1ff2b4f542.png" width ="150" />


#### Adafruit Bluetooth module
<img src ="https://user-images.githubusercontent.com/69119033/235577235-f549b4bd-98d4-42f0-85b4-f19d589d8052.png" width = "250"/>


|       Mbed LPC1768      |  Adafruit BLE |
|:---------------:|:-------------:|
| gnd             | gnd           |
| VU(5v)          | Vin (3.3-16V) |
| nc              | RTS           |
| Gnd             | CTS           |
| p9 (Serial RX) | TXO            |
| p10 (Serial TX) | RXI           |


#### RBG Rotary Pulse Generator
<img src = "https://user-images.githubusercontent.com/69119033/235577871-181157ae-e974-4b91-a801-67a5056cd895.png" width = "150"/>


| mbed LPC1768 | RPG breakout         |
|--------------|----------------------|
| gnd          | C encoder common     |
| Vout 3.3     | + LED+ for RGB LED   |
| p16          | A encoder output bit |
| p17          | B encoder output bit |
| p18          | SW pushbutton        |


## V. Components Overview: Software

Figure x. is a diagram illustrating the software architecture for the system. Data taken from a time server is used to get the current time. Commands from the BLE mobile app can be used to interact with the device’s features. Triggers raised by the system’s inputs (temperature readings and pressing a Bluetooth and/or physical button) are used to generate software outputs to a user’s mobile phone via a notification. The Bluetooth/physical pushbutton trigger also interacts with a remote desk lamp to turn it on an off. This is done by sending commands through to an online digital automation platform (IFTTT) from the mbed.

![image](https://user-images.githubusercontent.com/69119033/235551449-2d7c71c4-41d7-4670-a425-f3f64c2e2977.png)


**Figure 4.** Software architecture

## VI. Project Demonstration

#### Timer and alarm set up
Using the RGB rotary pulse generator, the user can select whether they would like to change the hour, minute, or volume of the time and alarm. Turning it clockwise increments these three variables and turning it counterclockwise decrements them. A button on the rotary pulse generator is used to confirm each variable selection. The Bluetooth app can also be used to set the timer and alarm in a similar way. The “up” arrow increments the hours/minutes/alarm volume. The “down” arrow decrements them. The user can toggle between the three using the “left” or “right” arrow.

Once the timer and alarm volume are selected it will be displayed on the uLCD screen. LED 3 is also used as a status indicator for the alarm (if the LED is on the alarm set and vice versa).
Once the alarm is set it can be disabled using the second pushbutton or the “1” button on the Bluetooth app.

#### Temperature trigger
If the temperature read by the device goes above a certain value (27 degrees Celsius) a trigger is raised and the user is notified via their mobile phone. 

####  Remote light control
The user can turn off a remote desk lamp using either the first pushbutton or the “2” button on the Bluetooth app. This also sends out a notification on the users phone. 

The video linked below demonstrates these features.


## VII. Home Control System Code

The code for the system can be in [main](https://github.com/kjsp507/4180-Project-Spring-2023/blob/main/main.cpp). Relevant libraries are also included in the repository.
