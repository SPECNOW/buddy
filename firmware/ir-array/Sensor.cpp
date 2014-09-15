#include "Sensor.h"

//#define DEBUG

/***
****	SENSOR
***/
Sensor::Sensor()
{
}

Sensor::~Sensor()
{
}

void Sensor::fetchData()
{	
#ifdef DEBUG
	Serial.print("DISTANCE: ");
	Serial.println(this->read_data());
#else
	Serial.write(this->read_data());
#endif
}

int Sensor::read_data()
{
	return -1;
}

/***
****	SONARSENSOR
***/
Sonar::Sonar(unsigned int echo_Pin, unsigned int trig_Pin, double conversion_const)	:	Sensor()
{
	this->echoPin = echo_Pin;
	this->trigPin = trig_Pin;
	this->conversionConstant = conversion_const;
	
	pinMode(this->trigPin, OUTPUT);
	pinMode(this->echoPin, INPUT);
}

Sonar::~Sonar()
{
}

int Sonar::read_data()
{
	unsigned long cum_sum = 0;
	int samp_val = 0, valid_samples = 0;
	
	for(unsigned int samp = 0; samp < NUM_SAMPLES; samp++)
	{
		samp_val = this->get_sample();
		if(samp_val != 0 && samp_val != -1)
		{
			cum_sum += samp_val;
			valid_samples++;
		}
	}

	if(valid_samples == 0)
		return 0;
		
	return (int)(cum_sum/(unsigned long)valid_samples);
}

int Sonar::get_sample()
{
	unsigned long high_duration;
	
	digitalWrite(this->trigPin, LOW);
	delayMicroseconds(ping_duration);
	digitalWrite(this->trigPin, HIGH);
	delayMicroseconds(ping_duration);
	digitalWrite(this->trigPin, LOW);

	high_duration = pulseIn(this->echoPin, HIGH);

	if (!high_duration) {
		return 0;
	} 

	if ( high_duration >37000){
		return -1;
	}
	
	return (int)((double)high_duration/this->conversionConstant);
}

/***
****	IRSENSOR
***/
IR::IR(unsigned int analog_Pin)	:	Sensor()
{
	this->analogPin = analog_Pin;
	pinMode(this->analogPin, INPUT);
	this->IRSensor.setARefVoltage(5);
	this->IRSensor.setAveraging(NUM_SAMPLES);
	this->IRSensor.begin(this->analogPin);
}

IR::~IR()
{
}

int IR::read_data()
{
	return (int)((double)this->IRSensor.getDistanceCentimeter()*2.54);	//Looks like it's in inches
}
