
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#include <ESP8266WiFi.h>

#define DHTTYPE DHT11   // DHT 11

#define rain_in A0

#define rain_dpin D1
#define dht_dpin D2
#define light_dpin D3


const char* host = "192.168.1.99";
const int httpPort = 80;

const char* ID = "myESP";
const char* PW = "password";

const char* ssid = "FASTWEB-E20FA7";  //"TIM-64269797";     // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "T18EFRGNRM"; //"kuzXEQG3szcDHK66U6NuDDA2";   // The password of the Wi-Fi network


String temp = "0";
String hum = "0";
String whether = "Sunny";
String isRain = "0";
const String src = "ESP";

const int sleepTime = 5;
WiFiClient client;

DHT dht(dht_dpin, DHTTYPE);

String getWeather(float h, float t) {
  String w = "";


  int rainAnalogVal = analogRead(rain_in);
  delay(1000);
  Serial.print("RAIN=> ");
  Serial.println(rainAnalogVal);
  
  if(rainAnalogVal>680) w="sunny";
  else if(rainAnalogVal<=680 && t<0) w="snow";
  else if(rainAnalogVal<=680 && rainAnalogVal>340) w="rainy";
  else if(rainAnalogVal<=340) w="storm";

  return w;
}
//////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {
  Serial.begin(9600);
  dht.begin();
  pinMode(rain_in, INPUT);
  pinMode(rain_dpin, OUTPUT);
  pinMode(light_dpin, OUTPUT);


  digitalWrite(rain_dpin,HIGH);
  delay(2000);
  int rain_test=analogRead(rain_in);
  delay(2000);
  digitalWrite(rain_dpin,LOW);
  Serial.print("RAIN=> ");
  Serial.println(rain_test);
 


  digitalWrite(light_dpin,HIGH);
  int light_test=analogRead(rain_in);
  delay(2000);
  digitalWrite(light_dpin,LOW);
  Serial.print("LIGHT=> ");
  Serial.println(light_test);
  

  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println("Succesfull connection");
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print(">>> Connecting to host: ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.print("Connection failed: ");
    Serial.println(host);
  } else {
    Serial.print("Connection succesfull: ");
    Serial.println(host);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    hum = "";
    hum.concat(h);
    temp = "";
    temp.concat(t);

    //the path and file to send the data to:
    String url = "/update";

    String weather = getWeather(h, t);
    // We now create and add parameters:
    url += "?src=" + src + "&temp=" + temp + "&hum=" + hum + "&weather=" + weather;

    client.println(String("GET ") + url + " HTTP/1.1\r\n" + "Host" + host + "\r\n" + "Connection : keep-alive\r\n\r\n" );


    //just checks the 1st line of the server response. Could be expanded if needed.
    while (client.available()) {
      String resLine = client.readStringUntil('\r');
      Serial.println(resLine);
      break;
    }

  } //end client connection if else
  Serial.println("ESP sleep");
  ESP.deepSleep(sleepTime * 1e6);


}
void loop() {
}
