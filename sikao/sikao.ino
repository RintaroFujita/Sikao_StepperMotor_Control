#include <SPI.h>

#define L6470_SS_PIN 10

bool forward = true;

void setup() {
  SPI.begin();
  pinMode(L6470_SS_PIN, OUTPUT);
  digitalWrite(L6470_SS_PIN, HIGH);

  L6470_setup();

  Serial.begin(9600);
}

void loop() {
  if (forward) {
    // モーター正転（0.5秒）
    rotateMotorMicrostep(1.0, 100);
    delay(500);

    // モーター停止（2秒）
    stopMotor();
    delay(2000);

    forward = false;
  } else {
    // モーター逆転（0.5秒）
    rotateMotorMicrostep(-1.0, 100);
    delay(500);

    // モーター停止（2秒）
    stopMotor();
    delay(2000);

    forward = true;
  }
}

void L6470_send(unsigned char value) {
  digitalWrite(L6470_SS_PIN, LOW);
  SPI.transfer(value);
  digitalWrite(L6470_SS_PIN, HIGH);
}

void L6470_setup() {
  L6470_send(0x0B); 
  L6470_send(0x0D); 
}

void rotateMotorMicrostep(float kaiten, int speed) {
  if (kaiten > 0) {
    L6470_send(0x50); // 正転
  } else {
    L6470_send(0x51); // 逆転
  }

  speed = constrain(speed, 0, 1023);
  byte speedHigh = speed / 256;
  byte speedLow = speed % 256;
  L6470_send(speedHigh);
  L6470_send(speedLow);

  int steps = abs(kaiten) * 128 * 4 * 200;
  for (int i = 0; i < steps; i++) {
    L6470_send(0x55); 
  }
}

void stopMotor() {
  L6470_send(0xB8); // モーターを停止
}
