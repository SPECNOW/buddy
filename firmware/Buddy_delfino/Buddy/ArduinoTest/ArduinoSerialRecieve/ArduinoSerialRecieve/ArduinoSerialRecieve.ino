char incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  while(!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available()>0){
    incomingByte = Serial1.read();
    Serial.println(incomingByte);
  }
  
}
