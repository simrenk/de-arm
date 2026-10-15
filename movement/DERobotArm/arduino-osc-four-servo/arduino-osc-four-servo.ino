/*
   arduino-osc-blink

   Listens for serial messages and controls the
   the on-board LED according to the message.

   To be used with the Processing sketch
   processing_osc_to_arduino.pde

   Written by Becky Stewart 2022
*/

struct Joint {
  Servo servo;
  int servoPin;
  int prevAngle;

  void begin() {
    servo.attach(servoPin);
    prevAngle = map(analogRead(potPin), 0, 1023, 0, 180);
    servo.write(prevAngle);
  }
};

Joint claw = { Servo(), 9 };
Joint forwardsBackwards = { Servo(), 10 };
Joint upDown = { Servo(), 12 };
Joint rotation = { Servo(), 11 }; 
Joint curr;

int servoNum = -1;
int pin = 3;

void setup() {
  Serial.begin(9600);  // initialise the serial communication
  //where are we getting power from? 
  pinMode(claw.servoPin, OUTPUT);
  pinMode(forwardsBackwards.servoPin, OUTPUT);
  pinMode(upDown.servoPin, OUTPUT);
  pinMode(rotation.servoPin, OUTPUT);
}

void loop() {
  // wait for a message to arrive
  if (Serial.available()) {
    // save the message when it arrives
    servoNum = Serial.read();
  }

void updatePosition(Joint &currJoint, int expectedAngle) {
  currJoint.servo.write(expectedAngle);
  currJoint.prevAngle = expectedAngle;
  Serial.print("expected angle: ");
  Serial.print(expectedAngle);
  Serial.print(", prevAngle update: ");
  Serial.println(currJoint.prevAngle);
}

  switch (servoNum) {
    case 1:
      curr = claw;
      break;
    case 2:
      curr = forwardsBackwards;
      break;
    case 3:
      curr = upDown;
      break;
    case 4:
      curr = rotation;
    default:
      break;
  }
  int expectedAngle;
  if (curr.prevAngle == 90) {
    expectedAngle = 30;
  } else {
    expectedAngle = 90;
  }
  updatePosition(curr, expectedAngle);
}