#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <List.hpp>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <TimeLib.h>
#include <WiFiUdp.h>

#include "mainPage_1.h"
#include "mainPage_2.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define watering_control D3 //Pin of the relé 
#define button D5  //Button for local setting
#define encoder A0  //Rotary Encoder for local setting
#define led_on D8 //Pin of the green Led


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//CONST

const char* ssid PROGMEM= "FASTWEB-E20FA7";  // The SSID (name) of the Wi-Fi network you want to connect to
const char* password PROGMEM= "T18EFRGNRM";  // The password of the Wi-Fi network

//VAR
bool startNow = false;  //Start now watering flag
bool daily = false;     //Daily flag
bool weekly = false;    //weekly flag

//Sensor variable from the station
float t = 0;  //Temperature
int h = 0;    //Humidity
String w = "cloudy"; //Weather


List<time_t> allarms_list; //list of allaram

time_t periodic;  //A periodic allarm(daily or weekly)

time_t last_watering; //The last time a watering was completed

time_t last_raining;  //The last time it rained 

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

AsyncWebServer server(80);

//Build the page with the last data get from the sens
String getPage() {
  return String(MAIN_page_1) +
    "var t = \" " + t + " \"; var h = \" " + h + " \"; var w =\"" + w + "\";" + 
    String(MAIN_page_2);
  
}

/*Check if an allarm occurrence has passed.
 * In both case remove the allarm from 
 */
 
bool checkAllram() {
  bool flag = false;
  if (now() >= getMinAllarm()) {
    flag = true;
    if (periodic == getMinAllarm()) {
      if (daily) {
        periodic = nextDaily(periodic);
      } else if (weekly) {
        periodic = nextWeekly(periodic);
      }
    }
    if (!allarms_list.isEmpty()) {
      if (allarms_list.getValue(allarms_list.getSize() - 1) == getMinAllarm()) {
        allarms_list.removeLast();
      }
    }

  }
  return flag;
}
//Clear all the allarm
void clearAllarm(){
    startNow = false;  
    daily = false;     
    weekly = false;
         
    allarms_list.removeAll(); 
    periodic= NULL;
    return;
  
}

//Print the next Allarm if is present, "no Allarm" otherwise
void printAllarm() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40, 30);
  //String printbleTime = "";

  //display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30, 40);
  // Display static text

  if (daily || weekly || (!allarms_list.isEmpty() )) {
    //time_t minAllarm = getMinAllarm();
    display.println( F("Next watering:") );
    display.setCursor(20, 55);
    display.println( timeToString(getMinAllarm()) );
  }
  else {
    display.println(F("No watering"));
    display.setCursor(30, 55);
    display.println(F("in program"));
  }

  display.display();
}

//Print the value from the weather station
void printSens() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print(t);
  display.setTextSize(1);
  display.print((char)247);
  display.print(F("C"));
  display.setCursor(90, 10);
  display.setTextSize(2);
  display.print(h);
  display.setTextSize(1);
  display.print(F("%"));
}

//Start watering if temperature is over 0 deg celsius,isn't raining  or in the last 12h it has rained
void start_watering() {
  if (t < 0 || (w != "sunny" && w!="cloudy") ||  now()-last_raining <= 43200) {
    startNow = false;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(15, 32);
    if(t<0) display.println(F("TOO COLD") );
    if(w!="sunny" && w!="cloudy") display.println( F("IS RAINING") );
    if(now()-last_raining >= 43200) display.println( F("IT RAINED 2 AGO") );
    
    display.display();
    delay(5000);
    return;
  }
  last_watering = now();
  digitalWrite(watering_control, HIGH);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(15, 32);
  display.println( F("WATERING") );
  display.display();
  delay(60000);
  digitalWrite(watering_control, LOW);
  startNow = false;
  return;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, F("text/plain"), F("Not found"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  

  pinMode(led_on, OUTPUT);
  pinMode(watering_control, OUTPUT);

  pinMode(button, INPUT);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println(F("WiFi Failed!"));
    return;
  }
  Serial.println();
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());


  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(8.64*1e7); //Sync every 24h the time with NTP server
  
  //Led on
  Serial.println("START");
  digitalWrite(led_on, HIGH);

 
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", getPage());
  });

  // Send a POST request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/", HTTP_POST, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;
    int params = request->params();
    
    //PARAM TEST
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
    }
    
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>q
    if (request->hasArg("date")) {
      String allarm = request->getParam(0)->value().c_str();
      time_t newAllarm = fromString(allarm);
      sortedAddT(newAllarm);
      request->send(200, "text/html", getPage());
    }
    //Istant Start
    else if (request->hasArg("start")) {
      startNow = true;
      request->send(200, "text/html", getPage());
    }
    //Select a time for a repetitive watering
    else if (request->hasArg("time")) {
      Serial.println(request->arg("modality"));
      Serial.println(request->arg("time"));
      String mod = request->getParam(1)->value().c_str();
      String t = request->getParam(0)->value().c_str();
      //For daily watering
      if (mod.equals("daily")) {
        daily = true;
        weekly = false;
        int h = t.substring(0, 2).toInt();
        int m = t.substring(3, 5).toInt();
        periodic = getDaily(h, m);
        request->send(200, "text/html", getPage());
      }
      //For weekly watering
      else if (mod.equals("weekly")) {
        if (request->hasArg("dayOfTheWeek")) {
          String dayOfTheWeek = request->getParam(2)->value().c_str();
          daily = false;
          weekly = true;
          int wd = dayOfTheWeek.toInt();
          int h = t.substring(0, 2).toInt();
          int m = t.substring(3, 5).toInt();
          periodic = getWeekly(h, m, wd);
          request->send(200, "text/html", getPage());
        }
      }
    }
    else if(request->hasArg("clear")){
      
      clearAllarm();
      request->send(200, "text/html", getPage());
      
    }
    else {
      Serial.println(F("No message"));
      inputMessage = "No message sent";
      inputParam = "none";
      request->send(200, "text/html", getPage());
    }
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
  
      AsyncWebParameter* tempMsg = request->getParam("temp");
      AsyncWebParameter* rainMsg = request->getParam("weather");
      AsyncWebParameter* humMsg = request->getParam("hum");
      
      w = rainMsg->value().c_str();
      String temp = tempMsg->value().c_str(); 
      String hum = humMsg->value().c_str();

      t = temp.toFloat();
      h = hum.toInt();

      Serial.println(F("ESP UPDATE"));
      Serial.print(F("Temp: "));
      Serial.println(t);
      Serial.print(F("Humidity: "));
      Serial.println(h);
      Serial.print(F("Weather: "));
      Serial.println(w);

      //if (w != "sunny") last_raining = now();
    
    request->send(200, "text/html", "receved");
  });
  server.onNotFound(notFound);
  server.begin();
  
}

void loop() {
  printSens();
  printAllarm();
  if (startNow || checkAllram()) {
    start_watering();
  }

  //If the temperature is over 30 deg celsius try to start watering.
  if (t > 30) { 
    if (now() - last_watering >= 43200 && getMinAllarm()-now()>=43200) {
      start_watering();
    }
  }
  if (digitalRead(button)) {
    delay(200);
    setLocally();
  }
}
