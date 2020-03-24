#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4
#define RST_PIN D2
#define LED_ROT D1
#define LED_GRUEN D0

MFRC522 mfrc522(SS_PIN, RST_PIN);
long tagTime = -1;


void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Hallo Alexander");
  tagTime=-1;

  pinMode(LED_ROT,OUTPUT);
  pinMode(LED_GRUEN,OUTPUT);
  digitalWrite(LED_ROT,HIGH);
  digitalWrite(LED_GRUEN,LOW);

}

void loop() {
  if (tagTime > -1) {
    if((millis()-tagTime)>5000){
      tagTime=-1;
      digitalWrite(LED_ROT,HIGH);
      digitalWrite(LED_GRUEN,LOW);  
    }
  } 
    
  
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print("Du bist");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" :" ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      
      tagTime=millis();
      digitalWrite(LED_ROT,LOW);
      digitalWrite(LED_GRUEN,HIGH);
      
      mfrc522.PICC_HaltA();
    }
    
  }

}
