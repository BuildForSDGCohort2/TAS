#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer server;
char* ssid = "JUDGE";
char* password = "&N0v20#1";

void setup(){
  
  WiFi.begin(ssid,password);
  Serial.begin(9600);
  
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",handleIndex);
  /*server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);*/
  
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(){
  
  server.handleClient();
  
}

void handleIndex(){

  DynamicJsonDocument doc(1024);
  double motion = 0;
  
  doc["type"] = "request";
  serializeJson(doc,Serial);
  boolean messageReady = false;
  String message = "";
  
  while(messageReady == false) {
    
    if(Serial.available()){
      message = Serial.readString();
      messageReady = true;
    }
  //server.send(200, "text/html", SendHTML(false));
  }
  
  DeserializationError error = deserializeJson(doc,message);
  
  if(error){
  
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  
  }
  
  motion = doc["motion"];
  
  String output = "motion: " + String(motion);
  server.send(200,"text/plain",output);

}

/*void handle_ledon() {
  server.send(200, "text/html", SendHTML(true)); 
}

void handle_ledoff() {
  server.send(200, "text/html", SendHTML(false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>Motion Detection Meter</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Motion</h1>\n";
  ptr +="<p>Click to refresh page.</p>\n";
  ptr +="<form method=\"get\">\n";
  if(led)
  ptr +="<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
  else
  ptr +="<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}*/
