//Draft code for buddy bot. 

#include <Comm.H>
#include <Velocity.H>
#include <Sensor.H>
#include <Encoder.H>

//unsigned int comm();
//unsigned int velocity();
//unsigned int sensor();
//unsigned int encoder_setup();

// structure for storing the variables used for encoder directionv
typedef struct dir_struct{
	volatile int prev;
	volatile int curr;
	volatile int out;
} dir_st;

//GLOBAL VARIABLES
int current_spd = 64;
int prev_spd = 64;
int test1, test2;
int store1[50], store2[50];
int countstore;
int PRINT_OUTPUT = 0;

// instantiating the direction structs
dir_st dir_left = {0, 0, 0}; 
dir_st dir_right = {0, 0, 0};

// handshake??
#define ping_duration 50
#define time_out 500
#define halt 64
#define typ_spd 70

// interrupt pin number
//interupt 0 is digital 2, interupt 1 is digital 3
#define encoder_interupt_right_num 0
#define encoder_interupt_left_num 1

// 1 forward, 0 no move, 2 is nt care, error
int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};               // Quadrature Encoder Matrix

// Define input pins
#define encoder_inputA1 5
#define encoder_inputB1 6

#define encoder_inputA2 7
#define encoder_inputB2 8
boolean doStuff = true;
//int main()
//{
//  //setup variable
//  //encoder_setup(1);
//  //encoder_interupt_attach(1);
//}
void setup () {
  //serial shit
  Serial.begin(9600);
  Serial1.begin(38400);
  encoder_setup(true);
  if (!encoder_interupt_attach (true) ){
    Serial.println("Error With attachting interrupt");
  }
  
  //temporiry pin fo rtesting
  pinMode(9, OUTPUT);
//  digitalWrite(10,HIGH);
  Serial.println("Done Setup: Starting Code");
}

void loop () {
  
//  if (PRINT_OUTPUT) {
//    doStuff = false;
//      int _test1, _test2, _curr, _out;
//      _test1 = test1;
//      _test2 = test2;
//      _curr = dir_right.curr;
//      _out = dir_right.out;
//    Serial.print("TEST1: ");
//    Serial.print (_test1);
//    Serial.print("    ");
//    Serial.print("TEST2: ");
//    Serial.print(_test2);
//    Serial.print("    Values: ");
//    Serial.print(_curr); 
//    Serial.print("    DIR: ");
//    Serial.println(_out);
//    PRINT_OUTPUT=0;
//    doStuff = true;
//  }

 if( PRINT_OUTPUT ) {
   encoder_interupt_attach(false);
   for( int i= 0; i <49 ; i++) {
     Serial.print(store1[i]); 
   }
   Serial.println("");
   for( int i= 0; i <49 ; i++) {
     Serial.print(store2[i]);
   }
   Serial.println("");
   Serial.println("");
   PRINT_OUTPUT = 0;
   encoder_interupt_attach(true);
 }
  
  
  
//  Serial.print("LEFT DIR: ");
//  Serial.print (dir_left.out);
//  Serial.print("    ");
//  Serial.print("DRIGHT DIR: ");
//  Serial.println(dir_right.out);
}

int comm()
{
  return 0;
}

//*****************************************************
//Collect sensor values and determine if distances.
//*****************************************************
int sensor()
{
	float sonic_sensor[2]; //determine which sensor is which. For now arbitrary name. to be moved to header file in future.
	int sensor[4];
	int min_dist = 3; //smallest distance allowable before emergency stop
	int dist_thres = 5; //threshold distance use to change the Kd value such that damping take effect. Thus, slowing down the vehicle.
	int SLOW = 0;
	int ECOMM = 0; //emergency stop variable.
	int i; //loop counter

	sonic_sensor[0] = ping(0,1,6035.003); //select correct pins for sonic sensor. U100 requires conversion constant of 6035.003
	sonic_sensor[1] = ping(2,3,58); //select correct pins for sonic sensor
	sensor[0] = analogRead(2);//insert correct pin number for IR Sensors.
	sensor[1] = analogRead(3);//insert correct pin number for IR Sensors.
	sensor[2] = analogRead(4);//insert correct pin number for IR Sensors.
	sensor[3] = analogRead(5);//insert correct pin number for IR Sensors.

	//Emergency bit variable to be sent to laptop. Future, break each sensor out and for vehicle to travel opposite direction.
	for (i = 0; i < 2; i++) {
		if (sonic_sensor[i] < min_dist) {
			ECOMM = 1;
			return ECOMM;
		}
	}
	for (i = 0; i < 4; i++) {
		if (sensor[i] < min_dist) {
			ECOMM = 1;
			return ECOMM;
		}
	}
	
	//Variable to slow down vehicle if getting too close. Future, break each sensor out and for vehicle to travel opposite direction.
	for (i = 0; i < 2; i++) {
		if (sonic_sensor[i] < dist_thres) {
			SLOW = 1;
			return SLOW;
		}
	}
	for (i = 0; i < 4; i++) {
		if (sensor[i] < dist_thres) {
			SLOW = 1;
			return SLOW;
		}
	}
	
	// Default
	SLOW = 0;
	return SLOW;
}


