
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>



#define DHTTYPE DHT11   // DHT 11

//Pin definition
#define anag_in A0 //Pin for the analog read
#define rain_dpout D1 //Pin to turn on/off the rain sesnor
#define dht_dpin D2 //Pin to read from the temperature sensor
#define light_dpout D3 //Pin to turn on the ligth resistor

//CONST

//Host
const char* host = "192.168.1.99";
const int httpPort = 80;

//WiFi credential
const char* ssid PROGMEM = "FASTWEB-E20FA7"; //"TIM-64269797";     // The SSID (name) of the Wi-Fi network you want to connect to
const char* password PROGMEM = "T18EFRGNRM"; //"kuzXEQG3szcDHK66U6NuDDA2";   // The password of the Wi-Fi network

const int sunset PROGMEM = 21; //average hour of the sunset
const int sunrise PROGMEM = 6; //average hour of the sunrise

const int sleepTime PROGMEM = 300; //Sleep time in second



// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200);

WiFiClient client;

DHT dht(dht_dpin, DHTTYPE);


//Function to approximate the weather from temperature, ligth and rain value
String getWeather(float t) {

  uint32_t beginWait = millis();
  while (!timeClient.update()) {
    if (millis() - beginWait >= 50000) ESP.deepSleep(1); //If the wait is over 2 minutes restart the board
    timeClient.forceUpdate();
  }

  String w = "";

  digitalWrite(rain_dpout, HIGH);
  delay(100);
  int rainAnalogVal = analogRead(anag_in);

  digitalWrite(rain_dpout, LOW);

  digitalWrite(light_dpout, HIGH);
  delay(100);
  int lightAnalogVal = analogRead(anag_in);

  digitalWrite(light_dpout, LOW);

  if (rainAnalogVal > 680) {
    if (lightAnalogVal <= 700 && (timeClient.getHours() > sunrise && timeClient.getHours() < sunset) && timeClient.getHours() != 00 ) w = "cloudy";
    else w = "sunny";
  }
  if (rainAnalogVal <= 680 && t < 0) w = "snow";
  else if (rainAnalogVal <= 680 && rainAnalogVal > 340) w = "rainy";
  else if (rainAnalogVal <= 340) w = "storm";

  /*TESTING*/
  /*
    Serial.println("WEATHER=> " + w);
    Serial.println("TEMPERATURE=> " + String(t));
    Serial.println("LIGHT SENS=> " + String(lightAnalogVal));
    Serial.println("RAIN SENS=> " + String(rainAnalogVal));

    Serial.print("TIME=>");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());
  */
  return w;
}

//////////////////////////////////////////////////////////////////////////////////////


void setup(void) {
  Serial.begin(9600);
  Serial.println("");

  dht.begin();

  pinMode(rain_dpout, OUTPUT);
  pinMode(light_dpout, OUTPUT);

  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(F("WiFi Failed!"));
    return;
  }
  Serial.println(F("Succesfull connection"));
  Serial.println();
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());

  timeClient.begin();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //Serial.println("**TESTING**");
  //Serial.println("HUMIDITY=>" + String(h));

  String weather = getWeather(t);



  Serial.print(F(">>> Connecting to host: "));
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.print(F("Connection failed: "));
    Serial.println(host);
  } else {
    Serial.print(F("Connection succesfull: "));
    Serial.println(host);

    //the path and file to send the data to:
    String url = "/update";

    // We now create and add parameters:
    url += "?temp=" + String(t) + "&hum=" + String(h) + "&weather=" + weather;

    //just checks the 1st line of the server response. Could be expanded if needed.
    Serial.println(F("ESP Sending..."));
    do {
      client.println(String("GET ") + url + " HTTP/1.1\r\n" + "Host:" + host + "\r\n" + "Connection : keep-alive\r\n\r\n" );
      Serial.print(F("..."));
    } while (!client.readStringUntil('\n'));
    Serial.println("\n");

  } //end client connection if else
  Serial.println(F("ESP sleep"));
  ESP.deepSleep(sleepTime * 1e6);

}



void loop() {
}
