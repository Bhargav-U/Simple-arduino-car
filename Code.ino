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
//Default bluetooth module name is HC-05 with password 1234
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
