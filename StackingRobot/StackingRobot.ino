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
int arm_power;

// Timers
long int micro_timer, print_timer, watchdog_timer;

 void setup(){
   micro_timer = millis();
   print_timer = millis();
   watchdog_timer = millis();
   
   Serial.begin(9600);
   Serial1.begin(9600);
 }
 
 void loop(){
  
   if(millis() - watchdog_timer > 200)
   {
     is_disabled = true;
   }
   
   ReceiveComm();
   SendComm();
   Actuate();
   
 }
 
 void ReceiveComm(){
   while(Serial.available() > 1){
     int motor_data = Serial.read();
     int arm_data = Serial.read();
     
     // Decode the command
     motor_power = motor_data & 63;
     arm_power = arm_data & 63;
     if(motor_data & 64) { motor_power = -motor_power; }
     if(arm_data & 64) { arm_power = -arm_power; }
     
     // Received data, so feed the watchdog
     is_disabled = 1;
     watchdog_timer = millis();
   }
 }
 
 void SendComm(){
   
 }
 
 void Actuate(){
  
   if(is_disabled){
     motor_power = 0;
     arm_power = 0;
   }
   
   SendMicroCommand();
   
 }
 
 void SendMicroCommand(){
  if(millis() - micro_timer < 50)
  {
    // Not time to send a new command yet
    return; 
  }  
  micro_timer = millis();
  
  byte motor_data = abs(motor_power);
  if(motor_power < 0) { motor_data |= 64; }
  byte arm_data = abs(arm_power);
  if(arm_power < 0) { arm_data |= 64; }
  
  Serial1.write(224);
  Serial1.write(motor_data);
  Serial1.write(arm_data);
  Serial1.write(192);
 }
 
 int Read2CByte(int input){
   return 0;
 }
 
 int Write2CByte(int input){
   return 0;
 }
 
 
