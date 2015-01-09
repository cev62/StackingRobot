import processing.serial.*;
import de.hardcode.jxinput.*;
import de.hardcode.jxinput.directinput.*;
import de.hardcode.jxinput.event.*;
import de.hardcode.jxinput.j3d.*;
import de.hardcode.jxinput.j3d.test.*;
import de.hardcode.jxinput.keyboard.*;
import de.hardcode.jxinput.test.*;
import de.hardcode.jxinput.util.*;
import de.hardcode.jxinput.virtual.*;


Serial serial;
long timer;
JXInputDevice gamepad;

float motor_power, turn_power, arm_power;
int motor_data, turn_data, arm_data;


void setup()
{
 println(Serial.list());
 serial = new Serial(this, /*"/dev/ttyACM1"*/ "COM10", 9600);
 timer = millis();
 
 print(JXInputManager.getNumberOfDevices());
 println(" devices");
 
 for(int i = 0; i < JXInputManager.getNumberOfDevices(); i++){
   if(JXInputManager.getJXInputDevice(i).getName().equals("Controller (Gamepad F310)")){
     gamepad = JXInputManager.getJXInputDevice(i);
     break;
   }
 }
 
 if(gamepad == null){
   println("Did not Find Gamepad...");
 }
 
  motor_power = 0.0;
  turn_power = 0.0;
  arm_power = 0.0;
  motor_data = 0;
  turn_data = 0;
  arm_data = 0;
}

void draw()
{
  DirectInputDevice.update();
  if(gamepad != null){
    motor_power = -(float)gamepad.getAxis(1).getValue();
    turn_power = (float)gamepad.getAxis(3).getValue();
    arm_power = (float)gamepad.getAxis(2).getValue();
    
    motor_data = (int)(abs(motor_power) * 31);
    if(motor_power < 0){ motor_data = (int)(motor_data + 32); }
    turn_data = (int)(abs(turn_power) * 31);
    if(turn_power < 0){ turn_data = (int)(turn_data + 32); }
    arm_data = (int)(abs(arm_power) * 31);
    if(arm_power < 0){ arm_data = (int)(arm_data + 32); }
    
    motor_data += 128 + 64;
    turn_data += 64;
    arm_data += 128;
    //motor_data = 2 * motor_data + 128;
    //arm_data = 2 * arm_data;
  }
  
  if(millis() - timer > 50){
    serial.write(motor_data);
    serial.write(turn_data);
    serial.write(arm_data);
    
    timer = millis();
  }
  //println(gamepad.getAxis(1).getValue());

  while(serial.available() > 0){
    print((char)(serial.read()));
  }
}
