#include <M5Core2.h>
#include <JPEGDecoder.h>
#include <time.h>
#include <SD.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <AXP192.h> // Include the AXP192 library

// Define custom color
#define TFT_DARKBLUE 0x001F // Define dark blue color

// Wi-Fi credentials
const char* ssid = "";
const char* password = "";

// NTP client setup
WiFiUDP ntpUDP;
// Set timezone offset to 9 hours (32400 seconds) for Japan Standard Time (JST)
NTPClient timeClient(ntpUDP, "pool.ntp.org", 9 * 60 * 60, 60000);

// Character appearances
enum CharacterAppearance {
  APPEARANCE1,
  APPEARANCE2,
  APPEARANCE3,
  APPEARANCE4,
  APPEARANCE5,
  APPEARANCE6, // Added APPEARANCE6 for special condition
};

// Current character appearance
CharacterAppearance currentAppearance = APPEARANCE1;

// Command types
enum CommandType {
  STATUS_DISPLAY,
  FEED,
  PLAY,
  EXERCISE,
  BATH,
  COMMAND_COUNT // Special value to get the command count
};

// Selected command
CommandType selectedCommand = STATUS_DISPLAY;

// Timer
unsigned long lastActionTime = 0;
const unsigned long actionInterval = 5000; // Action every 5 seconds

// Pet status
int health = 100;
int happiness = 100;
int hunger = 0;
int favorability = 50;
unsigned long startTime = 0; // Initialize to zero

// Last interaction time
unsigned long lastInteractionTime = 0;
const unsigned long interactionInterval = 60 * 60 * 1000; // 1 hour

// Function to draw JPG image
void drawJpgImage(const char* filename, int x, int y) {
  M5.Lcd.drawJpgFile(SD, filename, x, y);
}

// Function to draw centered JPG image
void drawCenteredJpgImage(const char* filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  JpegDec.decodeSdFile(file);
  int jpegWidth = JpegDec.width;
  int jpegHeight = JpegDec.height;

  int16_t x = (M5.Lcd.width() - jpegWidth) / 2;
  int16_t y = (M5.Lcd.height() - jpegHeight) / 2;

  M5.Lcd.drawJpgFile(SD, filename, x, y);
}

void showStatus() {
  M5.Lcd.clear();
  drawJpgImage("/background.jpg", 0, 0); // Background image
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.drawString("Health: " + String(health), 160, 50);
  M5.Lcd.drawString("Happiness: " + String(happiness), 160, 70);
  M5.Lcd.drawString("Hunger: " + String(hunger), 160, 90);
  M5.Lcd.drawString("Favorable: " + String(favorability), 160, 110);
}

void feedPet() {
  M5.Lcd.clear();
  drawJpgImage("/background.jpg", 0, 0); // Background image
  String image = "/feed" + String(random(1, 4)) + ".jpg";
  drawCenteredJpgImage(image.c_str()); // Random appearance image
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.drawString("Fed the pet!", 160, 50); // Same height as status display start
  hunger = max(0, hunger - 20);
  happiness = min(100, happiness + 10);
  favorability = min(100, favorability + 5);
  lastInteractionTime = millis(); // Update last interaction time
}

void playWithPet() {
  M5.Lcd.clear();
  drawJpgImage("/background.jpg", 0, 0); // Background image
  String image = "/play" + String(random(1, 4)) + ".jpg";
  drawCenteredJpgImage(image.c_str()); // Random appearance image
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.drawString("Played with the pet!", 160, 50); // Same height as status display start
  happiness = min(100, happiness + 20);
  favorability = min(100, favorability + 10);
  lastInteractionTime = millis(); // Update last interaction time
}

void exercisePet() {
  M5.Lcd.clear();
  drawJpgImage("/background.jpg", 0, 0); // Background image
  String image = "/exercise" + String(random(1, 4)) + ".jpg";
  drawCenteredJpgImage(image.c_str()); // Random exercise image
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.drawString("Exercised the pet!", 160, 50); // Same height as status display start
  health = min(100, health + 10);
  happiness = min(100, happiness + 10);
  lastInteractionTime = millis(); // Update last interaction time
}

