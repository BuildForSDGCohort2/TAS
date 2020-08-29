#include <ArduinoJson.h>

int calibrationTime = 5;
long unsigned int lowIn;  
long unsigned int pause = 5000; 
boolean lockLow = true;
boolean takeLowTime; 
int pirPin = 3;    
int ledPin = 13;
int buzzerPin = 5;
String message = "";
bool messageReady = false;

void setup() {
  
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  beep(10);
  delay(10);

  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
  }
  
  Serial.println(" done");
  Serial.println("MOTION SENSOR ACTIVE");
  delay(50);

}

void loop(){

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, message);
  
  while(Serial.available()){
    message = Serial.readString();
    messageReady = true;
  }
    
  if(digitalRead(pirPin) == HIGH){
    digitalWrite(ledPin, HIGH); 
    digitalWrite(buzzerPin, HIGH);
    
    if(messageReady){
          
      if(error){ 
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        messageReady = false;
        return;
      }
              
      if(doc["type"] == "request"){
        doc["type"] = "response";
        doc["motion"] = true;
        serializeJson(doc, Serial);
      }
      else{
        doc["type"] = "response";
        doc["motion"] = false;
        serializeJson(doc, Serial);
      }
             
      if(lockLow){ 
        lockLow = false;
                   
        Serial.println("\n");
        Serial.print("Motion detected at ");
        Serial.print(millis()/1000);
        Serial.println(" sec");
          
        Serial.print("Sound the buzzer ");
             
        beep(20);
        delay(50);
      }
                     
      takeLowTime = true;
           
    }
         
    messageReady = false;
  
    }

    if(digitalRead(pirPin) == LOW){     
      digitalWrite(ledPin, LOW); 
                  
      if(takeLowTime){
        lowIn = millis();
        takeLowTime = false;                
      }
                   
      if(!lockLow && millis() - lowIn > pause){
         
        lockLow = true;     
        Serial.println("\n");                  
        Serial.print("motion ended at ");
        Serial.print((millis() - pause)/1000);
        Serial.println(" sec");
        delay(50);
      }
  
  }

}

void beep(unsigned char delayms) {
      analogWrite(buzzerPin, 10);
      delay(delayms);
      analogWrite(buzzerPin, 0);
      delay(delayms);
}
