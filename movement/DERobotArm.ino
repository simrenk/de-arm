#include <Servo.h>

struct Joint {
  Servo servo;
  int servoPin;
  int potPin;
  int prevAngle;

  void begin() {
    servo.attach(servoPin);
    prevAngle = map(analogRead(potPin), 0, 1023, 0, 180);
  }
};

Joint claw = { Servo(), 6, A3};
Joint forwardsBackwards = { Servo(), 7, A2 };
Joint upDown = { Servo(), 8, A1};
Joint rotation = { Servo(), 9, A0 }; 

void setup() {
  Serial.begin(9600);
  claw.begin();
  forwardsBackwards.begin();
  upDown.begin();
  rotation.begin();
}

void updatePosition(Joint &currJoint, int expectedAngle) {
  currJoint.servo.write(expectedAngle);
  currJoint.prevAngle = expectedAngle;
}

void loop() {
  int clawExpectedAngle = map(analogRead(claw.potPin), 0, 1023, 0, 180);
  int forwardsBackwardsExpectedAngle = map(analogRead(forwardsBackwards.potPin), 0, 1023, 0, 180);
  int upDownExpectedAngle = map(analogRead(upDown.potPin), 0, 1023, 0, 180);
  int rotationExpectedAngle = map(analogRead(rotation.potPin), 0, 1023, 0, 180);

  if (clawExpectedAngle != claw.prevAngle) {
    updatePosition(claw, clawExpectedAngle);
  }

  if (forwardsBackwardsExpectedAngle != forwardsBackwards.prevAngle) {
    updatePosition(forwardsBackwards, forwardsBackwardsExpectedAngle);
  }

  if (upDownExpectedAngle != upDown.prevAngle) {
    updatePosition(upDown, upDownExpectedAngle);
  }

  if (rotationExpectedAngle != rotation.prevAngle) {
    updatePosition(rotation, rotationExpectedAngle);
  }
}