void bathPet() {
  M5.Lcd.clear();
  drawJpgImage("/background.jpg", 0, 0); // Background image
  drawCenteredJpgImage("/bath.jpg"); // Single bath image
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.drawString("Bathed the pet!", 160, 50); // Same height as status display start
  health = min(100, health + 10);
  happiness = min(100, happiness + 20);
  lastInteractionTime = millis(); // Update last interaction time
}

void cancelCommand() {
  // Clear command selection
  selectedCommand = STATUS_DISPLAY;
}

void connectWiFi() {
  M5.Lcd.setTextColor(TFT_DARKBLUE);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawString("Connecting to WiFi...", 160, 120);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int wifi_timeout = 10000; // 10 seconds timeout
  int startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifi_timeout) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" Failed to connect to WiFi");
    M5.Lcd.drawString("Failed to connect to WiFi", 160, 140);
    delay(2000);
  } else {
    Serial.println(" Connected to WiFi");
    timeClient.begin();
    timeClient.update();
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours = timeClient.getHours();
    TimeStruct.Minutes = timeClient.getMinutes();
    TimeStruct.Seconds = timeClient.getSeconds();
    M5.Rtc.SetTime(&TimeStruct);
  }
}

void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);

  // Disable automatic power off
  M5.Axp.SetSleep(0); // Disable sleep

  // Initialize RTC
  M5.Rtc.begin();

  // Initialize SD card
  if (!SD.begin()) {
    M5.Lcd.println("SD Card initialization failed.");
    while (1);
  }

  // Load last saved state
  loadState();

  // If startTime is zero (first run), initialize it
  if (startTime == 0) {
    startTime = millis();
  }
  
  // Set start time
  lastInteractionTime = millis(); // Initialize last interaction time

  // Connect to WiFi and get NTP time
  connectWiFi();

  // Initial display update
  updateDisplay();
}

void loop() {
  M5.update();

  // Check button state
  if (M5.BtnA.wasPressed()) {
    selectCommand();
    updateDisplay();
  }

  if (M5.BtnB.wasPressed()) {
    executeSelectedCommand();
    delay(5000); // Wait for 5 seconds
    updateDisplay();
  }

  if (M5.BtnC.wasPressed()) {
    cancelCommand();
    updateDisplay();
  }

  unsigned long currentTime = millis();

  // Update status and appearance
  updateCharacterAppearance();
  checkInactivity();

  // Update clock, battery, and background every minute
  static unsigned long lastUpdateTime = 0;
  if (currentTime - lastUpdateTime >= 60 * 1000) {
    updateDisplay();
    lastUpdateTime = currentTime;
  }

  // Auto-save every 5 minutes
  if (currentTime - lastActionTime >= 5 * 60 * 1000) {
    saveState();
    lastActionTime = currentTime;
  }
}

void updateClockAndBattery() {
  RTC_TimeTypeDef TimeStruct;
  M5.Rtc.GetTime(&TimeStruct);
  char timeStringBuff[6];
  sprintf(timeStringBuff, "%02d:%02d", TimeStruct.Hours, TimeStruct.Minutes);

  // Display current time
  M5.Lcd.setTextDatum(TC_DATUM); // Top center-align text
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue
  M5.Lcd.drawString(timeStringBuff, 160, 20, GFXFF);

  // Display battery level
  int batteryPercentage = M5.Axp.GetBatVoltage() * 100 / 4.2;
  M5.Lcd.setTextDatum(TR_DATUM); // Top right-align text
  M5.Lcd.drawString(String(batteryPercentage) + "%", 310, 20, GFXFF);
}

void selectCommand() {
  // Select command
  selectedCommand = static_cast<CommandType>((selectedCommand + 1) % COMMAND_COUNT);
}

void executeSelectedCommand() {
  // Execute selected command
  switch (selectedCommand) {
    case STATUS_DISPLAY:
      showStatus();
      break;
    case FEED:
      feedPet();
      break;
    case PLAY:
      playWithPet();
      break;
    case EXERCISE:
      exercisePet();
      break;
    case BATH:
      bathPet();
      break;
    default:
      break;
  }
}

