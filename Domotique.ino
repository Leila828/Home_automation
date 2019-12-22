//Our libraries whoaaa :o 
#include <ThingSpeak.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
DHTesp dht;
Servo myservo ;
WiFiClient Leila;
IPAddress ip(192, 168, 43, 7);
WiFiServer server(80);// 80 porte pour l'http

unsigned long id =487575;
const char* APIt="PGRBOCDZHP52MFAT"; 

int count;
int count2;
int System;
int i;
int fan=D2;
int door;
int led_app=D5;
int gled;
int b=0;
int Pin_Buzzer = D3;
int movement=0 ;
int Echo_Ultra = D10;
int Trig_Ultra= D11;
int Led_Red = D15 ;  //led blue dans notre maquette
int Led_Green = D12;                          
long duree_Ultra ;
float distance_Ultra ;
char ssid[] = "sosn";
char pass[] = "sosen123";

void setup() {
  //DHT tool has a special case , so it has to be declared in the setup 
    dht.setup(D8);
    
Serial.begin(115200);
   WiFi.begin(ssid, pass);
    while(WiFi.status()!= WL_CONNECTED)
    {
        delay(200);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.println (WiFi.localIP ());


        pinMode(fan,OUTPUT);

    pinMode(Trig_Ultra,OUTPUT);
    pinMode(Echo_Ultra,INPUT);
    pinMode(Led_Red,OUTPUT);
    pinMode(led_app,OUTPUT);
    pinMode(Led_Green,OUTPUT); 
    pinMode(Pin_Buzzer,OUTPUT);
     myservo.attach(D14);

ThingSpeak.begin(Leila);
 server.begin();
 count=0;
  count2=0;
 i=0;
 
}

void loop() {

 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
       //Serial.write(c);                    // print it out the serial monitor
      if (c == '\n') {                    // if the byte is a newline character
         if (currentLine.length() == 0) {
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else
        if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /SN" or "GET /SF":
        if (currentLine.endsWith("GET /SN")) {
                System= 1; 
        }
        
        if (currentLine.endsWith("GET /SF")) {
                 System=0;  
        }
         
   

          if (currentLine.endsWith("GET /RLN")) {
               digitalWrite( led_app,HIGH); 
        }
        
        if (currentLine.endsWith("GET /RLF")) {
                                digitalWrite( led_app,LOW); 
 

          }
        if (currentLine.endsWith("GET /DF")) {
                door= 1;  
        }
        
        if (currentLine.endsWith("GET /DC")) {
                 door=0;  
        }

        if (currentLine.endsWith("GET /FAN")){
                               digitalWrite(fan,HIGH); 
                                          Serial.println("mmmm");
                                i=1;
        }
         if (currentLine.endsWith("GET /FAF")){
                                       Serial.println("naas");
                                           i = 0;


        }
        
      }}
    
    // close the connection:
    client.stop();
  
  }
 if (System==1){
 

   
//Ultrasonic's coding
  digitalWrite(Trig_Ultra,LOW);
  delayMicroseconds(5);
  digitalWrite(Trig_Ultra,HIGH);
  delayMicroseconds(5);
  digitalWrite(Trig_Ultra,LOW);
  duree_Ultra = pulseIn(Echo_Ultra,HIGH);
  
  //Distance
  distance_Ultra = (duree_Ultra*0.034)/2;
  Serial.println("Our distance is <we already know it> ");
  Serial.print(distance_Ultra);
  Serial.println(" cm");

 

  count2=0;
 
 if (distance_Ultra > 11) {
                              movement=0; 
            
                           
                         
                      }
                 
                    else  { movement=1;
                         b=1;
                        digitalWrite(Led_Green,LOW);
                        digitalWrite(Led_Red,HIGH);
                        digitalWrite(Pin_Buzzer,HIGH);
                     
     
                    
                        }
       delay(30);
                       count++;
                         delay(20);

                       if (count==250){
                        ThingSpeak.setField(3,b);
                        ThingSpeak.writeFields(id,APIt);
                        count=0;
                        Serial.print("Leila");
                      }
     
     if (b==1) {
                          digitalWrite(Led_Green,LOW);
                          digitalWrite(Led_Red,HIGH);
                        }                    
                        
 }
  
 else if (System==0){
    b=0;
    count=0;
   
    movement=0;
  //noTone(Pin_Buzzer);
   digitalWrite(Pin_Buzzer,LOW);

  digitalWrite(Led_Red,LOW);
  digitalWrite(Led_Green,HIGH);
  float t =dht.getTemperature(); 
  float h =dht.getHumidity(); 
  Serial.println(t);
  Serial.println(h);
  if (i==0){
  if (t>=30){
    digitalWrite(fan,HIGH);
    Serial.print(" high"); 
  }
  else {
    digitalWrite(fan,LOW);
    Serial.print("low "); 
  }
  }
 
  
  count2++;
  Serial.println(count2);
  
  delay(20);

                         if (count2==1000){

  ThingSpeak.setField(1,t);
  ThingSpeak.setField(2,h);
  ThingSpeak.setField(3,movement);
  ThingSpeak.writeFields(id,APIt);
                          count2=0;

                         }
 }

 if (door==1){      
  myservo.write(20);   
  }
 if (door==0){
   myservo.write(180); 
 }       
 }
  

