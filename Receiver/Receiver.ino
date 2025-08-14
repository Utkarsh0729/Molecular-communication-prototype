// Receiver Code - 11-07-2025

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD with I2C
const int mq3Pin = A0;

const int threshold = 900;
const int readAfter = 2500;
const int bitDuration = 6000;

String message = "";
bool messageReceived = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Receiver Ready");
}

void loop() {
  if (messageReceived) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received:");
    lcd.setCursor(0, 1);
    lcd.print(message);
    delay(10000);
    return;
  }

  int val = analogRead(mq3Pin);
  Serial.print("Alcohol Level: ");
  Serial.println(val);

  if (val > threshold) {
    message = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Starting");
    Serial.println("Start Detecting");

    // --- Get letter count ---
    readBit();
    delay(1500);

    while (analogRead(mq3Pin) <= threshold) {
      //wait
    }

    Serial.print("Encoded bits: ");
    byte countBits = 0;
    for (int i = 3; i >= 0; i--) {
      bool bitSet = readBit();
      if (bitSet) countBits |= (1 << i);
      Serial.println(bitSet);
    }
    Serial.println();

    int numLetters = countBits;
    numLetters = numLetters & (~(1<<3));
    Serial.print("Letters to send: ");
    Serial.println(numLetters);
    delay(500);

    while (numLetters > 0) {
      delay(6500);
      // delay(6000);
      Serial.println("Waiting for letter...");
      Serial.print("Alcohol Level: ");
      Serial.println(analogRead(mq3Pin));

      // Wait for spray
      while (analogRead(mq3Pin) <= threshold) {
        // wait
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Received:");
      lcd.setCursor(0, 1);
      lcd.print(message);
      Serial.print("Encoded bits: ");

      byte bits = 0;
      for (int i = 5; i >= 0; i--) {
        bool bitSet = readBit();
        if (bitSet) bits |= (1 << i);
        Serial.println(bitSet);
      }
      Serial.println();

      byte payload = bits & ~(1 << 5); // Remove MSB
      char letter = (char)(payload - 1 + 'A');

      Serial.print("Decoded: ");
      Serial.println(letter);

      message += letter;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Received:");
      lcd.setCursor(0, 1);
      lcd.print(message);

      numLetters--;
    }

    Serial.println("All letters received.");
    messageReceived = true; // STOP further detection
  }

  delay(1000);
}

bool readBit() {
  // Wait a small buffer before sampling to ensure it's the middle of bit duration
  int val = analogRead(mq3Pin);
  Serial.print("Sampled Alcohol Level: ");
  Serial.println(val);
  
  delay(readAfter);

  val = analogRead(mq3Pin);
  Serial.print("Sampled Alcohol Level: ");
  Serial.println(val);
  delay(bitDuration-readAfter);

  return val > threshold;  // Simple 1-bit decision
}