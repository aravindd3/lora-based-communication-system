#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MY_ID 1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BTN_UP 32
#define BTN_DOWN 33
#define BTN_PTT 25
#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 26

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
struct Device { uint8_t id; const char* name; };
Device contacts[] = {{1, "Elon Aravind"}, {2, "Aravind Elon"}, {3, "Somante Maman"}, {4, "Mamante Soman"}};
const int totalContacts = 4;
int selectedIndex = 0;
enum State { IDLE, TRANSMITTING, RECEIVING, BUSY_LOCKED };
State currentState = IDLE;
bool channelBusy = false;
uint8_t busyBy = 0;
bool lastUpState = HIGH, lastDownState = HIGH, lastPTTState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

const char* getNameFromID(uint8_t id) {
  for (int i = 0; i < totalContacts; i++) if (contacts[i].id == id) return contacts[i].name;
  return "Unknown";
}

void drawMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Device: ");
  display.print(getNameFromID(MY_ID));
  for (int i = 0; i < totalContacts; i++) {
    int y = 16 + i * 12;
    if (i == selectedIndex) { display.fillRect(0, y, 128, 12, SSD1306_WHITE); display.setTextColor(SSD1306_BLACK); }
    else display.setTextColor(SSD1306_WHITE);
    display.setCursor(5, y + 2);
    display.print(contacts[i].name);
  }
  display.display();
}

void drawMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 30);
  display.print(msg);
  display.display();
}

void setup() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_PTT, INPUT_PULLUP);
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) while (1) {}
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  drawMenu();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize >= 3) {
    uint8_t type = LoRa.read(), sender = LoRa.read(), target = LoRa.read();
    if (type == 0x01 && sender != MY_ID) {
      channelBusy = true;
      busyBy = sender;
      if (target == MY_ID) { currentState = RECEIVING; drawMessage("Receiving from " + String(getNameFromID(sender))); }
      else { currentState = BUSY_LOCKED; drawMessage("Channel Busy"); }
    }
    if (type == 0x03) { channelBusy = false; busyBy = 0; currentState = IDLE; drawMenu(); }
  }
  bool currentUp = digitalRead(BTN_UP), currentDown = digitalRead(BTN_DOWN), currentPTT = digitalRead(BTN_PTT);
  unsigned long now = millis();
  if (currentState == IDLE && !channelBusy) {
    if (lastUpState == HIGH && currentUp == LOW && now - lastDebounceTime > debounceDelay) { selectedIndex--; if (selectedIndex < 0) selectedIndex = totalContacts - 1; drawMenu(); lastDebounceTime = now; }
    if (lastDownState == HIGH && currentDown == LOW && now - lastDebounceTime > debounceDelay) { selectedIndex++; if (selectedIndex >= totalContacts) selectedIndex = 0; drawMenu(); lastDebounceTime = now; }
    if (lastPTTState == HIGH && currentPTT == LOW && now - lastDebounceTime > debounceDelay) {
      uint8_t target = contacts[selectedIndex].id;
      if (target == MY_ID) { drawMessage("Cannot talk to self"); delay(800); drawMenu(); }
      else { LoRa.beginPacket(); LoRa.write(0x01); LoRa.write(MY_ID); LoRa.write(target); LoRa.endPacket(); currentState = TRANSMITTING; drawMessage("Talking to " + String(getNameFromID(target))); }
      lastDebounceTime = now;
    }
  }
  if (currentState == TRANSMITTING && lastPTTState == LOW && currentPTT == HIGH) {
    uint8_t target = contacts[selectedIndex].id;
    LoRa.beginPacket(); LoRa.write(0x03); LoRa.write(MY_ID); LoRa.write(target); LoRa.endPacket();
    currentState = IDLE; channelBusy = false; drawMenu();
  }
  lastUpState = currentUp;
  lastDownState = currentDown;
  lastPTTState = currentPTT;
}
