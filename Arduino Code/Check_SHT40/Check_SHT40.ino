#include "DFRobot_SHT40.h"

DFRobot_SHT40 SHT40(0x44); 

uint32_t id = 0;
float temperature, humidity;

void setup() {
  Serial.begin(9600);
  SHT40.begin();
  while((id = SHT40.getDeviceID()) == 0){
    Serial.println("ID retrieval error, please check whether the device is connected correctly!!!");
    delay(1000);
  }
  delay(1000);
  Serial.print("id :0x"); Serial.println(id, HEX);
}

void loop() {
  temperature = SHT40.getTemperature(PRECISION_HIGH);
  humidity = SHT40.getHumidity(PRECISION_HIGH);

  if(temperature == MODE_ERR){
    Serial.println("Incorrect mode configuration to get temperature");
  } else{
    Serial.print("Temperature :"); Serial.print(temperature); Serial.println(" C");
  }
  if(humidity == MODE_ERR){
    Serial.println("The mode for getting humidity was misconfigured");
  } else{
    Serial.print("Humidity :"); Serial.print(humidity); Serial.println(" %RH");
  }

  if(humidity > 80){
    SHT40.enHeater(POWER_CONSUMPTION_H_HEATER_1S);
  }
  delay(1000);
  Serial.println("----------------------------------------");
}
