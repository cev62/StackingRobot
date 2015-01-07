import processing.serial.*;
import de.hardcode.jxinput.*;

Serial serial;
//Command command;
boolean up = false, down = false, left = false, right = false, key_change = false;
long timer;
JXInputDevice gamepad;

void setup()
{
 println(Serial.list());
 //serial = new Serial(this, /*"/dev/ttyACM1"*/ "COM9", 9600);
 //command = new Command();
 timer = millis();
 
 println(JXInputManager.getNumberOfDevices());
 
 gamepad = JXInputManager.createKeyboardDevice();
 
 
 println(gamepad.getMaxNumberOfAxes());
 
 //if(JXInputManager.getNumberOfDevices() > 0){
 //  gamepad = JXInputManager.getJXInputDevice(0);
 // }
 
}

void draw()
{
  
}
