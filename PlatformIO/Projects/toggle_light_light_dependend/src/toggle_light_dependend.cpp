#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_TSL2561_U.h>

#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "ADN-Group31"
#define BULP_IP "192.168.0.55"


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

const float L_ON = 200.0; //Turn on below threshold
const float L_OFF = 300.0; //Turn off above threshold

//Prototypes
void slowBlink();
void constantBlink();
void fastBlink();
void connectWIFI();
void lightSensorSetup();
void turnLampOff();
void turnLampOn();
void testlightIntensitiy();

unsigned long startTime;
int light_status = 1;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  connectWIFI();
  delay(1000);
  lightSensorSetup();
}

void loop() {
  //testlightIntensitiy();
  sensors_event_t event;
  tsl.getEvent(&event);
  if(event.light < L_ON && light_status == 0){
    Serial.println("Over Threshhold. Turn Lamp on");
    turnLampOn();
  } else if(event.light > L_OFF && light_status == 1){
    Serial.println("Over Threshhold. Turn Lamp off");
    turnLampOff();
  }
  delay(2000);
}

void lightSensorSetup(){
  tsl.begin();
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
}

void turnLampOff(){
  HTTPClient http;
  String url = "http://" + String(BULP_IP) + "/cm?cmnd=Power%20OFF";
  Serial.println("Sending request: " + url);
  http.begin(url);
  http.GET();
  http.end();
  light_status = 0;
}

void turnLampOn(){
  HTTPClient http;
  String url = "http://" + String(BULP_IP) + "/cm?cmnd=Power%20ON";
  Serial.println("Sending request: " + url);
  http.begin(url);
  http.GET();
  http.end();
  light_status = 1;
}


void testlightIntensitiy(){
  sensors_event_t event;
  tsl.getEvent(&event);
  Serial.print(event.light);
  Serial.println("lux");
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