d/*
 *  Created by Jean-Baptiste Leconte on 8/23/16.
 *  Copyright © 2016 Jean-Baptiste Leconte. All rights reserved.
 *  
 *  
 *  
 *  enter the IP address of the ESP8266 12E module into your web browser or app + /door/1 or , will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define magSwitch 14
#define doorPin 4
#define statusLed 5
#define wifiStatusNotConnected 13 
#define wifiConnected 12 

// home WiFI login info
const char *ssid = "Enter Your ssid";
const char *password = "Enter Your password";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server ( 80 );


void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO 
       pinMode(magSwitch, INPUT_PULLUP);
       pinMode(doorPin, OUTPUT);
       pinMode (statusLed, OUTPUT);
       pinMode (wifiStatusNotConnected, OUTPUT);
       pinMode (wifiConnected, OUTPUT);
       
       digitalWrite(statusLed, HIGH);
 
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   digitalWrite(wifiStatusNotConnected, HIGH);
   Serial.print(".");
   
  }
  Serial.println("");
  Serial.println("WiFi connected");
      digitalWrite(wifiStatusNotConnected, LOW);
      digitalWrite(wifiConnected, HIGH);
  ;
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  //Magnet Switch code
  unsigned int doorState;
   doorState= digitalRead(magSwitch);
      if (doorState == HIGH){
         Serial.println(" magSwitch The Door is Open");
     }
      else {
       Serial.println(" magswitch The Door is Closed");
      }
}

  
void loop() {

 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
 
 // temp code
  int sensorValue = analogRead(A0);
  float volt = (sensorValue/1024.0) * 5; //convert to Millivolts
  float tempC = (volt - 0.5) * 100;      //convert to temp in C
  float tempF = tempC * (9.0/5.0) + 32.0; //convert to temp in F
  delay(2000);
  Serial.println(" The outside Temp is:   ");
  
  Serial.println(tempF);
  

      
  // Door Stauts request
  
  int state;
  unsigned int doorState;
  doorState= digitalRead(magSwitch);
  if (doorState == 1){
      state = 1;
  }
  else if (doorState == 0){
      state = 0;
  }
  String s = "The Door is  ";
  s += (state)?"OPEN":"Closed";
  
  
  if (req.indexOf("/door/1") != -1){
     client.println("Door is Active");
     digitalWrite(doorPin, HIGH);
     delay(1000);
     digitalWrite(doorPin, LOW);}
  else if (req.indexOf("/doorState/1") != -1){
        
        client.println(); 
        client.println(s);
     }
  else {
    Serial.println("invalid request");
   client.stop();
    return;
  }
  
  client.flush();

 // Send the response to the client
  
  client.println("The Temp is: ");
  client.println(tempF);

  delay(1);
  Serial.println("Client disonnected");

 
  
 client.flush();
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

}