void updateDisplay() {
  // Get current time
  RTC_TimeTypeDef TimeStruct;
  M5.Rtc.GetTime(&TimeStruct);
  int currentHour = TimeStruct.Hours;

  // Display text based on game state
  M5.Lcd.fillRect(0, 20, 320, 240, TFT_BLACK); // Clear screen except for the time display area
  drawJpgImage("/background.jpg", 0, 0); // Background image

  // Display current time and battery level
  updateClockAndBattery();

  M5.Lcd.setTextDatum(MC_DATUM); // Center-align text
  M5.Lcd.setTextColor(TFT_DARKBLUE); // Set text color to dark blue

  if (currentHour >= 0 && currentHour < 8) {
    // Display sleep image between 0:00 and 8:00
    drawCenteredJpgImage("/sleep.jpg");
  } else {
    switch (currentAppearance) {
      case APPEARANCE1:
        drawCenteredJpgImage("/appearance1.jpg");
        break;
      case APPEARANCE2:
        drawCenteredJpgImage("/appearance2.jpg");
        break;
      case APPEARANCE3:
        drawCenteredJpgImage("/appearance3.jpg");
        break;
      case APPEARANCE4:
        drawCenteredJpgImage("/appearance4.jpg");
        break;
      case APPEARANCE5:
        drawCenteredJpgImage("/appearance5.jpg");
        break;
      case APPEARANCE6:
        drawCenteredJpgImage("/appearance6.jpg");
        break;
    }
  }

  // Display command menu
  switch (selectedCommand) {
    case STATUS_DISPLAY:
      M5.Lcd.drawString("> STATUS DISPLAY", 160, 220, GFXFF);
      break;
    case FEED:
      M5.Lcd.drawString("> FEED", 160, 220, GFXFF);
      break;
    case PLAY:
      M5.Lcd.drawString("> PLAY", 160, 220, GFXFF);
      break;
    case EXERCISE:
      M5.Lcd.drawString("> EXERCISE", 160, 220, GFXFF);
      break;
    case BATH:
      M5.Lcd.drawString("> BATH", 160, 220, GFXFF);
      break;
    default:
      break;
  }
}

void updateCharacterAppearance() {
  if (happiness <= 50) {
    currentAppearance = APPEARANCE2;
  } else if (hunger >= 70) {
    currentAppearance = APPEARANCE3;
  } else if (health <= 50) {
    currentAppearance = APPEARANCE4;
  } else if (favorability <= 50) {
    currentAppearance = APPEARANCE5;
  } else if (favorability == 100) {
    currentAppearance = APPEARANCE6;
  } else {
    currentAppearance = APPEARANCE1;
  }
}

void checkInactivity() {
  unsigned long currentTime = millis();
  if (currentTime - lastInteractionTime >= interactionInterval) {
    // Decrease health and happiness, increase hunger
    health = max(0, health - 5);
    happiness = max(0, happiness - 5);
    hunger = min(100, hunger + 5);
    favorability = max(0, favorability - 5); // Reduce favorability every hour

    // Update last interaction time to avoid multiple decrements within a short time
    lastInteractionTime = currentTime;
  }
}

void saveState() {
  File file = SD.open("/pet_state.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  file.printf("Health:%d\n", health);
  file.printf("Happiness:%d\n", happiness);
  file.printf("Hunger:%d\n", hunger);
  file.printf("Favorability:%d\n", favorability);
  file.printf("Appearance:%d\n", currentAppearance);
  file.close();
}

void loadState() {
  File file = SD.open("/pet_state.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    int separatorIndex = line.indexOf(':');
    if (separatorIndex == -1) continue;

    String key = line.substring(0, separatorIndex);
    unsigned long value = line.substring(separatorIndex + 1).toInt();

    if (key == "Health") health = value;
    else if (key == "Happiness") happiness = value;
    else if (key == "Hunger") hunger = value;
    else if (key == "Favorability") favorability = value;
    else if (key == "Appearance") currentAppearance = static_cast<CharacterAppearance>(value);
  }
  file.close();
}
