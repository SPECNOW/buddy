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

// instantiating the direction structs
dir_st dir_left = {0, 0, 0}; 
dir_st dir_right = {0, 0, 0};

// handshake??
#define ping_duration 50
#define time_out 500

// interrupt pin number
#define encoder_interupt_right_num 0
#define encoder_interupt_left_num 5


int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};               // Quadrature Encoder Matrix

// Define input pins
#define encoder_inputB1 5
#define encoder_inputA1 6
#define encoder_inputA2 7
#define encoder_inputB2 8

int main()
{
//setup variables
  return 0;
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
	int sensor1 = 0; //determine which sensor is which. For now arbitrary name. to be moved to header file in future.
	int sensor2 = 0;
	int sensor3 = 0;
	int sensor4 = 0;
	int sensor5 = 0;
	int sensor6 = 0;
	int min_dist = 3; // smallest distance allowable before emergency stop
	int dist_thres = 5; //threshold distance use to change the Kd value such that damping take effect. Thus, slowing down the vehicle.
	int SLOW = 0;
	int ECOMM = 0; // emergency stop variable.

	sensor1 = ping(0,1,6035.003); //select correct pins for sonic sensor. U100 requires conversion constant of 6035.003
	sensor2 = ping(2,3,58); //select correct pins for sonic sensor
	sensor3 = analogRead(2);//insert correct pin number for IR Sensors.
	sensor4 = analogRead(3);//insert correct pin number for IR Sensors.
	sensor5 = analogRead(4);//insert correct pin number for IR Sensors.
	sensor6 = analogRead(5);//insert correct pin number for IR Sensors.

	//Emergency bit variable to be sent to laptop. Future, break each sensor out and for vehicle to travel opposite direction.
	if(sensor1 | sensor2 | sensor3 | sensor4 | sensor5 | sensor6 < min_dist)
	{
		ECOMM = 1;
		return ECOMM;
	}
	else
	{
		ECOMM = 0;
		return ECOMM;
	}
	//Variable to slow down vehicle if getting too close. Future, break each sensor out and for vehicle to travel opposite direction.
		if(sensor1 | sensor2 | sensor3 | sensor4 | sensor5 | sensor6 < dist_thres)
	{
		SLOW = 1;
		return SLOW;
	}
	else
	{
		SLOW = 0;
		return SLOW;
	}
}

//********************************************************
//pin function for the HC-SR04 and US100 Ultrasonic sensor. Pin_Trig and Pin Echo to be determined on Arduino board.
//Pin_Trig Arduino output to trigger sensor.
//Pin_Echo Arudino input to read high duration.
//return 0 if failed, return in CM
//********************************************************
int ping(int pin_Trig, int pin_Echo, double conversion_const)
{
	unsigned long time1, time2, high_duration;
	digitalWrite(pin_Trig, LOW);
	delayMicroseconds(ping_duration);
	digitalWrite(pin_Trig, HIGH);
	delayMicroseconds(ping_duration);
	digitalWrite(pin_Trig, LOW);

	high_duration = pulseIn(pin_Echo, HIGH, time_out);

	if (!high_duration) {
	//Serial.print("Error, PulseIn returned 0");
	return 0;
	} 

	if ( high_duration >37000){
	//Serial.print ("Distance = INF ");
	return 0;
	}

	return high_duration/conversion_const;
}
//********************************************************
//Set up the pin inout of encoder
//********************************************************
void encoder_setup(bool encoder_dir_on_off)
{
	//Put this setup in main
	//put the input pins as input 
	if(encoder_dir_on_off = true)
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

// Function in progress
// maybe a struct typed function, returns LEFT AND RIGHT rotational direction
//int find_rotational_dir() {
//
//  return dir_new;
//}

//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_right (void) {
	dir_right.prev = dir_right.curr;
	dir_right.curr = digitalRead (encoder_inputA1) * 2 + digitalRead (encoder_inputA1);           // Convert binary input to decimal value
	dir_right.out = QEM [dir_right.prev * 4 + dir_right.curr]; // out variable returns -1
}

//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_left (void) {
	dir_left.prev = dir_left.curr;
	dir_left.curr = digitalRead (encoder_inputA2) * 2 + digitalRead (encoder_inputA2);           // Convert binary input to decimal value
	dir_left.out = QEM [dir_left.prev * 4 + dir_left.curr]; // out variable returns -1
}

//Function for attaching/ detaching  interupts
//input 1 or 0 for attaching or detaching 
//return 0 if action not done
int encoder_interupt_attach (bool input) {
	if (input == 1 ) { // attach interupt
		attachInterrupt(encoder_interupt_right_num, ISR_acquire_dir_right, RISING);
		attachInterrupt(encoder_interupt_left_num, ISR_acquire_dir_left, RISING);
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

