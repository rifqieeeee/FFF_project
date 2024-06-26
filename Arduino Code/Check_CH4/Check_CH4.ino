int CH4_pin = A1;
int CH4Value = 0;

void setup()
{
  Serial.begin(9600); //Set serial baud rate to 9600 bps
}

void loop()
{
  CH4Value = analogRead(CH4_pin);
  Serial.println(CH4Value);
  delay(100);
}