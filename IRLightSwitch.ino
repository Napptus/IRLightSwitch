//www.elegoo.com
//2023.05.05

#include "IRremote.h"
#include <Servo.h>

char incomingByte = '0';
int receiver = 11; 
int currentState = 0;
String full = "0";
Servo myservo;


IRrecv irrecv(receiver);     // create instance of 'irrecv'
uint32_t last_decodedRawData = 0;

int translateIR() // takes action based on IR code received
{
  // Check if it is a repeat IR code 
  if (irrecv.decodedIRData.flags)
  {
    //set the current decodedRawData to the last decodedRawData 
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  } else
  {
    //output the IR code on the serial monitor
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xE916FF00: return 0;    break;
    case 0xF30CFF00: return 1;    break;
    case 0xBA45FF00: return 2;     break;
    default:
      Serial.println(" other button");
  }
  
  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500); // do not get immediate repeat
  return 5;
}

void setup()
{
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo.write(75); // reset servo position
  
  Serial.begin(9600);
  Serial.println("Program started.");
  irrecv.enableIRIn(); // Start the receiver

}


void loop()
{
  if (irrecv.decode()) // have we received an IR signal?
  {
    int code = translateIR();
    //code for using 0/1 buttons to turn off/turn on, instead of Power button
    // if(code == 0){
    //   if(currentState != 0){
    //     Serial.println("Off");
    //     myservo.write(110);
    //     delay(1000);
    //     myservo.write(75);
    //     currentState = 0;
    //   }
    // }else if (code == 1) {
    //   if(currentState != 1){
    //     Serial.println("On");
    //     myservo.write(55);
    //     delay(1000);
    //     myservo.write(75);
    //     currentState = 1;
    //   }
    // } else if (code == 2){Serial.println("lol");} 
    // Serial.println(currentState);

    if(code == 2 && currentState == 0) {
        Serial.println("On");
        myservo.write(55);
        delay(1000);
        myservo.write(75);
        currentState = 1;
    } else if (code == 2 && currentState == 1) {
        Serial.println("Off");
        myservo.write(110);
        delay(1000);
        myservo.write(75);
        currentState = 0;
    }
    irrecv.resume(); // receive the next value
  }
}
