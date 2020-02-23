/*
Project: Smart Robot
Author: Rahul, Narshi, Piyush and Milan
Date: 1st march 2018
Place: GEC Rajkot
*/

#include <FirebaseArduino.h>
#include  <ESP8266WiFi.h>
#include <Servo.h>
#include <DHT.h>
#include "config.h"
 
Servo myservo;
#define FIREBASE_HOST "*****.firebaseio.com"    //your firebase id
#define WIFI_SSID "WIFINAME"  //your Wifi name
#define WIFI_PASSWORD "****"  //your Wifi password
#define authCode "********************"          //your Auth code 
#define DHTTYPE DHT11   // DHT 11

const int dataPin = 4;   //Outputs the byte to transfer to D2 pin of nodemcu
const int loadPin = 5;   //Controls the internal transference of data in SN74HC595 internal registers to D1 
const int clockPin = 16;  //Generates the clock signal to control the transference of data to D0 
const int DHTPIN = 2;    // Data pin of dht11 to D4  

DHT dht(DHTPIN, DHTTYPE);

void setup() {
 myservo.attach(0); // D3 pin of nodemcu
 pinMode(dataPin, OUTPUT);
 pinMode(loadPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(D5, INPUT);
 pinMode(D6, OUTPUT);
 pinMode(A0, INPUT);
  
  Serial.begin(115200);
   WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("WiFi Connected!");
  
  dht.begin();
  Firebase.begin(FIREBASE_HOST, authCode);
  Firebase.set("Status",0);
  Firebase.set("Servo",0);
}

String n,k; 
int pos,h,t,v,g;
byte data,L;

void loop() {
  
  n=Firebase.getString("Status");
  //Serial.println(n);
  Checktemp();

  if (n=="\"LIGHTON\"")
  {
    L = B00000010;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, L); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("torch on");
    delay(100);
  }

  else if (n=="\"LIGHTOFF\"")
  {
    L = B00000000;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, L); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("torch off");
    delay(100);
  }
  
  else if (n=="\"FORWARD\"")
  {
    data =B01100000 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
   // Serial.println("moving forward");
    delay(100);
  }
  
  else if (n=="\"REVERSE\"")
  {
    byte data = B10010000 | L ;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("moving reverse");
    delay(100);
  }
  
  else if (n=="\"LEFT\"")
  {
    data = B01010000 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("moving left");
    delay(100);
  }

  else if (n=="\"RIGHT\"")
  {
    data = B10100000 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("moving right");
    delay(100);
  }

   else if (n=="\"STOP\"")
  {
    data = B00000000 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    //Serial.println("not moving");
    delay(100);
  }
  
  else if (n=="\"PANON\"")
  {
    data = B00001000 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    delay(500);
    k=Firebase.getString("Servo");
    pos = k.toInt();
    myservo.write(pos);  
    //Serial.println(pos);
    delay(2000);
  }
  
  else if (n=="\"TILTON\"")
  {
    data = B00000100 | L;
    digitalWrite(loadPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, data); 
    digitalWrite(loadPin, HIGH);
    delay(500);
    k=Firebase.getString("Servo");
    pos = k.toInt();
    myservo.write(pos);  
    //Serial.println(pos);
    delay(2000);
  }

 else if (n=="\"OFFLINE\"")
  {
    v = analogRead(A0);
    double voltage = map(v,0,850, 0, 1300); // map 0-850 to 0-1300
    voltage /=100;// divide by 100 to get the decimal values
    g = digitalRead(D5);
    Firebase.setFloat ("Voltage",voltage);
    Firebase.setFloat ("Gas",g);
  }
  
  else if (n=="\"SENSOR\"")
  {
    h = dht.readHumidity(); 
    t = dht.readTemperature();
    v = analogRead(A0);
    double voltage = map(v,0,850, 0, 1300); // map 0-850 to 0-1300
    voltage /=100;// divide by 100 to get the decimal values
    g = digitalRead(D5);
    Firebase.setFloat ("Temp",t);
    Firebase.setFloat ("Humidity",h);
    Firebase.setFloat ("Voltage",voltage);
    Firebase.setFloat ("Gas",g);
    //Serial.println("sensor value");
    delay(100);
  }
   delay(200);
}

void Checktemp(){
  //  h = dht.readHumidity(); 
    t = dht.readTemperature();
   // Serial.println("temperature = ");  
   // Serial.println(t);
   // Serial.println("humidity = ");
   // Serial.println(h);
    if (t>35)
    {
      digitalWrite(D6, HIGH);
    }
    else {
      digitalWrite(D6, LOW);
    }
    delay(100);
}


