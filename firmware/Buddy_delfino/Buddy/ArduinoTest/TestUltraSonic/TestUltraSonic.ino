int UTLRA_TRIGA_PIN = 2;
int UTLRA_TRIGB_PIN = 3;

int ULTRA_PULSEA_PIN = 0;
int ULTRA_PULSEB_PIN = 0;

volatile int pulseResponse = 128;

volatile int pwmA = 0;
volatile int pwmB = 0;
volatile int startTimeA = 0;
volatile int startTimeB = 0;
volatile int isA = 0;

void rising() {
  // Assume we're doing A or B (not both at the same time, one after the other)
  if(digitalRead(UTLRA_TRIGA_PIN)) {
    isA = 1;
    startTimeA = micros();
  } else {
    isA = 2;
    startTimeB = micros();
  }
}
 
void falling() {
  if(isA == 1) {
    pwmA = startTimeA-micros();
    sendPule(ULTRA_PULSEA_PIN);
    Serial.println("Received Trig A, Sent Pulse A");
  } else if (isA == 2) {
    pwmB = startTimeB-micros(); 
    sendPule(ULTRA_PULSEB_PIN);
    Serial.println("Received Trig B, Sent Pulse B");
  }
  isA = 0;
}

void sendPule(int pin) {
  digitalWrite(pin, HIGH);
  delay(1+pulseResponse/2);
  digitalWrite(pin, LOW);
}
 
void loop() { 
  while(!Serial.available());
  pulseResponse = (int)Serial.read();
  Serial.write("Pulse Width set to ");
  Serial.println(1+pulseResponse/2);
}

void setup() {
  Serial.begin(115200);

  pinMode(ULTRA_PULSEA_PIN, OUTPUT);
  digitalWrite(ULTRA_PULSEA_PIN, LOW);
  
  pinMode(ULTRA_PULSEB_PIN, OUTPUT);
  digitalWrite(ULTRA_PULSEB_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), rising, RISING);
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), rising, RISING);
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), falling, FALLING);
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), falling, FALLING);
}
