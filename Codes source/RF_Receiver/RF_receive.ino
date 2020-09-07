#include <SPI.h>  
#include "RF24.h" 
#include <Servo.h>
/*//////////Arduino UNO//////////////
V+=3.3        GND=GND
CSN=10        CE=9
MOSI=11       SCK=13
              MISO=12
///////////////////////////////////*/

#define enA 7 //gris
#define in1 2 //blue
#define in2 3 //violet
#define enB 8 //blanc
#define in3 4 //noir
#define in4 5 //marron

Servo servoLED;
Servo servoBAC;
RF24 myRadio (9, 10); 
byte addresses[][6] = {"0"};
int angle=10;
int motorSpeedA = 0;
int motorSpeedB = 0;

struct package {
  int id=0;
  int ServoBAC =90;
  float ServoLED = 100;
  int axeX=515;
  int axeY=528;
};

typedef struct package Package;
Package data;

void setup() {
  Serial.begin(115200);
  delay(1000);
////////MOTORS///////////
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);                 
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
///////SERVO//////////////
  servoLED.attach(A0);
  servoLED.write(data.ServoLED);
  servoBAC.attach(6);
  servoBAC.write(data.ServoBAC);
////////RADIO////////////
  myRadio.begin();
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}


void loop()  {
  if ( myRadio.available()) {
    while (myRadio.available()){             
      myRadio.read( &data, sizeof(data) );
    }
    /////SERVO/////////////////////
    servoBAC.write(data.ServoBAC);
    servoLED.write(data.ServoLED);
    //////////////////////////////
    ////////MOTORS////////////////                      
  // Axe X pour aller en av et arr
  if (data.axeY < 470) {
    // backward motor A
    digitalWrite(in1, HIGH);                          
    digitalWrite(in2, LOW);
    // backward motor B
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
    
    int motorSpeedA = map(data.axeY, 470, 0, 130, 255);
    int motorSpeedB = map(data.axeY, 470, 0, 130, 255);
  }

  
 else if (data.axeY > 550) {
    // motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    //motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
    motorSpeedA = map(data.axeY, 550, 1023, 130, 255);
    motorSpeedB = map(data.axeY, 550, 1023, 130, 255);
  }
  // si joystick reste au centre
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

 // X axe pour tourner
  if (data.axeX < 470) {
    int xMapped = map(data.axeX, 470, 0, 130, 255);
    // aller à gauche
    motorSpeedB = motorSpeedB - xMapped;
    motorSpeedA = motorSpeedA + xMapped;
    //limites
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
  }
  
 if (data.axeX > 550) {
    int xMapped = map(data.axeX, 550, 1023, 130, 255);
    // aler à droite
    motorSpeedB = motorSpeedB + xMapped;
    motorSpeedA = motorSpeedA - xMapped;
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
  }


  

  if (motorSpeedA < 50) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 50) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // send PWM signal à motor A
  analogWrite(enB, motorSpeedB); // send PWM signal à motor B
    /////////////////////////////
    Serial.print("\nPackage:");
    Serial.print(data.id);
    Serial.print("\n");
    Serial.print("Servo_bac = "), Serial.println(data.ServoBAC);
    Serial.print("Servo_led = "),Serial.println(data.ServoLED);
    Serial.println("X = "), Serial.println(data.axeX);
    Serial.println("Y = "), Serial.println(data.axeY);

    
  }
}
