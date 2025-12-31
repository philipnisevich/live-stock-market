#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Homeland Security Van 3";
const char* password = "Adobe2000";
String payload = "";

int GreenLed = 15;
int RedLed = 33;
int dt = 2000;

void connectWiFi() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected to WiFi");
  display.display();
  delay(2000);
}

void resetDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

void readPrice(int x, int y, const String& stockName) {
  String httpRequestAddress = "https://finnhub.io/api/v1/quote?symbol=" + stockName + "STOCK_API_TOKEN";
  HTTPClient http;
  int httpCode;

  http.begin(httpRequestAddress);
  httpCode = http.GET();

  digitalWrite(GreenLed, HIGH);
  digitalWrite(RedLed, HIGH);

  if (httpCode > 0) {
    DynamicJsonDocument doc(1024);
    String payload = http.getString();
    Serial.println(payload);
    deserializeJson(doc, payload);

    float previousClosePrice = doc["pc"];
    float currentPrice = doc["c"];
    float differenceInPrice = ((currentPrice - previousClosePrice) / previousClosePrice) * 100.0;

    resetDisplay();
    display.setTextSize(2);
    display.setCursor(40, y);
    display.println(stockName);

    if (differenceInPrice < 0.0) {
      display.setTextColor(SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.setTextSize(2);
    display.setCursor(x, y + 25);
    display.print(currentPrice, 2);
    display.println("$");

    display.setTextSize(2);
    display.setCursor(x, y + 50);
    display.print(differenceInPrice, 2);
    display.println("%");
    if(differenceInPrice >= 0){
      digitalWrite(GreenLed, LOW);
    }
    else if(differenceInPrice < 0){
      digitalWrite(RedLed, LOW);
    }

    display.display();
  } else {
    resetDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Error in HTTP request");
    display.display();
  }

  http.end();
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Stock Prices Tracker");
  display.display();
  pinMode(GreenLed, OUTPUT);
  pinMode(RedLed, OUTPUT);

  connectWiFi();
}

void loop() {
  readPrice(0, 0, "AAPL");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "AMZN");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "NKE");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "TSLA");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "VEEV");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "PFE");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "OXY");
  delay(dt);
  display.clearDisplay();
  readPrice(0, 0, "EBAY");
  delay(dt);
  display.clearDisplay();
  readPrice(0,0, "ADBE");
  delay(dt);
  display.clearDisplay();
}

