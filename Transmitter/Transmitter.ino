// Transmitter Code - 11-07-2025

#include <Servo.h>

Servo sprayServo;

const int servoPin = 9;
const int sprayAngle = 0;
const int restAngle = 90;
const int sprayDuration = 300;
const int bitDuration = 6000;

void setup() {
  Serial.begin(9600);
  sprayServo.attach(servoPin);
  sprayServo.write(restAngle);
  Serial.println("Transmitter Ready. Enter number of letters (0-9):");
}

void loop() {
  if (Serial.available() > 0) {
    char ch = Serial.read();

    if (ch >= '0' && ch <= '9') {
      int q = ch - '0'; // Convert ASCII to int

      Serial.print("Letters to send: ");
      Serial.println(q);
      int cases = q;
      q = q | (1<<3);

      delay(2000); // Wait before transmitting
      
      // Start signal
      spray();
      delay(2000);

      // Send 4 bits indicating number of letters (MSB to LSB)
      transmit(q, 3);

      while (cases--) {
        delay(7000);
        Serial.println("Waiting for letter...");

        while (Serial.available() == 0) {
          // Wait for next letter
        }

        char letter = Serial.read();
        Serial.print("Sending: ");
        Serial.println(letter);

        byte temp = letter - 'A' + 1; // payload from 1 to 26
        temp |= (1 << 5); // Set MSB = 1 for parity/header

        transmit(temp, 5);
        Serial.println();

      }

      Serial.println("All letters sent.");
    }
  }
}

void transmit(byte n, int x) {
  Serial.print("Encoded bits: ");
  Serial.println(n, BIN);

  for (int i = x; i >= 0; i--) {
    bool bitSet = n & (1 << i);
    Serial.print(bitSet);
    if (bitSet) {
      spray();
    } else {
      delay(bitDuration);
    }
  }
}

void spray() {
  sprayServo.write(sprayAngle);
  delay(sprayDuration);
  sprayServo.write(restAngle);
  delay(bitDuration - sprayDuration);
}
