//imports
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <AsyncMqttClient.h>
#include <avdweb_Switch.h>

//definitions
#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "ADN-Group31"
#define BULP_IP "192.168.0.55"
#define MQTT_HOST IPAddress(192, 168, 0, 1)
#define MQTT_PORT 1883
#define PUSHBUTTON 17

Switch button = Switch(PUSHBUTTON);

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
}

void loop() {
  button.poll();
  if(button.pushed()){ 
    Serial.println("Button pushed");
    const char* payload = "TOGGLE";
    uint16_t pidPub = mqttClient.publish(CMD_TOPIC, 1, false, "TOGGLE", 0, false, 0);
    Serial.printf("Sent: %s  %s\n", CMD_TOPIC, payload);
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