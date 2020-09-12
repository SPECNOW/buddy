int x = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(38400);
  while (!Serial);
  while (!Serial1);
}

void loop() {
  //waiting for character in from Sabertooth
  while(Serial1.available()==0);
  x = Serial1.read();

  // Serial - Send to PC
  Serial.write(x);
}
