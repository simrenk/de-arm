#include <Servo.h>

struct Joint {
  Servo servo;
  int servoPin;
  int potPin;
  int prevAngle;

  void begin() {
    servo.attach(servoPin);
    prevAngle = map(analogRead(potPin), 0, 1023, 0, 180);
    servo.write(prevAngle);
  }
};

Joint claw = { Servo(), 9, A3 };
Joint forwardsBackwards = { Servo(), 10, A2 };
Joint upDown = { Servo(), 12, A1 };
Joint rotation = { Servo(), 11, A0 }; 

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  claw.begin();
  forwardsBackwards.begin();
  upDown.begin();
  rotation.begin();
}

void updatePosition(Joint &currJoint, int expectedAngle) {
  currJoint.servo.write(expectedAngle);
  currJoint.prevAngle = expectedAngle;
  Serial.print("expected angle: ");
  Serial.print(expectedAngle);
  Serial.print(", prevAngle update: ");
  Serial.println(currJoint.prevAngle);
}

bool outOfThreshold(int angle1, int angle2) {
  return (abs(angle1 - angle2) > 2);
}

void loop() {
  int clawRead = analogRead(claw.potPin);
  int forwardsBackwardsRead = analogRead(forwardsBackwards.potPin);
  int upDownRead = analogRead(upDown.potPin);
  int rotationRead = analogRead(rotation.potPin);
  Serial.print("clawRead: ");
  Serial.print(clawRead);
  Serial.print(", forwardsBackwardsRead: ");
  Serial.print(forwardsBackwardsRead);
  Serial.print(", upDownRead: ");
  Serial.print(upDownRead);
  Serial.print(", rotationRead: ");
  Serial.println(rotationRead);

  int clawExpectedAngle = map(analogRead(claw.potPin), 0, 1023, 0, 180);
  int forwardsBackwardsExpectedAngle = map(analogRead(forwardsBackwards.potPin), 0, 1023, 0, 180);
  int upDownExpectedAngle = map(analogRead(upDown.potPin), 0, 1023, 0, 180);
  int rotationExpectedAngle = map(analogRead(rotation.potPin), 0, 1023, 0, 180);

  // if (outOfThreshold(clawExpectedAngle, claw.prevAngle)) {
  if (clawExpectedAngle != claw.prevAngle) {
    Serial.println("Claw out of threshold");
    updatePosition(claw, clawExpectedAngle);
  }

  // if (outOfThreshold(forwardsBackwardsExpectedAngle, forwardsBackwards.prevAngle)) {
  if (forwardsBackwardsExpectedAngle != forwardsBackwards.prevAngle) {
    Serial.println("forwardsBackwards out of threshold");
    updatePosition(forwardsBackwards, forwardsBackwardsExpectedAngle);
  }

  // if (outOfThreshold(upDownExpectedAngle, upDown.prevAngle)) {
  if (upDownExpectedAngle != upDown.prevAngle) {
    Serial.println("upDown out of threshold");
    updatePosition(upDown, upDownExpectedAngle);
  }

  // if (outOfThreshold(rotationExpectedAngle, rotation.prevAngle)) {
  if (rotationExpectedAngle != rotation.prevAngle) {
    Serial.println("rotation out of threshold");
    updatePosition(rotation, rotationExpectedAngle);
  }
}

