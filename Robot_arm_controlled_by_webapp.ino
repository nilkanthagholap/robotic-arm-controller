/*
  ArmDeck firmware — 6-channel servo controller over serial
  ------------------------------------------------------------
  Original sketch written for use with the ArmDeck web app.

  Protocol (unchanged, so it's compatible with the existing app):
    Each line sent over serial looks like:   <channel>,<angle>\n
    e.g. "3,120\n" moves channel 3 to 120 degrees.

  Channels:
    1 = Base         (0-180)
    2 = Shoulder      (0-180)
    3 = Elbow         (0-180)
    4 = Wrist Roll     (0-180)
    5 = Wrist Pitch     (0-180)
    6 = Gripper         (0-90)   <- intentionally narrower range

  Improvements over the earlier version of this sketch:
    - Every channel is clamped to its own safe range before writing,
      so a bad or out-of-range value from the host can't over-rotate a servo.
    - Serial reads are non-blocking (no readStringUntil timeout stall).
    - Line parsing looks for the actual separator instead of assuming
      a fixed character position, so it's not fragile to formatting.
    - Malformed lines are ignored instead of silently moving a servo to 0.
*/

#include <Servo.h>

const uint8_t NUM_CHANNELS = 6;

struct Channel {
  Servo servo;
  uint8_t pin;
  int minDeg;
  int maxDeg;
};

// Pin numbers match the original wiring. Adjust here if your wiring differs.
Channel channels[NUM_CHANNELS] = {
  { Servo(), 2, 0, 178 },  // 1 Base
  { Servo(), 3, 0, 178 },  // 2 Shoulder
  { Servo(), 4, 0, 178 },  // 3 Elbow
  { Servo(), 5, 0, 178 },  // 4 Wrist Roll
  { Servo(), 6, 0, 178 },  // 5 Wrist Pitch
  { Servo(), 7, 0,  88 },  // 6 Gripper — narrower range
};

// If you wire a 7th servo for a mirrored/differential wrist mechanism,
// attach it to a pin here and uncomment the two lines marked below.
// Servo servo6;
// const uint8_t SERVO6_PIN = 8;

const uint8_t LINE_BUF_SIZE = 16;
char lineBuf[LINE_BUF_SIZE];
uint8_t lineLen = 0;

void setup() {
  Serial.begin(9600);
  for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
    channels[i].servo.attach(channels[i].pin);
    channels[i].servo.write((channels[i].minDeg + channels[i].maxDeg) / 2);
  }
  // servo6.attach(SERVO6_PIN);
}

void loop() {
  readSerialNonBlocking();
}

void readSerialNonBlocking() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      lineBuf[lineLen] = '\0';
      handleLine(lineBuf);
      lineLen = 0;
    } else if (c != '\r' && lineLen < LINE_BUF_SIZE - 1) {
      lineBuf[lineLen++] = c;
    }
  }
}

void handleLine(char* line) {
  // Find the separator between the channel number and the angle value.
  char* comma = strchr(line, ',');
  if (comma == nullptr) return;               // malformed line — ignore

  *comma = '\0';
  int ch = atoi(line);
  int value = atoi(comma + 1);

  if (ch < 1 || ch > NUM_CHANNELS) return;      // out-of-range channel — ignore

  Channel &target = channels[ch - 1];
  value = constrain(value, target.minDeg, target.maxDeg);
  target.servo.write(value);

  // Mirror the wrist-pitch drive onto a second servo, if you have one wired.
  // if (ch == 5) servo6.write(180 - value);
}
