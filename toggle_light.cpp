#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "ADN-Group31"
#define BULP_IP "192.168.0.52"
#define BULP_HOSTNAME "sonoff 02-3044"

//Prototypes
void slowBlink();
void constantBlink();
void fastBlink();

unsigned long startTime;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

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

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(BULP_IP) + "/cm?cmnd=Power%20TOGGLE";
    Serial.println("Sending request: " + url);
    http.begin(url);
    http.GET();
    http.end();

    Serial.println("Toggled power!");
  } else {
    Serial.println("WiFi not connected");
  }

  delay(2000);
}
void slowBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay (1000);
}

void constantBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (1000);
}   

void fastBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (200);
  digitalWrite(LED_BUILTIN, LOW);
  delay (200);
}