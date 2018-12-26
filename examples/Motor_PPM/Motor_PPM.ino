/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo motor_left;  // create servo object to control a servo
Servo motor_right; // create servo object to control a servo
// twelve servo objects can be created on most boards


void setup() {
  Serial.begin(115200);
  motor_left.attach(12);  // attaches the servo on pin 9 to the servo object
  motor_right.attach(14);  // attaches the servo on pin 9 to the servo object
  Serial.println("Start");
}
String serialdata="" ;
String serialdataold="";
void loop() {
  while(Serial.available()>0){
    delay(15);
    serialdata=Serial.readString(); // Read All char inputed
    Serial.println(serialdata);
    serialdata.trim(); //cut string for compare == " "
  }
  if(serialdata!=serialdataold){
    motor_left.writeMicroseconds(serialdata.toInt());
    //motor_left.write(serialdata.toInt());
    serialdataold=serialdata;
  }
}

