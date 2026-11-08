#include <Servo.h>

struct Goo {
  //Try to understand this more
  Servo servo;
  int pin;
  int stopMicroSeconds;                  
  int screwInMicroSeconds;
  int screwOutMicroSeconds;

  void begin(int pin, int stopMicroSeconds, int screwInMicroSeconds, int screwOutMicroSeconds) {
    this->pin = pin;
    this->stopMicroSeconds = stopMicroSeconds;
    this->screwInMicroSeconds = screwInMicroSeconds;
    this->screwOutMicroSeconds = screwOutMicroSeconds;
    servo.attach(this->pin);
  }
};

struct Tentacles {
  int in1;
  int in2;
  int enable; //PWM pin
  int slowedSpeed; // 0-255

  void begin(int in1, int in2, int enable, int slowedSpeed) { //is A0 etc actually correctly represented as an int
    this->in1 = in1;
    this->in2 = in2;
    this->enable = enable;
    this->slowedSpeed = slowedSpeed;

    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->enable, OUTPUT);
  }
};

char handStyle = -1;
Goo goo;
Tentacles tentacles;

void setup() {
  Serial.begin(9600);
  goo.begin(A2, 1500, 1200, 2000); //these values might be wrong
  tentacles.begin(A0, A1, 5, 50);
}

void gooEject(Goo &goo, int delayTime) {
  goo.servo.writeMicroseconds(goo.screwInMicroSeconds);
  delay(delayTime);
  goo.servo.writeMicroseconds(goo.stopMicroSeconds);
}

void gooReset(Goo &goo, int delayTime) {
  goo.servo.writeMicroseconds(goo.screwOutMicroSeconds);
  delay(delayTime);
  goo.servo.writeMicroseconds(goo.stopMicroSeconds);
}

void tentaclesWrithing(Tentacles &tentacles, int delayTime) {
  digitalWrite(tentacles.in1, HIGH);
  digitalWrite(tentacles.in2, LOW);

  analogWrite(tentacles.enable, 255);    // brief full-power kick so it starts
  delay(100);
  analogWrite(tentacles.enable, tentacles.slowedSpeed);

  delay(delayTime);
  digitalWrite(tentacles.in1, LOW);
  digitalWrite(tentacles.in2, LOW);
}

void loop() {
  if (Serial.available()) {
    handStyle = Serial.read();
  }

  switch (handStyle) {
    case '1': //flick, twich leg
      //activate & deactivate dc motor
      tentaclesWrithing(tentacles, 300); //pass like this or as pointer? i think it's fine but idkkk
    case '2': //boo, short goo, big twitch
      //activate & deactivate servo
      gooEject(goo, 300);
      //activate & deactivate dcmotor
      tentaclesWrithing(tentacles, 500);
    case '3': //squeeze, big goo
    //activate & deactivate servo
    gooEject(goo, 500);
  };
}
