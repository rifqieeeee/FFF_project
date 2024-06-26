#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "DFRobot_SHT40.h"

DFRobot_SHT40 SHT40(0x44); 

uint32_t id = 0;
int temperature, humidity;
int sensorPin = A1;
int smoke_data = 0;
int CH4_pin = A0;
int CH4Value = 0;

/* Uncomment to initialize the I2C address, uncomment only one. If you get a totally blank screen try the other */
#define i2c_Address 0x3c // Initialize with the I2C addr 0x3C Typically eBay OLED's
// #define i2c_Address 0x3d // Initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);

  SHT40.begin();
  while((id = SHT40.getDeviceID()) == 0){
    Serial.println("ID retrieval error, please check whether the device is connected correctly!!!");
    delay(1000);
  }
  delay(1000);
  Serial.print("id :0x"); Serial.println(id, HEX);
  
  delay(250); // Wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default

  display.display();
  delay(2000);

  display.clearDisplay();
}

void loop() {
  temperature = SHT40.getTemperature(PRECISION_HIGH);
  humidity = SHT40.getHumidity(PRECISION_HIGH);
  
  smoke_data = analogRead(sensorPin);
  CH4Value = analogRead(CH4_pin);

  // Serial.print("Temperature : "); Serial.print(temperature); Serial.println(" C");
  // Serial.print("Humidity : "); Serial.print(humidity); Serial.println(" %RH");
  // Serial.print("Smoke : "); Serial.println(smoke_data);
  // Serial.print("CH4 : "); Serial.println(CH4Value);

  char data_send[50];
  sprintf(data_send, "$%d,%d,%d,&", CH4Value, temperature,smoke_data);
  // Serial.println(data_send);
  for (int i = 0; data_send[i] != '\0'; i++) {
    Serial.println(data_send[i]);
    Serial1.println(data_send[i]);
    delay(50);
  }
  delay(1000);

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
  display.print("CH4   = ");
  display.println(CH4Value);
  // display.println(" - safe");

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print("Temp  = ");
  display.print(temperature);
  display.println(" C ");
  // display.print(humidity);
  // display.println("%RH - safe");

  display.setCursor(0, 50);
  display.setTextSize(1);
  display.print("Smoke = ");
  display.print(smoke_data);

  // Display the text
  display.display();
  delay(5000);
  display.clearDisplay();  
}
