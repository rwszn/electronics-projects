#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int powerPin = 7, upPin = 6, downPin = 5, okPin = 4, ledPin = 8, buzzerPin = 9;
struct Button { int pin; bool stableState; bool lastReading; unsigned long lastChange; };
Button powerButton = {7, HIGH, HIGH, 0};
Button upButton = {6, HIGH, HIGH, 0};
Button downButton = {5, HIGH, HIGH, 0};
Button okButton = {4, HIGH, HIGH, 0};
bool screenOn = false, ledOn = false, buzzerOn = false;
enum Page { MAIN_MENU, LED_MENU, BUZZER_MENU, FIREWORKS, SETTINGS_MENU };
Page currentPage = MAIN_MENU;
int mainSelection = 0, ledSelection = 0, buzzerSelection = 0, settingsSelection = 0;
bool editingSetting = false;
int brightness = 75, animationSpeed = 3, volume = 60;
int fireworkX = 64, fireworkY = 30, fireworkFrame = 0;
unsigned long nextFireworkFrame = 0;
bool tonePlaying = false;
unsigned long toneEndTime = 0;
const int fireworksNotes[] = {523, 659, 784, 1047, 1319, 1568, 1319};
const int fireworksDurations[] = {75, 75, 75, 90, 90, 150, 180};
const int fireworksMelodyLength = 7;
bool fireworksMelodyActive = false;
int fireworksMelodyStep = 0;
unsigned long nextMelodyAction = 0;

bool buttonPressed(Button &button) {
  bool reading = digitalRead(button.pin);
  if (reading != button.lastReading) { button.lastReading = reading; button.lastChange = millis(); }
  if (millis() - button.lastChange > 30) {
    if (reading != button.stableState) {
      button.stableState = reading;
      if (button.stableState == LOW) return true;
    }
  }
  return false;
}

void stopBuzzerTone() {
  TCCR1A = 0; TCCR1B = 0; digitalWrite(buzzerPin, LOW); tonePlaying = false;
}
void startTone(int frequency, int duration) {
  if (!buzzerOn || volume <= 0 || frequency <= 0) { stopBuzzerTone(); return; }
  pinMode(buzzerPin, OUTPUT);
  unsigned long top = F_CPU / (8UL * frequency) - 1;
  if (top > 65535) top = 65535;
  if (top < 2) top = 2;
  TCCR1A = 0; TCCR1B = 0; TCNT1 = 0;
  ICR1 = top;
  OCR1A = ((top + 1) * volume) / 200;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
  toneEndTime = millis() + duration;
  tonePlaying = true;
}
void updateTone() {
  if (tonePlaying && (long)(millis() - toneEndTime) >= 0) stopBuzzerTone();
}
void startFireworksMelody() {
  if (!buzzerOn || volume <= 0) { fireworksMelodyActive = false; return; }
  fireworksMelodyActive = true; fireworksMelodyStep = 0; nextMelodyAction = millis();
}
void stopFireworksMelody() { fireworksMelodyActive = false; stopBuzzerTone(); }
void updateFireworksMelody() {
  if (!fireworksMelodyActive) return;
  if (!screenOn || currentPage != FIREWORKS || !buzzerOn || volume <= 0) { stopFireworksMelody(); return; }
  if (tonePlaying) return;
  if ((long)(millis() - nextMelodyAction) < 0) return;
  if (fireworksMelodyStep >= fireworksMelodyLength) { fireworksMelodyActive = false; return; }
  startTone(fireworksNotes[fireworksMelodyStep], fireworksDurations[fireworksMelodyStep]);
  fireworksMelodyStep++;
  nextMelodyAction = toneEndTime + 35;
}
void buttonSound() { startTone(1200, 25); }

