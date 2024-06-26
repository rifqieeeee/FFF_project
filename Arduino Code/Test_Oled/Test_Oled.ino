#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment to initialize the I2C address, uncomment only one. If you get a totally blank screen try the other */
#define i2c_Address 0x3c // Initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d // Initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  delay(250); // Wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default

  display.display();
  delay(2000);

  display.clearDisplay();
}

void loop() {
  int dp = 15;
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0+dp, 0);
  display.setTextSize(2);
  display.println("FFF");

  display.setCursor(40+dp, 0);
  display.setTextSize(1);
  display.println("PROJECT");

  display.setCursor(40+dp, 10);
  display.setTextSize(1);
  display.println("MONITORING");

  display.setCursor(0, 30);
  display.setTextSize(1);
  display.println("CH4   = 123 - safe");

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.println("Temp  = 123 - safe");

  display.setCursor(0, 50);
  display.setTextSize(1);
  display.println("Smoke = 123 - safe");

  // Display the text
  display.display();
  delay(2000);
  display.clearDisplay();  
}

