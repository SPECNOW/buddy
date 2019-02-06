int UTLRA_TRIGA_PIN = 2;
int UTLRA_TRIGB_PIN = 3;

int ULTRA_PULSEA_PIN = 4;
int ULTRA_PULSEB_PIN = 5;

volatile uint8_t pulseResponse = 128;

volatile int pwmA = 0;
volatile int pwmB = 0;
volatile int startTimeA = 0;
volatile int startTimeB = 0;
volatile bool trigA = false, trigB = false;


void rising_a() {
  // Assume we're doing A or B (not both at the same time, one after the other)
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), falling_a, FALLING);
  //Serial.println("RISING");
  startTimeA = micros();
}

void rising_b() {
  // Assume we're doing A or B (not both at the same time, one after the other)
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), falling_b, FALLING);
  //Serial.println("RISING");
  startTimeB = micros();
}
 
void falling_a() {
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), rising_a, RISING);
  //Serial.println("FALLING");
  pwmA = startTimeA-micros();
  trigA=true;
}

void falling_b() {
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), rising_b, RISING);
  //Serial.println("FALLING");
  pwmB = startTimeB-micros();
  trigB=true;
}

void sendPulse(int pin) {
  delayMicroseconds(10);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseResponse*100);
  digitalWrite(pin, LOW);
}
 
void loop() {
  if(trigA){
    Serial.println("Received Trig A, Sent Pulse A");
    sendPulse(ULTRA_PULSEA_PIN);
    trigA=false;
  }
  if(trigB){
    Serial.println("Received Trig B, Sent Pulse B");
    sendPulse(ULTRA_PULSEB_PIN);
    trigB=false;
  }  
}

void setup() {
  Serial.begin(115200);
  pinMode(UTLRA_TRIGA_PIN, INPUT);
  pinMode(UTLRA_TRIGB_PIN, INPUT);

  pinMode(ULTRA_PULSEA_PIN, OUTPUT);
  digitalWrite(ULTRA_PULSEA_PIN, LOW);
  
  pinMode(ULTRA_PULSEB_PIN, OUTPUT);
  digitalWrite(ULTRA_PULSEB_PIN, LOW);

  Serial.write("Enter Pulse Width value:");
  while(!Serial.available());
  pulseResponse = (uint8_t)Serial.read();
  Serial.write("Pulse Width set to ");
  Serial.println(pulseResponse);

  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), rising_a, RISING);
  attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), rising_b, RISING);
  //attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGA_PIN), falling_b, FALLING);
  //attachInterrupt(digitalPinToInterrupt(UTLRA_TRIGB_PIN), falling_b, FALLING);
}
