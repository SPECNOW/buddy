int x = 0;
int y = 0;
int PWMpin = 9;
int PWMvari_received = 0;
const byte numChar = 32;
char receivedPWMChars[numChar];
int microseconds = 0;
boolean newData = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(PWMpin,OUTPUT);  
  while (!Serial);  
}

void loop() {
  // put your main code here, to run repeatedly:
  // PWM test code.
  Serial.println("Enter desired PWM speed. Please use numbers between 0 and 255 inclusive.");
  while(Serial.available()==0);
  
  recvChar(); //when serial is > 0 go to recieved single charater.
  showNewData(); //print new data in command line.
 
}

void recvChar(){
  static byte ndx = 0;
  char endMarker = '\n';
  char received_single_byte;
  while(Serial.available() > 0 && newData == false){
    received_single_byte = Serial.read();
    if(received_single_byte != endMarker){
      receivedPWMChars[numChar] = received_single;
      numChar++;
      if (ndx >= numChar){
        ndx = numChars - 1;
      }
      else{
        receivedPWMChars[ndx] = '\0'; // terminate the string
        ndx = 0;
        newData = true;
      }
    }
    
  }
  if(Serial.available() > 0){
    PWMvari_received = Serial.read();//this will only read a single digit
    if (PWMvari_received >=0 && PWMvari_received <=255){
    newData = true;
    }
  }
}

void showNewData(){
    if (newData = true){
    Serial.println("PWM Speed is");
    Serial.write(PWMvari_received);
    Serial.write("\n");
    analogWrite(PWMpin, x); //set PWM pin
    newData = false;
  }
}

