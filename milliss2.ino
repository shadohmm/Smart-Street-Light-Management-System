#include "SPI.h"
#include "MFRC522.h"
#include<millisDelay.h>
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

// millis delay
const unsigned long TOTAL_DELAY=20000;
const unsigned long SCAN_AFTER=10000;
unsigned long prevTime=0;
bool ledOn=false;
millisDelay ledDelay;

// ir sensor
const int ProxSensor=2;
int inputVal = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(7, OUTPUT);
  Serial.println("I am waiting for card...");
  ledOn=false;
  
  //ir sensor
  pinMode(7, OUTPUT);          // Pin 7 has an LED connected on most Arduino boards:  
  pinMode(ProxSensor,INPUT);    //Pin 2 is connected to the output of proximity sensor
  Serial.begin(9600);

}

void loop() {
    if (rfid.PICC_IsNewCardPresent()) {  //rfid sensor
      Serial.println("ON");
      digitalWrite(7, HIGH);
      ledOn=true;
      ledDelay.start(TOTAL_DELAY);
    }
    if(ledOn==true || rfid.PICC_IsNewCardPresent())
    {   unsigned long currentTime=millis();
        if(ledDelay.remaining()<=SCAN_AFTER)   //millis delay
        {
         if(rfid.PICC_IsNewCardPresent())
         {
           Serial.println("Time Extended");   
           ledDelay.start(30000);
          }
        }
    
    if(ledDelay.justFinished())   //millis delay
      {
        ledDelay.repeat();
        Serial.println("Turning off LED");
        digitalWrite(7, LOW);
        ledOn=false;   
      }
    }
    //ir sensor  
    if(digitalRead(ProxSensor)==HIGH)      //ir sensor output
    {
      Serial.println("ON");
     digitalWrite(7, HIGH);// set the LED on
     ledDelay.start(TOTAL_DELAY);
     ledOn=true;
    }

}
