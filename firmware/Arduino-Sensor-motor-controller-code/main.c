//Draft code for buddy bot. 

#include <Comm.H>
#include <Velocity.H>
#include <Sensor.H>
#include <Encoder.H>

unsigned comm();
unsigned velocity();
unsigned sensor();
unsigned encoder_setup();

typedef struct dir_struct( // structure for storing the variables used for encoder direction
	volatile int old;
	volatile int new; 
	volatile int out;
} dir_st;

// instantiating the direction structs
dir_st dir_left = {0, 0, 0}; 
dir_st dir_right = {0, 0, 0};

int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};               // Quadrature Encoder Matrix

// Define input pins
#define inputB1 5
#define inputA1 6
#define inputA2 7
#define inputB2 8

main()
{
//setup variables

}

comm()
{

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
		Return ECOMM;
	}
	else
	{
		ECOMM = 0
		Return ECOMM;
	}
	//Variable to slow down vehicle if getting too close. Future, break each sensor out and for vehicle to travel opposite direction.
		if(sensor1 | sensor2 | sensor3 | sensor4 | sensor5 | sensor6 < dist_thres)
	{
		SLOW = 1;
		Return SLOW;
	}
	else
	{
		SLOW = 0;
		Return SLOW;
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
	if(encoder_dir_on_off = TRUE)
	{
		pinMode(encoder_pin_A1, INPUT);
		pinMode(encoder_pin_B1, INPUT);
		pinMode(encoder_pin_A2, INPUT);
		pinMode(encoder_pin_B2, INPUT);
	}
	else
	{

	}
}

// Function in progress
// maybe a struct typed function, returns LEFT AND RIGHT rotational direction
int find_rotational_dir() {

  return dir_new;
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

//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_right {
	dir_right.old = dir_right.new;
	dir_right.new = digitalRead (encoder_pin_A1) * 2 + digitalRead (encoder_pin_A1);           // Convert binary input to decimal value
	dir_right.out = QEM [dir_right.old * 4 + dir_right.new]; // out variable returns -1
}

//ISR
// out variable returns -1 for backwards, 1 for forward
void ISR_acquire_dir_left {
	dir_left.old = dir_left.new;
	dir_left.new = digitalRead (encoder_pin_A2) * 2 + digitalRead (encoder_pin_A2);           // Convert binary input to decimal value
	dir_left.out = QEM [dir_left.old * 4 + dir_left.new]; // out variable returns -1
}

long find_encoder_freq
{
	attachInterrupt(encoder_interupt_num, ISR_acquire_dir, RISING); //provide pin number
	
	
	detachInterrupt(encoder_interupt_num);
}
