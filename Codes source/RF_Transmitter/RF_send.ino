//#include <Esplora.h>

#include <SPI.h>  
#include "RF24.h"

/*//////////Arduino UNO//////////////
V+=3.3        GND=GND
CSN=10        CE=9
MOSI=11       SCK=13
              MISO=12
///////////////////////////////////*/

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};
int buttonSERVObac=4;
int buttonSERVOled=3;
int a=-1;
int b=-1;

struct package {
  int id=0;
  int ServoBAC =90;
  float ServoLED = 100;
  int axeX=515;
  int axeY=528;
};


typedef struct package Package;
Package data; //data est un package


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openWritingPipe(addresses[0]);
  
  pinMode(buttonSERVOled, INPUT);
  digitalWrite(buttonSERVOled, HIGH);
}

void loop() {
  myRadio.write(&data, sizeof(data)); 
  data.id = data.id + 1;
 /////SERVO_led/////////////////////
  if (digitalRead(buttonSERVObac) == HIGH){
    a=a*(-1);
  }
  switch (a) {
    case -1 :
      data.ServoBAC=90;
    break;
    case 1 :
      data.ServoBAC=130;
    break;
  }
  ///////////SERVO_led//////////////////
    if (digitalRead(buttonSERVOled) == 0){
    b=b*(-1);
  }
  switch (b) {
    case -1 :
      data.ServoLED=100;
    break;
    case 1 :
      data.ServoLED=46.3;
    break;
  }
  ///////MOTORS////////////////
  data.axeX = analogRead(A0); // axe X
  data.axeY = analogRead(A1); // axe Y
  ////////////////////////////
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\nServo_bac = "), Serial.println(data.ServoBAC);
    Serial.print("Servo_led = "),Serial.println(data.ServoLED);
    Serial.print("X = "), Serial.println(data.axeX);
    Serial.print("Y = "), Serial.println(data.axeY);
  
  delay(130);
}
