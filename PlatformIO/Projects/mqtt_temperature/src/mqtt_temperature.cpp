//imports
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include<AsyncTCP.h>
#include<AsyncMqttClient.h>
#include <Adafruit_BME280.h>

//definitions
#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "adn-group31"
#define BULP_IP "192.168.0.55"
#define MQTT_HOST IPAddress(192, 168, 0, 1)
#define MQTT_PORT 1883

Adafruit_BME280 bme;

AsyncMqttClient mqttClient;

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
  mqttSetup();
  bme.begin(0x76);
}

void loop() {
  static unsigned long lastPublish = 0;
  const unsigned long publishInterval = 30000;
  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
    float temp = bme.readTemperature();
    float hum  = bme.readHumidity();
    float pres = bme.readPressure() / 100.0F; // hPa (optional /100, je nach Vorgabe)
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.println(hum);
    Serial.print("Pressure: ");
    Serial.println(pres);
    char payload[3 * sizeof(float)];
    memcpy(payload,                     &temp, sizeof(float));
    memcpy(payload + sizeof(float),     &hum,  sizeof(float));
    memcpy(payload + 2 * sizeof(float), &pres, sizeof(float));
    mqttClient.publish("adn/group31/temp", 1, true, payload, sizeof(payload));
  }
}

void onMqttMessage(char* topic, char* payload,
                   AsyncMqttClientMessageProperties properties,
                   size_t len, size_t index, size_t total) {
  // Payload ist NICHT nullterminiert → Länge benutzen
  String t = topic;
  String msg = String(payload).substring(0, len); // nur zur Anzeige
  Serial.printf("[MQTT] %s  %s\n", t.c_str(), msg.c_str());
}

void mqttSetup(){
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setClientId(ADNGROUP);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.connect();
  // Wait for connection
  while(!mqttClient.connected()){
    Serial.print("Connecting MQTT"); delay(500);
  }
  Serial.println("\nConnected to MQTT");
  uint16_t pidSub = mqttClient.subscribe(STAT_TOPIC, 1);
  //mqttClient.subscribe(STAT_TOPIC, 1);
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