#include <Servo.h>

// NFR required libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// list of pins
// 2,3,4  >> left Sensors
// 9,10, 14 >> Right Sensors
// 5,6 >> Servo , Esc
// 7,8 CE, CSN (NFR)
// 11 >> MOSI, 12 >> MSIO, 13 >>SCK

// NFR signal variables
RF24 radio(7,8); // CE, CSN
const byte address[6] = "00001"; // address required to communicate with the transmitter 
// int CurrentTrafficLight; // red:1, Yellow:2, green:3

//motor pins
#define SERVO_PIN 5
#define ESC_PIN 6

//IR sensor pins
// Left Sensors
#define LEFT_OUTER_SENSOR  2
#define LEFT_CENTER_SENSOR 3
#define LEFT_SENSOR 4

// Right Sensors
// static const uint8_t A0 = 14; 
#define RIGHT_SENSOR 9
#define RIGHT_CENTER_SENSOR 10
#define RIGHT_OUTER_SENSOR  A0

// Steering Values
#define LEFT_HARD_STEER 130
#define LEFT_STEER 123 // 110
#define CENTER_STEER 100
#define RIGHT_STEER 75 // 90
#define RIGHT_HARD_STEER 60

// motor moving values
#define MIN_PULSE_LENGTH 1200
#define MAX_PULSE_LENGTH 1800
#define MOVE_FORWARD 1379
#define STOP 1500

int motor_forward = 1290;

//defining Servo objects for controlling motors
Servo esc;
Servo servo;

// #########################
// Direction Methods
// #########################

void steerLeft() {
  servo.write(LEFT_STEER);
}

void steerRight() {
  servo.write(RIGHT_STEER);
}
void steerCenter() {
  servo.write(CENTER_STEER);
}

// #########################
// Motion Methods
// #########################

//function for advancing
void goForwards() {
  // esc.write(motor_forward);
  esc.writeMicroseconds(MOVE_FORWARD);
}

//function for stopping
void stop() {
  esc.writeMicroseconds(STOP);
}

//#########################
// DIRECTIONS
// #########################

//calculate in which direction to steer the wheels
int calculateDirection() {
  int left =   digitalRead(LEFT_OUTER_SENSOR) + digitalRead(LEFT_CENTER_SENSOR) + digitalRead(LEFT_SENSOR);
  int right =   digitalRead(RIGHT_OUTER_SENSOR) + digitalRead(RIGHT_CENTER_SENSOR) + digitalRead(RIGHT_SENSOR);

 if (left > right) {
    steerLeft();
  }
  else if (left < right) {
    steerRight();
  }
  else  {
    steerCenter();
  } 
}


// #########################
// NFR Signal Handling
// #########################

void NFRSetup(){
  // will work as a listener
  radio.begin();
  radio.openReadingPipe(0,address); // define this module as a reciever which gets data from this address
  radio.setPALevel(RF24_PA_MIN); // set power Ampilifier level to min
  radio.startListening(); 
}

void TrafficSignalHandler(){
 if(radio.available()){
   int CurrentTrafficLight;
    radio.read(&CurrentTrafficLight,sizeof(CurrentTrafficLight));
    if(CurrentTrafficLight == 1){ // Red Ampel
      Serial.println("Stop");
      stop();
    }
    if(CurrentTrafficLight == 3){ // Green Ampel
      Serial.println("Move Again");
      goForwards();
    }
  }
}

  
void ESC_Calibration(){

  delay(1500);

  esc.writeMicroseconds(1400);
  delay(200);

  esc.writeMicroseconds(STOP);
  delay(1500);
}

void setup() {
  // servo.attach(SERVO_PIN,972,2188);
  servo.attach(SERVO_PIN);
  esc.attach(ESC_PIN,MIN_PULSE_LENGTH,MAX_PULSE_LENGTH);

  pinMode(LEFT_OUTER_SENSOR, INPUT);
  pinMode(LEFT_CENTER_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);

  pinMode(RIGHT_OUTER_SENSOR, INPUT);
  pinMode(RIGHT_CENTER_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  ESC_Calibration();
  NFRSetup();

  Serial.begin(9600);
  steerCenter();
}

//#########################
// Main Code
// #########################
void followLine(){
    goForwards();
    calculateDirection();
}

void loop() {
    goForwards(); // Moving Forward
    calculateDirection();  // Handling Direction Changing
    TrafficSignalHandler(); // Getting Current Traffic Signal
}
