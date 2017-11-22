int x = 0;
int y = 0;
int PWMpin = 9;
int PWMvari = 0;
int microseconds = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial){
  pinMode(PWMpin,OUTPUT);  
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enter Character");        //ask for character
  while(Serial.available()==0);             //waiting for character in loop
  x = Serial.read();
  Serial.write(x);
  Serial.write("\n");
  while(Serial1.available()==0){
    /*delay(1000);
    microseconds = microseconds + 1;
    if(microseconds == 2){
      microseconds = 0;
      break;
    }*/
  }
  y = Serial1.read();                       // ready from serial1 buffer
  Serial.print("Received from Delfino: ");
  Serial.write(y);
  Serial.write("\n");
  //PWM
  /*PWMvari = Serial.read();
  if (x >=0 && x <=255){
    Serial.println("PWM Speed is");
    Serial.write(x);
    Serial.write("\n");
    analogWrite(PWMpin, x);
  }*/
  
  
}
