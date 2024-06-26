void setup() {
  Serial1.begin(115200);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Serial communication started on Serial1 (TX1/RX1)");
}

void loop() {
  Serial1.println("Lalala");

  // const char* message = "$Rifqi%";
  // for (int i = 0; message[i] != '\0'; i++) {
  //   Serial.println(message[i]);
  //   Serial1.println(message[i]);
  //   delay(50);
  // }

  delay(1000);
}
