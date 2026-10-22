#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BTN_UP 4
#define BTN_PTT 5
#define BTN_DOWN 18

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const char* contacts[] = {"Elon Aravind", "Aravind Elon", "Somante Maman", "Mamante Soman"};
const int totalContacts = 4;
int selectedIndex = 0;
bool inTalkMode = false;
unsigned long talkStartTime = 0;
const unsigned long maxTalkTime = 10000;
int hours = 12;
int minutes = 0;
int batteryLevel = 75;

void drawStatusBar() {
  display.fillRect(0, 0, 128, 16, SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 4);
  if (hours < 10) display.print("0");
  display.print(hours);
  display.print(":");
  if (minutes < 10) display.print("0");
  display.print(minutes);
  int bx = 100, by = 4;
  display.drawRect(bx, by, 20, 8, SSD1306_WHITE);
  display.fillRect(bx + 20, by + 2, 2, 4, SSD1306_WHITE);
  display.fillRect(bx + 1, by + 1, map(batteryLevel, 0, 100, 0, 18), 6, SSD1306_WHITE);
}

void drawMenu() {
  display.fillRect(0, 16, 128, 48, SSD1306_BLACK);
  display.setTextSize(1);
  for (int i = 0; i < totalContacts; i++) {
    int y = 18 + i * 12;
    if (i == selectedIndex) {
      display.fillRect(0, y - 2, 128, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(5, y);
    display.print(contacts[i]);
  }
}

void drawTalkScreen(unsigned long elapsed) {
  display.fillRect(0, 16, 128, 48, SSD1306_BLACK);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 18);
  display.print("Talking to:");
  display.setCursor(0, 30);
  display.print(contacts[selectedIndex]);
  int barWidth = map(elapsed, 0, maxTalkTime, 0, 120);
  display.drawRect(4, 44, 120, 8, SSD1306_WHITE);
  display.fillRect(4, 44, barWidth, 8, SSD1306_WHITE);
  int remaining = (maxTalkTime - elapsed) / 1000;
  if (remaining < 0) remaining = 0;
  display.setCursor(50, 56);
  display.print(remaining);
  display.print("s");
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_PTT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  static unsigned long lastMinuteUpdate = 0;
  if (millis() - lastMinuteUpdate > 60000) {
    minutes++;
    if (minutes >= 60) { minutes = 0; hours++; if (hours >= 24) hours = 0; }
    lastMinuteUpdate = millis();
  }
  bool pttPressed = (digitalRead(BTN_PTT) == LOW);
  if (pttPressed && !inTalkMode) { inTalkMode = true; talkStartTime = millis(); }
  if (!pttPressed && inTalkMode) inTalkMode = false;
  display.clearDisplay();
  drawStatusBar();
  if (inTalkMode) {
    unsigned long elapsed = millis() - talkStartTime;
    if (elapsed >= maxTalkTime) { inTalkMode = false; elapsed = maxTalkTime; }
    drawTalkScreen(elapsed);
  } else {
    if (digitalRead(BTN_UP) == LOW) { selectedIndex--; if (selectedIndex < 0) selectedIndex = totalContacts - 1; delay(200); }
    if (digitalRead(BTN_DOWN) == LOW) { selectedIndex++; if (selectedIndex >= totalContacts) selectedIndex = 0; delay(200); }
    drawMenu();
  }
  display.display();
}
