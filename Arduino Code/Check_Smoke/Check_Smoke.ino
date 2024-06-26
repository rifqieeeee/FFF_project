int CH4_pin = A1;
int CH4Value = 0;

void setup()
{
  Serial.begin(9600); //Set serial baud rate to 9600 bps
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(100);
}