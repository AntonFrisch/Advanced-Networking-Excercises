//imports
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include<AsyncTCP.h>
#include "TinyGPS++.h"

//definitions
#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "adn-group31"
#define BULP_IP "192.168.0.55"

HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

unsigned long startTime;

int light_status = 0; // 0 = off, 1 = on

static const char* CMD_TOPIC  = "cmnd/sonoff_04/POWER"; // Befehl ggf. POWER ändern

static const char* STAT_TOPIC = "stat/sonoff_01-5615/#";     // Rückmeldungen

//Prototypes
void slowBlink();
void constantBlink();
void fastBlink();
void connectWIFI();
void turnLampOff();
void turnLampOn();
void mqttSetup();

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  connectWIFI();
  delay(1000);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  delay(1000);
  Serial.println(F("GPS Test gestartet"));
}

void loop() {
while(gpsSerial.available()>0){
  gps.encode(gpsSerial.read());
}
  //Position
  double latitude  = gps.location.lat();       // in Grad
  double longitude = gps.location.lng();       // in Grad
  double altitude  = gps.altitude.meters();    // in Metern

  uint32_t hdop    = gps.hdop.value();         // x/100 m (HDOP*100)
  // Debug
  uint32_t sats        = gps.satellites.value();
  uint32_t age         = gps.location.age();   // ms seit letztem Fix
  uint32_t failed      = gps.failedChecksum();
  uint32_t processed   = gps.charsProcessed();
  gps.passedChecksum();
  //Default GPS Info in Case connection is bad
      /*
    latitude  = 49.573796;
    longitude = 11.026791;
    age       = 500;
    */
  //Printing
  Serial.println(F("----- GPS Info -----"));
  Serial.print(F("Latitude:  "));
  Serial.println(latitude, 6);
  Serial.print(F("Longitude: "));
  Serial.println(longitude, 6);
  Serial.print(F("Altitude:  "));
  Serial.print(altitude);
  Serial.println(F(" m"));
  Serial.print(F("HDOP:      "));
  Serial.print(hdop);
  Serial.println(F(" (x/100 m)"));
  Serial.print(F("Satellites:          "));
  Serial.println(sats);
  Serial.print(F("Age:                 "));
  Serial.print(age);
  Serial.println(F(" ms"));
  Serial.print(F("Failed checksums:    "));
  Serial.println(failed);
  Serial.print(F("Processed characters:"));
  Serial.println(processed);
  Serial.println();
  delay (10000);
}

void slowBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay (1000);
}

void constantBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
}   

void fastBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (200);
  digitalWrite(LED_BUILTIN, LOW);
  delay (200);
}

void connectWIFI () {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(ADNGROUP);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  startTime = millis();
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < 10000) {
    slowBlink();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    constantBlink();
  } else {
    Serial.println("\nFailed to connect to WiFi");
    fastBlink();
}
}