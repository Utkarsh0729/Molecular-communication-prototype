# Molecular Communication Using Alcohol Spray & MQ3 Sensor

This project demonstrates **molecular communication** — transmitting information using molecules as carriers instead of traditional electromagnetic signals.  
The system is split into two modules:

- **Transmitter Module**: Encodes data (letters) into binary and sends it by spraying alcohol using a servo motor.
- **Receiver Module**: Uses an MQ3 alcohol sensor to detect sprays, decode the binary data, and display the received message on an LCD.

---

## 1. Project Overview

Molecular communication is a form of communication inspired by biology, where information is encoded into chemical signals that propagate through a medium.  
Here, **alcohol vapors** serve as the carrier, a **servo motor** controls the spray, and the **MQ3 sensor** detects the vapors.

The communication is **timing-based**:
- Spray = Binary `1`
- No spray = Binary `0`
- Fixed bit duration for synchronization
- Start signal to indicate transmission start

---

## 2. Hardware Components

### Transmitter
- Arduino board
- Servo motor (controls spray nozzle)
- Spray bottle containing alcohol
- Power source

### Receiver
- Arduino board
- MQ3 alcohol sensor
- 16x2 I2C LCD
- Power source

---

## 3. Working Principle

### Step 1: Initiation
1. User enters the number of letters to send.
2. The transmitter sends a **start spray** signal.
3. Transmitter encodes the number of letters into **4 bits** and sends them.

### Step 2: Letter Transmission
1. For each letter:
   - Convert letter (`A-Z`) to a 6-bit payload (1–26 range).
   - Set the **MSB to 1** as a header/parity bit.
   - Send each bit in sequence:
     - Spray for `1`
     - No spray for `0`

### Step 3: Reception
1. MQ3 sensor detects alcohol concentration.
2. For each bit:
   - Read alcohol level halfway through bit duration.
   - Compare with threshold to decide if it's `1` or `0`.
3. Decode bits back to letters.
4. Display received message on LCD.

---

## 4. Protocol Design

- **Start Signal**: One spray before bitstream begins
- **Bit Duration**: `6000 ms`
- **Spray Duration**: `300 ms`
- **Threshold**: `900` (MQ3 sensor analog reading)
- **Letter Count Encoding**: 4-bit binary
- **Letter Encoding**: 6-bit binary (MSB = 1, next 5 bits = letter index)

---

## 5. Circuit Connection

### Transmitter
| Component       | Pin  |
|-----------------|------|
| Servo Signal    | 9    |
| Servo Power     | 5V   |
| Servo Ground    | GND  |

### Receiver
| Component            | Pin   |
|----------------------|-------|
| MQ3 Sensor Output    | A0    |
| MQ3 Sensor Power     | 5V    |
| MQ3 Sensor Ground    | GND   |
| LCD I2C SDA          | A4    |
| LCD I2C SCL          | A5    |
| LCD I2C Power        | 5V    |
| LCD I2C Ground       | GND   |

---

## 6. Code

- **Transmitter**:  
  Reads user input from Serial, encodes, and sends via spray.
- **Receiver**:  
  Continuously monitors MQ3 readings, decodes binary, and prints to LCD.

---

## 7. Usage Instructions

1. **Setup Hardware** as per circuit diagrams.
2. Upload **transmitter.ino** to the transmitter Arduino.
3. Upload **receiver.ino** to the receiver Arduino.
4. Open Serial Monitor for the transmitter.
5. Enter number of letters (0–9).
6. Enter each letter when prompted.
7. Observe received message on the LCD.

---

## 8. Limitations

- **Speed**: Very slow due to molecular diffusion delays.
- **Interference**: Sensitive to environmental airflow and alcohol residue.
- **Distance**: Effective only over short ranges (a few centimeters to meters).

---

## 9. Possible Improvements

- Use **faster-responding gas sensors**.
- Implement **error detection/correction**.
- Miniaturize system for better portability.
- Experiment with different molecules for safer indoor use.

---

## 10. License
This project is open-source and available under the MIT License.
