#include <Servo.h>

struct Joint {
  Servo servo;
  int servoPin;

  void begin() {
    servo.attach(servoPin);
  }
};

Joint rotation = { Servo(), 8 };
Joint forwardsBackwards = { Servo(), 6 };
Joint upDown = { Servo(), 9 };
Joint claw = { Servo(), 7 };
int pos = 0;

void setup() {
  Serial.begin(9600); // initialise the serial communication
  rotation.begin();   //attach to relevant servo pin for each servo
  forwardsBackwards.begin();
  upDown.begin();
  claw.begin();
  Serial.println("Arduino ready");
}

void loop() {
  //ROTATION
  Serial.println("Rotation");
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    rotation.servo.write(pos);          // tell servo to go to position in variable 'pos'
    delay(15);                          // waits 15ms for the servo to reach the position
  }

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    rotation.servo.write(pos);          // tell servo to go to position in variable 'pos'
    delay(15);                          // waits 15ms for the servo to reach the position
  }

  //FORWARDS BACKWARDS
  Serial.println("Forwards Backwards");
  upDown.servo.write(90);
  for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    forwardsBackwards.servo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);                          // waits 15ms for the servo to reach the position
  }

  for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
    forwardsBackwards.servo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }

  //UP DOWN
  Serial.println("Up Down");
  for (pos = 90; pos >= 20; pos -= 1) { // goes from 180 degrees to 0 degrees
      upDown.servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 20; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    upDown.servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  //CLAW
  Serial.println("Claw");
  for (pos = 180; pos >= 60; pos -= 1) { // goes from 100 degrees to 0 degrees
    claw.servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 60; pos <= 180; pos += 1) { // goes from 0 degrees to 100 degrees
    // in steps of 1 degree
    claw.servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
