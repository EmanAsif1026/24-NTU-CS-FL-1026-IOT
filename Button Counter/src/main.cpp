
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
#define OLED_RESET -1

const int BUTTON_PIN = 18;
const unsigned long DEBOUNCE_MS = 40;

Adafruit_SSD1306 display(
  SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET
);

int pressCount = 0;
int lastRawReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastRawChangeTime = 0;

void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("ESP32 Button Monitor");
  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  display.setCursor(0, 22);
  display.print("Button: ");
  display.println(
    stableButtonState == LOW ? "PRESSED" : "RELEASED"
  );

  display.setCursor(0, 40);
  display.print("Press count: ");
  display.println(pressCount);

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin(21, 22);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED initialization failed.");
    while (true) {
      delay(1000);
    }
  }

  updateDisplay();
  Serial.println("Ready. Press the button.");
}

void loop() {
  int rawReading = digitalRead(BUTTON_PIN);

  if (rawReading != lastRawReading) {
    lastRawChangeTime = millis();
    lastRawReading = rawReading;
  }

  if (millis() - lastRawChangeTime >= DEBOUNCE_MS &&
      rawReading != stableButtonState) {

    stableButtonState = rawReading;

    if (stableButtonState == LOW) {
      pressCount++;
      Serial.print("Press count: ");
      Serial.println(pressCount);
    }

    updateDisplay();
  }
}