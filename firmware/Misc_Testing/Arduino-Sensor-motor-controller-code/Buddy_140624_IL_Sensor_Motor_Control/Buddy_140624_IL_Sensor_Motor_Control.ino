//Draft code for buddy bot. 

#include <Comm.H>
#include <Velocity.H>
#include <Sensor.H>
#include <Encoder.H>

unsigned comm();
unsigned velocity();
unsigned sensor();
unsigned encoder_setup();
int ping(int pin_Trig, int pin_Echo, double conversion_const);
long find_encoder_freq();

#define encoder_pin_A 13
#define encoder_pin_B 14
int QEM [16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0}; // Quadrature Encoder Matrix
int dir_val_old, dir_val_new; //the value of the direction, from the table above
int dir_old, dir_new; //the direction of motor
int encoder_ms_old, encoder_ms_new;

main()
{
  unsigned c = 0;
  
  c = sensor();
//setup variables

}

unsigned comm()
{

}
//*****************************************************
//Collect sensor values and determine if distances.
//*****************************************************
unsigned sensor()
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
	}
	else
	{
		ECOMM = 0;
	}
	//Variable to slow down vehicle if getting too close. Future, break each sensor out and for vehicle to travel opposite direction.
	if(sensor1 | sensor2 | sensor3 | sensor4 | sensor5 | sensor6 < dist_thres)
	{
		SLOW = 1;
	}
	else
	{
		SLOW = 0;
	}
  		return ECOMM;
  		return SLOW;
}

//********************************************************
//pin function for the HC-SR04 and US100 Ultrasonic sensor. Pin_Trig and Pin Echo to be determined on Arduino board.
//Pin_Trig Arduino output to trigger sensor.
//Pin_Echo Arudino input to read high duration.
//return 0 if failed, return in CM
//********************************************************
int ping(int pin_Trig, int pin_Echo, double conversion_const)
{
	unsigned int ping_duration = 15;
        unsigned int time_out = 50000;
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
//pass encoder interupts to determine high and low values to laptop.
//********************************************************
void encoder_setup(bool encoder_dir_on_off)
{
	//Put this setup in main
	//put the input pins as input 
       
	if(encoder_dir_on_off = true)
	{
		pinMode(encoder_pin_A, INPUT);
		pinMode(encoder_pin_B, INPUT);
		//attach the interrupt
		attachInterrupt(0, ISR_acquire_dir, RISING);
		Serial.begin(9600);
	}
	else
	{
		detachInterrupt(0);
	}
}
void ISR_acquire_dir()
{
  //set old dir as new dir
  //sets new value as old. and fetch new value ( single step convert input to decimal)
  dir_old = dir_new;
  dir_val_old = dir_val_new;
  dir_val_new = digitalRead(encoder_pin_A)*2 + digitalRead(encoder_pin_B);
  dir_new = QEM[dir_val_old*4+dir_val_new];
}
int find_rotational_dir() {
  //attachInterrupt(encoder_interupt_num, ISR_acquire_dir, RISING);
  while ( (dir_new != dir_old) && (dir_new && dir_old !=0) ) {
      //holds here till we find dir
      
          //********debug statements
     // println("Waiting for direction to be found");
          //********debug statements
  }
    //********debug statements
    //  print("Found! it is going in: ");
    //  println(dir_new);
    //********debug statements
    
    //detachInterrupt(encoder_interupt_num);
  return dir_new;
}

/*long find_encoder_freq()
{
	attachInterrupt(encoder_interupt_num, ISR_acquire_dir, RISING); //provide pin number
	
	
	detachInterrupt(encoder_interupt_num);
}
*/
