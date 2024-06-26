int i;

void setup() {
  // Initialize the Serial monitor at 115200 baud rate
  Serial.begin(115200);
  // Initialize Serial1 at 115200 baud rate
  Serial1.begin(115200);

  // Give some time to establish the serial connections
  delay(1000);
}

void loop() {
  // Check if data is available to read from Serial1
  if (Serial1.available() > 0) {
    // Read the incoming byte from Serial1
    char incomingByte = Serial1.read();
    // Print the incoming byte to the Serial monitor
    Serial.print("Received: ");
    Serial.println(incomingByte);
  }

  // Optional: Send data to Serial1 for testing
  if (Serial.available() > 0) {
    char outgoingByte = Serial.read();
    Serial1.print(outgoingByte);
    delay(10); // Small delay to ensure data transmission
  }
}
