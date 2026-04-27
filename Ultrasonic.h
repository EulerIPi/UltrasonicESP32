#ifndef ULTRASONIC_H
#define ULTRASONIC_H
class Ultrasonic {
  volatile uint64_t Ti = 0, Tf = 0;
  uint64_t timeOut;
  uint16_t triggerPin, echoPin;
  uint64_t mask;
  static Ultrasonic* instance;
  static void IRAM_ATTR isr();
  volatile bool flag = false;

public:
  Ultrasonic(uint16_t trig, uint16_t echo);
  float get();
};
#endif