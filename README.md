<h1 align="center" id="title">Simple-arduino-car</h1>

<p id="description">Putting my basics to test by creating a simple Arduino powered robot that can be autonomous and at the same time controlled using an android app using Bluetooth made using mit app inventor</p>

<p align="center"><img src="https://img.shields.io/badge/adafruit-000000?style=for-the-badge&amp;logo=adafruit&amp;logoColor=white" alt="shields"><img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&amp;logo=Arduino&amp;logoColor=white" alt="shields"><img src="https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&amp;logo=arduino&amp;logoColor=white" alt="shields"></p>

<h2>Reference images</h2>
<h3>Project Images:</h3>

![Project Screenshot 5](https://drive.google.com/uc?id=1M1tNNTqjPdYsG3KQmHrVK-230JKbq4UV)

![Project Screenshot 6](https://drive.google.com/uc?id=1mgv5ilX8CiASfX_bjigh_EVikdrKcBHE)

![Project Screenshot 7](https://drive.google.com/uc?id=1S_CwtH46RTgWwxNFAG72-mvs_5XOAAlr)

![Project Screenshot 8](https://drive.google.com/uc?id=1_PdeunSadO90vp2YqJw_g3jch-g5ZMg7)

<h3>Circuit:</h3>

![Project Screenshot 1](https://drive.google.com/uc?id=1DcQOYJwVBkGs_U8BOB6r2f3HfOlWmW7x)

* use a seperate single power source,dont remove the jumper,use a 2cell and 1cell battery holder in series for a 11.3v ouput 

![Project Screenshot 2](https://drive.google.com/uc?id=188AmnbHYtHaieVy3nS2PRSsQckepm1Eh)

<h3>App:</h3>

![Project Screenshot 3](https://drive.google.com/uc?id=1-D3dTnr2Bp68_p4pt8DXktXqTpqZHaF_)

![Project Screenshot 4](https://drive.google.com/uc?id=1ltYE2ZKSSVRR1k20sHWcidQQAs6Z9Vs7)






  
  
<h2>Features</h2>

Here're some of the project's best features:

*   Ulrasonics object detection and avoidance
*   IR line detection and line following
*   IR edge detection and avoidace
*   Bluetooth control

<h2>Installation Steps:</h2>

<p>Use the zip file and include this as a library</p>

[Adafruit motor sheild library(sourced form arduino official website)](https://drive.google.com/file/d/1fXrFSRek9RJ6NRSuPyDTwnQhqFvIXKbO/view?usp=sharing)

  
  
<h2>Built with</h2>

Technologies used in the project:

*   Arduino
*   Mit app inventor


<h2>Code</h2>

```Arduino
/*
 * Creators :akshay,bhargav,vatsav
 *ardiuno powered smart car
 *motor map
 *L------------R
 *-----front----
 *----sensor----
 *[1]........[2]
 *..............
 *..............
 *..............
 *[3]........[4]
 *-----back-----
 *L------------R
 */
//L2393d motor driver module library
#include<AFMotor.h>
 //defining pins for ultrasonic snesor
#define triggerpin A0
#define echopin A5
//defining pins for ir sensors
#define fright A1
#define fleft A2
//variables that are use in code
int distance = 0; //for ultrasonic snesor
int duration = 0; //for ultrasonic sensor
int start_flag = 0; //for starting the bot,like a key
char bluetooth_data; //for bluetooth communication//configuring motors
AF_DCMotor motor1(3, MOTOR12_1KHZ);
AF_DCMotor motor2(4, MOTOR12_1KHZ);
//to move forward
void forward() {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    // motor3.run(FORWARD);
    // motor4.run(FORWARD);
    Serial.println("forward");
}
//to move backward
void backwards() {
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    // motor3.run(BACKWARD);
    // motor4.run(BACKWARD);
    Serial.println("backward");
}
//to turn left
void left() {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    // motor3.run(BACKWARD);
    // motor4.run(FORWARD);
    Serial.println("left");
} //to turn right
void right() {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    // motor3.run(FORWARD);
    // motor4.run(BACKWARD);
    Serial.println("right");
}
//to stop
void halt() {
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    // motor3.run(RELEASE);
    // motor4.run(RELEASE);
    delay(500);
    Serial.println("stop");
}
//ultrasonic sensor code
void sensor() {
    digitalWrite(triggerpin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerpin, LOW);
    duration = pulseIn(echopin, HIGH);
    distance = duration * 0.034 / 2;
    if (distance < 20) {
        left();
        delay(1000);
    } else {
        forward();
    }
}
//code for edge data of the bot
void edge() {
    /*
     * if edge on fornt,move back until there is no edge and turn left when edge comes
     * if edge on back,move front until no object and no edge,if object or edje present,turn left
     */
    if (digitalRead(fright) == 1 || digitalRead(fleft) == 1) {
        backwards();
        delay(500);
        left();
        delay(500);
    } else {
        Serial.println("no problem");
        sensor();
    }
}
//changing modes for mode1
void change_mode() {
    if (Serial.available() > 0) {
        bluetooth_data = Serial.read();
        switch (bluetooth_data) {
        case 'W':
            mode1();
            break;
        case 'X':
            mode2();
            break;
        case 'Y':
            mode3();
            break;
        case 'Z':
            mode4();
            break;
        default:
            break;
        }
    }
}
//mode1 ------> selfdriving or automatic mode
void mode1() {
    halt();
    while (1 == 1) {
        change_mode();
        edge();
    }
}
//mode2 -----> assisted mode
void mode2() {
    /*
     * follow bluetooth instructions using sensors
     */
    Serial.println("Mode 2");
    halt();
    while (1 == 1) {
        if (Serial.available() > 0) {
            bluetooth_data = Serial.read();
            switch (bluetooth_data) {
            case 'F':
                edge();
                break;
            case 'B':
                backwards();
                break;
            case 'L':
                left();
                break;
            case 'R':
                right();
                break;
            case 'H':
                halt();
                break;
            case 'W':
                mode1();
                break;
            case 'X':
                mode2();
                break;
            case 'Y':
                mode3();
                break;
            case 'Z':
                mode4();
                break;
            }
        }
        if (bluetooth_data == 'F') {
            edge();
        }
    }
}
//mode3 -------> manual mode
void mode3() {
    /*
     *blindly follow bluetooth instructions without using sensors
     */
    Serial.println("Mode 3");
    halt();
    while (1 == 1) {
        if (Serial.available() > 0) {
            bluetooth_data = Serial.read();
            switch (bluetooth_data) {
            case 'F':
                forward();
                break;
            case 'B':
                backwards();
                break;
            case 'L':
                left();
                break;
            case 'R':
                right();
                break;
            case 'H':
                halt();
                break;
            case 'W':
                mode1();
                break;
            case 'X':
                mode2();
                break;
            case 'Y':
                mode3();
                break;
            case 'Z':
                mode4();
                break;
            }
        }
    }
}
//line follwwing robot,we need to move the back side edge sensor to left and front edge sensor should be on right
void mode4() {
    while (1 == 1) {
        while (Serial.available() > 0) {
            change_mode();
        }
        if (digitalRead(fright) == 0 && digitalRead(fleft) == 0) {
            digitalWrite(triggerpin, LOW);
            delayMicroseconds(2);
            digitalWrite(triggerpin, HIGH);
            delayMicroseconds(10);
            digitalWrite(triggerpin, LOW);
            duration = pulseIn(echopin, HIGH);
            distance = duration * 0.034 / 2;
            if (distance < 20) {
                left();
            } else {
                forward();
            }
        }
        if (digitalRead(fright) == 0 && digitalRead(fleft) == 1) //left side there is line,so turn left
        {
            motor1.run(BACKWARD);
            motor2.run(FORWARD);
        }
        if (digitalRead(fright) == 0 && digitalRead(fleft) == 1) //right side there is line, so turn right
        {
            motor2.run(BACKWARD);
            motor1.run(FORWARD);
        }
        if (digitalRead(fright) == 1 && digitalRead(fleft) == 1) {
            sensor();
        }
    }
}
//setup function
void setup() {
    //setting speed of motors to maxiumum
    motor1.setSpeed(255);
    motor2.setSpeed(255);
    //configering ultrasonic sensor pins
    pinMode(triggerpin, OUTPUT);
    pinMode(echopin, INPUT);
    pinMode(fright, INPUT);
    pinMode(fleft, INPUT);
    //serial communication for bluetooth
    Serial.begin(9600);
}
//loop function
void loop() {
    //using start flag as a key and starting the bot with automatic mode.all modes are infinate loop,so loop function is just to start the bot
    if (start_flag == 0) {
        Serial.println("started");
        delay(5000);
        mode1();
        start_flag == 1;
    }
}
```

<h2>components</h2>

[Arduino uno]()

[Motor driver shield]()

[chasis set]()

[Ultrasonic Distance sensor]()

[IR proximity sensor]()

[Bluetooth module]()

[1cell battery holder]()

[2cell battery holder]()

[Batteries]()

[Battery charger]()


