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
  Serial.begin(9600);  // initialise the serial communication
  rotation.begin();
  forwardsBackwards.begin();
  upDown.begin();
  claw.begin();
  Serial.println("Arduino ready");
}

char servoToActivate = -1;

void loop() {
  if (Serial.available()) {
    // save the message when it arrives
    servoToActivate = Serial.read();
    Serial.print("Servo to activate: ");
    Serial.println(servoToActivate);
  }

  switch (servoToActivate) {
    case '1':
    //ROTATION
      Serial.println("Case 1");
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        rotation.servo.write(pos);          // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        rotation.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      break;
    case '2':
      Serial.println("Case 2");
      //FORWARDS BACKWARDS
      upDown.servo.write(90);
      for (pos = 90; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        forwardsBackwards.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      for (pos = 180; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
        forwardsBackwards.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      break;

    case '3':
      Serial.println("Case 3");
    //UP DOWN
      for (pos = 90; pos >= 20; pos -= 1) { // goes from 180 degrees to 0 degrees
          upDown.servo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
      }
      for (pos = 20; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        upDown.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      break;

    case '4':
      Serial.println("Case 4");
    //CLAW
      for (pos = 180; pos >= 60; pos -= 1) { // goes from 180 degrees to 0 degrees
        claw.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }

      for (pos = 60; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        claw.servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      break;
    default:
      Serial.println("Invalid input -- no movement");
  }
}
