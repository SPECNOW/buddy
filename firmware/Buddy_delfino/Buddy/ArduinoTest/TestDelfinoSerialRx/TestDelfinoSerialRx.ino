int x = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial);
  while (!Serial1);
}

void loop() {
  //waiting for character in from PC
  while(Serial.available()==0);
  x = Serial.read();

  // Serial 1 - Send to Delfino
  Serial1.write(x);

  // Wait for response from Delfino
  while(Serial1.available()==0);

  // Send response back to PC
  if(Serial1.available() >0){
    y = Serial1.read();
    Serial.write(y);
  }  
}