void applyBrightness() {
  int contrast = map(brightness, 0, 100, 0, 255);
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(contrast);
}
void drawMainMenu() {
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0); display.println("CONTROL PANEL"); display.drawLine(0, 11, 127, 11, SSD1306_WHITE);
  const char* options[] = {"LED CONTROL", "BUZZER CONTROL", "FIREWORKS", "SETTINGS"};
  for (int i = 0; i < 4; i++) { display.setCursor(10, 15 + (i * 12)); display.print(mainSelection == i ? "> " : "  "); display.println(options[i]); }
  display.display();
}
void drawLEDMenu() {
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 0); display.println("LED CONTROL"); display.drawLine(0, 11, 127, 11, SSD1306_WHITE);
  display.setCursor(10, 18); display.print("LED STATUS: "); display.println(ledOn ? "ON" : "OFF");
  display.setCursor(10, 34); display.print(ledSelection == 0 ? "> " : "  "); display.println("TOGGLE LED");
  display.setCursor(10, 48); display.print(ledSelection == 1 ? "> " : "  "); display.println("BACK"); display.display();
}
void drawBuzzerMenu() {
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0); display.println("BUZZER CONTROL"); display.drawLine(0, 11, 127, 11, SSD1306_WHITE);
  display.setCursor(10, 18); display.print("BUZZER: "); display.println(buzzerOn ? "ON" : "OFF");
  display.setCursor(10, 34); display.print(buzzerSelection == 0 ? "> " : "  "); display.println("TOGGLE BUZZER");
  display.setCursor(10, 48); display.print(buzzerSelection == 1 ? "> " : "  "); display.println("BACK"); display.display();
}
void drawSettingsMenu() {
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(35, 0); display.println("SETTINGS"); display.drawLine(0, 11, 127, 11, SSD1306_WHITE);
  display.setCursor(5, 15); display.print(settingsSelection == 0 ? "> " : "  "); display.print("BRIGHTNESS: "); display.print(brightness); display.println("%");
  display.setCursor(5, 27); display.print(settingsSelection == 1 ? "> " : "  "); display.print("SPEED: "); display.println(animationSpeed);
  display.setCursor(5, 39); display.print(settingsSelection == 2 ? "> " : "  "); display.print("VOLUME: "); display.print(volume); display.println("%");
  display.setCursor(5, 51); display.print(settingsSelection == 3 ? "> " : "  "); display.println("BACK"); display.display();
}
void startFireworks() {
  fireworkX = random(12, 116); fireworkY = random(20, 52); fireworkFrame = 0; nextFireworkFrame = 0;
  startFireworksMelody();
}
void updateFireworks() {
  unsigned long interval = map(animationSpeed, 1, 5, 150, 40);
  if (millis() < nextFireworkFrame) return;
  nextFireworkFrame = millis() + interval;
  if (fireworkFrame > 12) startFireworks();
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE);
  display.setCursor(38, 0); display.println("FIREWORKS"); display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  int radius = fireworkFrame * 2;
  for (int i = 0; i < 12; i++) {
    float angle = i * 0.523599;
    int x = fireworkX + cos(angle) * radius, y = fireworkY + sin(angle) * radius;
    if (x >= 0 && x < 128 && y >= 12 && y < 64) display.drawPixel(x, y, SSD1306_WHITE);
  }
  for (int i = 0; i < 4; i++) {
    int x = fireworkX + random(-radius - 2, radius + 3), y = fireworkY + random(-radius - 2, radius + 3);
    if (x >= 0 && x < 128 && y >= 12 && y < 64) display.drawPixel(x, y, SSD1306_WHITE);
  }
  display.display(); fireworkFrame++;
}
void setup() {
  pinMode(powerPin, INPUT_PULLUP); pinMode(upPin, INPUT_PULLUP); pinMode(downPin, INPUT_PULLUP); pinMode(okPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); digitalWrite(ledPin, LOW);
  pinMode(buzzerPin, OUTPUT); digitalWrite(buzzerPin, LOW);
  randomSeed(analogRead(A0));
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { while (true) {} }
  display.clearDisplay(); display.display(); display.ssd1306_command(SSD1306_DISPLAYOFF);
}
void loop() {
  updateTone(); updateFireworksMelody();
  bool powerPressed = buttonPressed(powerButton), upPressed = buttonPressed(upButton), downPressed = buttonPressed(downButton), okPressed = buttonPressed(okButton);
  if (powerPressed || upPressed || downPressed || okPressed) buttonSound();
  if (powerPressed) {
    screenOn = !screenOn;
    if (screenOn) { currentPage = MAIN_MENU; mainSelection = 0; display.ssd1306_command(SSD1306_DISPLAYON); applyBrightness(); drawMainMenu(); }
    else { stopFireworksMelody(); display.clearDisplay(); display.display(); display.ssd1306_command(SSD1306_DISPLAYOFF); }
  }
  if (!screenOn) return;
  if (currentPage == MAIN_MENU) {
    if (upPressed) { mainSelection = (mainSelection + 3) % 4; drawMainMenu(); }
    if (downPressed) { mainSelection = (mainSelection + 1) % 4; drawMainMenu(); }
    if (okPressed) {
      if (mainSelection == 0) { currentPage = LED_MENU; ledSelection = 0; drawLEDMenu(); }
      else if (mainSelection == 1) { currentPage = BUZZER_MENU; buzzerSelection = 0; drawBuzzerMenu(); }
      else if (mainSelection == 2) { currentPage = FIREWORKS; startFireworks(); }
      else if (mainSelection == 3) { currentPage = SETTINGS_MENU; settingsSelection = 0; editingSetting = false; drawSettingsMenu(); }
    }
  }
  else if (currentPage == LED_MENU) {
    if (upPressed || downPressed) { ledSelection = 1 - ledSelection; drawLEDMenu(); }
    if (okPressed) {
      if (ledSelection == 0) { ledOn = !ledOn; digitalWrite(ledPin, ledOn ? HIGH : LOW); drawLEDMenu(); }
      else { currentPage = MAIN_MENU; drawMainMenu(); }
    }
  }
  else if (currentPage == BUZZER_MENU) {
    if (upPressed || downPressed) { buzzerSelection = 1 - buzzerSelection; drawBuzzerMenu(); }
    if (okPressed) {
      if (buzzerSelection == 0) {
        buzzerOn = !buzzerOn; drawBuzzerMenu();
        if (buzzerOn) startTone(1500, 80);
        else stopFireworksMelody();
      } else { currentPage = MAIN_MENU; drawMainMenu(); }
    }
  }
  else if (currentPage == SETTINGS_MENU) {
    if (editingSetting) {
      bool changed = false;
      if (upPressed) {
        if (settingsSelection == 0 && brightness < 100) { brightness += 5; changed = true; }
        if (settingsSelection == 1 && animationSpeed < 5) { animationSpeed++; changed = true; }
        if (settingsSelection == 2 && volume < 100) { volume += 5; changed = true; }
      }
      if (downPressed) {
        if (settingsSelection == 0 && brightness > 0) { brightness -= 5; changed = true; }
        if (settingsSelection == 1 && animationSpeed > 1) { animationSpeed--; changed = true; }
        if (settingsSelection == 2 && volume > 0) { volume -= 5; changed = true; }
      }
      if (changed) {
        applyBrightness();
        if (settingsSelection == 2) startTone(1000, 45);
        drawSettingsMenu();
      }
      if (okPressed) { editingSetting = false; drawSettingsMenu(); }
    } else {
      if (upPressed) { settingsSelection = (settingsSelection + 3) % 4; drawSettingsMenu(); }
      if (downPressed) { settingsSelection = (settingsSelection + 1) % 4; drawSettingsMenu(); }
      if (okPressed) {
        if (settingsSelection == 3) { currentPage = MAIN_MENU; drawMainMenu(); }
        else { editingSetting = true; drawSettingsMenu(); }
      }
    }
  }
  else if (currentPage == FIREWORKS) {
    if (okPressed) { stopFireworksMelody(); currentPage = MAIN_MENU; drawMainMenu(); }
    else updateFireworks();
  }
}
