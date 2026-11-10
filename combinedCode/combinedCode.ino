#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

struct Goo {
  Servo servo;
  int pin;
  int stopMicroSeconds;
    //Try to understand this more                  
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

  void begin(int in1, int in2, int enable, int slowedSpeed) {
    this->in1 = in1;
    this->in2 = in2;
    this->enable = enable;
    this->slowedSpeed = slowedSpeed;

    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->enable, OUTPUT);
  }
};

Goo goo;
Tentacles tentacles;

const char* WIFI_NAME = "SimrenSingh";
const char* WIFI_PASSWORD = "brutherplease";
const int PORT = 12000;              // Wekinator's output port
const unsigned long STABLE_MS = 300;  // class must hold this long

WiFiUDP udp;
bool usingWekinator = false;
unsigned long lastMessage = 0;

int incomingHandMovement = 0;
unsigned long incomingSince = 0;
int activeHandMovement = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("beginning of setup");
  WiFi.config(IPAddress(172, 20, 10, 10),     // the board's fixed IP
          IPAddress(172, 20, 10, 1),      // the hotspot itself
          IPAddress(255, 255, 255, 240)); // iPhone hotspot subnet
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(300);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nWi-Fi connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo Wi-Fi yet, typed testing still works");
  }
  if (!udp.begin(PORT)) { Serial.print("UDP failed to begin"); }
  goo.begin(13, 1500, 1200, 2000); //these values might be wrong
  tentacles.begin(25, 26, 27, 50); //these too
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

void setIncomingHandMovement(int handMovement) {
  if (handMovement != incomingHandMovement) {
    incomingHandMovement = handMovement;
    incomingSince = millis();
  }
}

void onWekinator(OSCMessage &msg) {
  setIncomingHandMovement((int)round(msg.getFloat(0)));
}

void respondToHandMovement(int handMovement) {
    switch (handMovement) {
      case 1: //flick, twitch leg
        //activate & deactivate dc motor
        Serial.println("Case 1");
        tentaclesWrithing(tentacles, 3000); //pass like this or as pointer? i think it's fine but idkkk
        break;
      case 2: //boo, short goo, big twitch
        //activate & deactivate servo
        Serial.println("Case 2");
        gooEject(goo, 5000);
        //activate & deactivate dcmotor
        delay(300);
        tentaclesWrithing(tentacles, 5000);
        break;
      case 3: //squeeze, big goo
      //activate & deactivate servo
        Serial.println("Case 3");
        gooEject(goo, 7000);
        break;
      default:
        break;
  };
}

void loop() {
  static unsigned long lastBeat = 0;
  if (millis() - lastBeat > 2000) {
    lastBeat = millis();
    Serial.println("alive");
  }
  //Manual testing
  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '1' && c <= '4') setIncomingHandMovement(c - '0');
  }

  // Gestures from Wekinator over Wi-Fi
  int size = udp.parsePacket();
  if (size) {
    OSCMessage msg;
    while (size--) { msg.fill(udp.read()); }
    if (!msg.hasError()) { msg.dispatch("/wek/outputs", onWekinator); }
  }

  if ((incomingHandMovement != activeHandMovement) && (millis() - incomingSince > STABLE_MS)) { //might not need this stable_ms given that it's a whole range of motion
    activeHandMovement = incomingHandMovement;
    Serial.printf("Gesture %d\n", activeHandMovement);
    respondToHandMovement(activeHandMovement);
  }
}