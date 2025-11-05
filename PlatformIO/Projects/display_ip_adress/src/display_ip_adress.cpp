#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>

#define WIFI_SSID "ADN-IOT"
#define WIFI_PASSWORD "WBNuyawB2a"
#define ADNGROUP "ADN-Group31"
#define OLED_RESET 0 //GPIO0
Adafruit_SSD1306 display(OLED_RESET);

//Prototypes
void slowBlink();
void constantBlink();
void fastBlink();
void connectWIFI();

unsigned long startTime;

void setup() {
    Serial.begin(115200);
    delay(2000);
    pinMode(LED_BUILTIN, OUTPUT);
    connectWIFI();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000);
    display.clearDisplay();


}

void loop() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
  display.clearDisplay();
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