
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <WiFiUdp.h>


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
const char* ssid = "FASTWEB-E20FA7";  //"TIM-64269797";     // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "T18EFRGNRM"; //"kuzXEQG3szcDHK66U6NuDDA2";   // The password of the Wi-Fi network

const int sunset = 21; //average hour of the sunset
const int sunrise = 6; //average hour of the sunrise

const int sleepTime = 300; //Sleep time in second

//VAR

String temp = "0";  //Temperature var
String hum = "0";   //Humidity var
String weather = "Sunny"; //weather var

static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = +2;     // Central European Time

time_t getNtpTime();

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

WiFiClient client;

DHT dht(dht_dpin, DHTTYPE);


//Function to approximate the weather from temperature, ligth and rain value 
String getWeather(float t) {
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
    if (lightAnalogVal <= 700 && (hour() >sunrise && hour() <sunset) && hour()!=00 ) w = "cloudy";
    else w = "sunny";
  }
  else if (rainAnalogVal <= 680 && t < 0) w = "snow";
  else if (rainAnalogVal <= 680 && rainAnalogVal > 340) w = "rainy";
  else if (rainAnalogVal <= 340) w = "storm";
  /*TESTING*/
  /*
  Serial.println("WEATHER=> "+w);
  Serial.println("TEMPERATURE=> "+String(t));
  Serial.println("HUMIDITY=> "+String(h));
  Serial.println("LIGHT=> "+ String(lightAnalogVal));
  Serial.println("RAIN=> "+String(rainAnalogVal));
  */
  return w;
}

//Get the current time 
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (true) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//////////////////////////////////////////////////////////////////////////////////////


void setup(void) {
  Serial.begin(9600);
  dht.begin();
  pinMode(anag_in, INPUT);
  pinMode(rain_dpout, OUTPUT);
  pinMode(light_dpout, OUTPUT);

  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println("Succesfull connection");
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String weather = getWeather(t); 

  Serial.print(">>> Connecting to host: ");
  Serial.println(host);

  if (!client.connect(host, httpPort)) {
    Serial.print("Connection failed: ");
    Serial.println(host);
  } else {
    Serial.print("Connection succesfull: ");
    Serial.println(host);
 
    //the path and file to send the data to:
    String url = "/update";
    
    // We now create and add parameters:
    url += "?temp=" + String(t) + "&hum=" + String(h) + "&weather=" + weather;

    client.println(String("GET ") + url + " HTTP/1.1\r\n" + "Host:" + host + "\r\n" + "Connection : keep-alive\r\n\r\n" );

    //just checks the 1st line of the server response. Could be expanded if needed.
    while (client.available()) {
      Serial.println(client.readStringUntil('\r'));
      break;
    }

  } //end client connection if else
  Serial.println("ESP sleep");
  ESP.deepSleep(sleepTime * 1e6);


}
void loop() {
}
