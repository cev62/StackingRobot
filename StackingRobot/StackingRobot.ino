#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>

void ReceiveComm();
void SendComm();
void Actuate();
void SendMicroCommand();
int Read2CByte(int input);
int Write2CByte(int input);

// State Variables
int is_disabled;
int motor_power;
int turn_power;
int arm_power;

// Servos
Servo *left, *right;

// Timers
long int micro_timer, print_timer, watchdog_timer;

SoftwareSerial blue_serial(9, 10);

 void setup(){
   micro_timer = millis();
   print_timer = millis();
   watchdog_timer = millis();
   
   Serial.begin(9600);
   Serial1.begin(9600);
   blue_serial.begin(9600);
   
   left = new Servo();
   right = new Servo();
   left->attach(3);
   right->attach(4);
   
 }
 
 void loop(){
  
   if(millis() - watchdog_timer > 200)
   {
     is_disabled = 0;
   }
   
   ReceiveComm();
   
   if(millis() - print_timer > 100){
     SendComm();
     print_timer = millis();
   }
   
   Actuate();
   
   delay(10);
   
 }
 
 void ReceiveComm(){
   if(Serial.available() > 0){
     int data = Serial.read();
     delay(1);
     
     // Decode the command
     if((data & 128) && (data & 64)){
       // Motor Command
       motor_power = data & 31;
       if(data & 32){ motor_power = -motor_power; }
     }
     if(!(data & 128) && (data & 64)){
       // Turn Command
       turn_power = data & 31;
       if(data & 32){ turn_power = -turn_power; }
     }
     if((data & 128) && !(data & 64)){
       // Arm Command
       arm_power = data & 31;
       if(data & 32){ arm_power = -arm_power; }
     }
     
     // Received data, so feed the watchdog
     is_disabled = 0;
     watchdog_timer = millis();
   }
   
   if(blue_serial.available() > 0){
     int data = blue_serial.read();
     delay(1);
     
     // Decode the command
     if((data & 128) && (data & 64)){
       // Motor Command
       motor_power = data & 31;
       if(data & 32){ motor_power = -motor_power; }
     }
     if(!(data & 128) && (data & 64)){
       // Turn Command
       turn_power = data & 31;
       if(data & 32){ turn_power = -turn_power; }
     }
     if((data & 128) && !(data & 64)){
       // Arm Command
       arm_power = data & 31;
       if(data & 32){ arm_power = -arm_power; }
     }
     
     // Received data, so feed the watchdog
     is_disabled = 0;
     watchdog_timer = millis();
   }
 }
 
 void SendComm(){
   Serial.print("Motor: ");
   Serial.println(motor_power);
   Serial.print("Turn: ");
   Serial.println(turn_power);
   Serial.print("Arm: ");
   Serial.println(arm_power);
   Serial.print("Buffer: ");
   Serial.println(blue_serial.available());
   if(motor_power != 0){
   blue_serial.print("Motor: ");
   blue_serial.println(motor_power);}
   if(turn_power != 0){
   blue_serial.print("Turn: ");
   blue_serial.println(turn_power);}
   if(arm_power != 0){
   blue_serial.print("Arm: ");
   blue_serial.println(arm_power);}
   blue_serial.print("Buffer: ");
   blue_serial.println(blue_serial.available());
 }
 
 void Actuate(){
  
   if(is_disabled){
     motor_power = 0;
     arm_power = 0;
     turn_power = 0;
   }
   
   SendMicroCommand();
   
   left->write(90 + (int)(turn_power * 60.0 / 31.0));
   right->write(90 + (int)(turn_power * 60.0 / 31.0));
   
 }
 
 void SendMicroCommand(){
  if(millis() - micro_timer < 50)
  {
    // Not time to send a new command yet
    return; 
  }  
  micro_timer = millis();
  
  int motor_data = abs(2*motor_power);
  if(motor_power < 0) { motor_data |= 64; }
  int arm_data = abs(2*arm_power);
  if(arm_power < 0) { arm_data |= 64; }
  
  motor_data += 128;
  
  Serial1.write(motor_data);
  Serial1.write(arm_data);
 }
 
 int Read2CByte(int input){
   return 0;
 }
 
 int Write2CByte(int input){
   return 0;
 }
 
 