//********************************************************
//pin function for the HC-SR04 and US100 Ultrasonic sensor. Pin_Trig and Pin Echo to be determined on Arduino board.
//Pin_Trig Arduino output to trigger sensor.
//Pin_Echo Arudino input to read high duration.
//return 0 if failed, return in CM
//********************************************************
float ping(int pin_Trig, int pin_Echo, float conversion_const)
{
	unsigned long time1, time2, high_duration;
	digitalWrite(pin_Trig, LOW);
	delayMicroseconds(ping_duration);
	digitalWrite(pin_Trig, HIGH);
	delayMicroseconds(ping_duration);
	digitalWrite(pin_Trig, LOW);

	high_duration = pulseIn(pin_Echo, HIGH, time_out);

	if (high_duration == 0) {
		//Serial.print("Error, PulseIn returned 0");
		return 0;
	}

	if (high_duration > 37000) {
		//Serial.print ("Distance = INF ");
		return 0;
	}

	return (float)high_duration / conversion_const;
}


//********************************************************
//Set up the pin inout of encoder
//********************************************************
void encoder_setup(bool encoder_dir_on_off)
{
	//Put this setup in main
	//put the input pins as input 
	if(encoder_dir_on_off)
	{
		pinMode(encoder_inputA1, INPUT);
		pinMode(encoder_inputB1, INPUT);
		pinMode(encoder_inputA2, INPUT);
		pinMode(encoder_inputB2, INPUT);
	}
	else
	{

	}
}


//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_right (void) {
  PORTH |= 1<<6;
  //digitalWrite(10,HIGH);
  //digitalWrite(10,LOW);
  //if(doStuff)
  //{
        //int timeout = 5;
        //do
        //{
          dir_right.prev = dir_right.curr;
          test1 = (PINH >> 3) & 1;//(boolean)digitalRead (encoder_inputB1);
          test2 = (PINE >> 3) & 1;//(boolean)digitalRead (encoder_inputA1);
          //Serial.println("RIGHT Reads:encoder_inputB1: encoder_inputA1");
    	  dir_right.curr = ((int)test1 << 1)+ (int)test2;           // Convert binary input to decimal value
        //  timeout--;
        //}while(dir_right.curr == dir_right.prev && timeout >0);
        dir_right.out = QEM [dir_right.prev * 4 + dir_right.curr]; // out variable returns -1
        store1[countstore] =  (dir_right.curr);
        store2[countstore] = (dir_right.prev);
        countstore++;
        if (countstore == 49) {
          detachInterrupt(0);
          PRINT_OUTPUT = 1;
          countstore = 0;
        }
        PORTH &= !(1<<6);
  //}
  //digitalWrite(10,LOW);
}

//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_left (void) {
	dir_left.prev = dir_left.curr;
        //Serial.println("LEFT Reads:encoder_inputB2: encoder_inputA2");
	dir_left.curr = digitalRead (encoder_inputB2) * 2 + digitalRead (encoder_inputA2);           // Convert binary input to decimal value
	dir_left.out = QEM [dir_left.prev * 4 + dir_left.curr]; // out variable returns -1
        //Serial.println("Left");
        //Serial.println(dir_left.out);
}

//Function for attaching/ detaching  interupts
//input 1 or 0 for attaching or detaching 
//return 0 if action not done
int encoder_interupt_attach (bool input) {
	if (input == 1 ) { // attach interupt
		attachInterrupt(encoder_interupt_right_num, ISR_acquire_dir_right, CHANGE);
		attachInterrupt(encoder_interupt_left_num, ISR_acquire_dir_left, CHANGE);
		return 1;
	}
	if ( input == 0 ) { // detach interput
		detachInterrupt(encoder_interupt_right_num);
		detachInterrupt(encoder_interupt_left_num);
		return 1;
	}
	
	return 0;
}


//long find_encoder_freq
//{
//	attachInterrupt(encoder_interupt_num, ISR_acquire_dir, RISING); //provide pin number
//	
//	
//	detachInterrupt(encoder_interupt_num);
//}
/*Create motor commmunication function based on PC commands.
Motor communication recied from PC will be used to determine whel speeds. */


/*

*/
int motor_control_output(int motor_spd_command) //motor_spd_command is used for 
{
  //int faster = 2;
  //int slower = 1;
  //int halt = 0; 
  int pre_spd = current_spd;
  int current_spd = motor_spd_command;
        if (current_spd = prev_spd)
        {
            return 0;
        }
        else if (motor_spd_command != pre_spd)
        {
          Serial1.write(motor_spd_command);
        }
        else
        {
          Serial1.write(halt);
        }
        
}
