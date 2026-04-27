Ultrasonic::Ultrasonic(uint16_t trig, uint16_t echo) : triggerPin(trig), echoPin(echo), mask(1ULL << echo){
  instance = this;
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);
  attachInterrupt(digitalPinToInterrupt(echoPin), isr, CHANGE);
}

Ultrasonic* Ultrasonic::instance = nullptr;

void IRAM_ATTR Ultrasonic::isr() {
  // Sinaliza que ocorreu uma borda
  instance->flag = true;
  if (GPIO.in & instance->mask){
    instance->Ti = esp_timer_get_time();
  }else{
    instance->Tf = esp_timer_get_time();
  }
}

float Ultrasonic::get() {
  flag = false;   // limpa estado anterior
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(triggerPin, LOW);

  timeOut = esp_timer_get_time() + 30000;
  while (esp_timer_get_time() < timeOut) {
    if (flag) {
      flag = false;              // capturou primeira borda (subida)
      timeOut = esp_timer_get_time() + 32000;
      while (esp_timer_get_time() < timeOut) {
        if (flag) {
          flag = false;          // segunda borda (descida)
          return (Tf - Ti) * 1e-6f * 171.5f;
        }
      }
    }
  }
  return -1.0f;   // timeout
}