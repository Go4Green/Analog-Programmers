#include <Arduino.h>


#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "ArduinoJson.h"


// setting the sensor in the pin A0
float tempC = 0;
float mq7 = 0;
float mq135 = 0;
float mq131 = 0;

float mq7bef=0;
float mq135bef=0;
float mq131bef=0;


//float pcurve[3]  =  {2.3,0.72,-0.34}; 

float battery = 96.8;

String stringmessage;




// Function that will be executed once when connecting or resetting the Arduino
void setup()
{
  
  // start the Serial communication
  Serial.begin(115200); //115200
  WiFi.begin("Innovathens", "");


  while (WiFi.status() != WL_CONNECTED)
  { //Wait for the WiFI connection

    delay(500);
    Serial.println("Waiting for connection");
  }

  pinMode(A0, INPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
}

// Function that will be executed continuously
void loop()
{

  digitalWrite(D0, HIGH);
  delayMicroseconds(500);
  Serial.print("Temp  "); //Send a text to serial
  for (size_t i = 0; i < 10; i++)
  {
    float tempCtemp = (pow(float(analogRead(A0)) * 3300 / (1024),2)) ;
    tempC = tempCtemp + tempC;
    delayMicroseconds(1);
  }
  tempC = tempC / 10;

  Serial.print(tempC); //It gets the temperature in celcius and send to serial
  Serial.print(" C ");

  
  digitalWrite(D0, LOW);
  delayMicroseconds(10);
  /////////////////
  digitalWrite(D1, HIGH);
  delayMicroseconds(500);
  Serial.print(" CO ");
  for (size_t i = 0; i < 20; i++)
  {
    //float mq7temp = pow(((float(analogRead(A0)) * 3.3 / (1023)) / 0.01)/1.51,12.3) ;
    float mq7temp =float(analogRead(A0)) * 3.3 / (1024) ;
    mq7 = mq7temp + mq7;
    delayMicroseconds(1);
  }
  mq7 = mq7/2;
  Serial.println("mq7");
  //Serial.println(mq7temp);
  Serial.println(mq7);
  Serial.print("ppm ");
  Serial.println(" "); // Blank line
  digitalWrite(D1, LOW);
  delayMicroseconds(10);

  ///////////////
  digitalWrite(D2, HIGH);
  delayMicroseconds(500);
  Serial.print(" air quality ");

  for (size_t i = 0; i < 20; i++)
  {
    //float mq135temp = pow(((float(analogRead(A0)) * 5 / (1023)) / 0.01),4.4) ;
    //float mq135temp = pow(float(analogRead(A0) * 3.3 / (1.023)),1) ;
    float mq135temp = pow(float(analogRead(A0) * 3.3 / (10.23)),2) ;
    mq135 = mq135temp + mq135;
    delayMicroseconds(1);
  }
  //float vout =analogRead(A0) * 3300 / (1023);
  //float mq135temp =pow(10,(((log(vout)-pcurve[1])/pcurve[2]) + pcurve[0])) ;

  mq135 = mq135 / 2;
  mq135=(mq135bef*9 + mq135)/10;
  mq135bef=mq135;
  Serial.println(mq135);
  //Serial.println(mq135temp);
  Serial.print("ppm ");
  Serial.println(" "); // Blank line
  digitalWrite(D2, LOW);
  delayMicroseconds(10);
  
  
  for (size_t i = 0; i < 20; i++)
  {
    //float mq131temp = pow(((float(analogRead(A0)) * 5 / (1023)) / 0.01),4.4) ;
    //float mq131temp = pow(float(analogRead(A0) * 3.3 / (1.023)),1) ;
    float mq131temp = pow(float(analogRead(A0) * 3.3 / (10.23)),2) ;
    mq135 = mq131temp + mq131;
    delayMicroseconds(1);
  }
  //float vout =analogRead(A0) * 3300 / (1023);
  //float mq131temp =pow(10,(((log(vout)-pcurve[1])/pcurve[2]) + pcurve[0])) ;

  mq131 = mq131 / 20;
  mq135=(mq131bef*9 + mq131)/10;
  mq131bef=mq131;
  Serial.println(mq131);
  //Serial.println(mq131temp);
  Serial.print("ppm ");
  Serial.println(" "); // Blank line
  digitalWrite(D2, LOW);
  delayMicroseconds(10);
  /////////////////
  // to store the value in a variable you will use.
  //float temperature = temp.cel();
  if (WiFi.status() == WL_CONNECTED)
  {
    //Check WiFi connection status

    StaticJsonDocument<200> doc;
    doc["temp"] = tempC;
    doc["lgp"] = mq7 * 8.6;
    doc["ch4"] = mq7 * 10.82;
    doc["alcohol"] = mq7 * 10.86;
    doc["air"] = mq7 * 11.82;
    doc["h2"] = mq7 / 1.13;
    doc["o3"] = mq131 * ;
    doc["no2"] = mq131 * 1.44;;
    doc["so2"] = mq131 * 1.22;;
    doc["co"] = mq7;
    doc["aqi"] = mq135;
    doc["battery"] = battery;
    doc["time"] = 1351824120;
    serializeJson(doc, stringmessage);

    HTTPClient http; //Declare object of class HTTPClient

    http.begin("http://172.16.176.72:1880/temperature"); //Specify request destination

    http.addHeader("Content-Type", "application/json"); //Specify content-type header

    int httpCode = http.POST(stringmessage); //Send the request
    String payload = http.getString();       //Get the response payload
    Serial.println(stringmessage);
    Serial.println(httpCode);
  }
  else
  {

    Serial.println("Error in WiFi connection");
  }

  tempC = 0;
  mq7 = 0;
  mq135 = 0;
  stringmessage = "";
  //delay
  delay(1199);
}