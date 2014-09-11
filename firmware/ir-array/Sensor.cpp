#include "Sensor.h"

/***
****	SENSOR
***/
Sensor::Sensor(unsigned int sensor_pin)
{
	this->sensor_id = this->num_sensors;
	this->num_sensors++;
}

Sensor::~Sensor()
{
}

void Sensor::fetchData()
{	
	Serial.write(this->read_data());
}

int Sensor::read_data()
{
	return -1;
}

/***
****	SONARSENSOR
***/
Sonar::Sonar(unsigned int echo_Pin, unsigned int trig_Pin, double conversion_const)
{
	this->echoPin = echo_Pin;
	this->trigPin = trig_Pin;
	this->conversionConstant = conversion_const;
	
	pinMode(this->trigPin, OUTPUT);
	pinMode(this->echoPin, INPUT);
	
	this->sensor_id = this->num_sensors;
	this->num_sensors++;
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
			valid_samples = 0;
		}
	}
	
	if(valid_samples == 0)
		return 0;
		
	return (int)(cum_sum/(long)valid_samples);
}

int Sonar::get_sample()
{
	unsigned long high_duration;
	
	digitalWrite(this->trigPin, LOW);
	delayMicroseconds(ping_duration);
	digitalWrite(this->trigPin, HIGH);
	delayMicroseconds(ping_duration);
	digitalWrite(this->trigPin, LOW);

	high_duration = pulseIn(this->echoPin, HIGH, time_out);

	if (!high_duration) {
	//Serial.print("Error, PulseIn returned 0");
	return 0;
	} 

	if ( high_duration >37000){
	//Serial.print ("Distance = INF ");
	return -1;
	}

	return (unsigned long)((double)high_duration/this->conversionConstant);
}

/***
****	IRSENSOR
***/
IR::IR(unsigned int analog_Pin)
{
	this->analogPin = analog_Pin;
	this->IRSensor.setARefVoltage(5);
	this->IRSensor.setAveraging(NUMBER_SAMPLES);
	this->IRSensor.begin(this->analogPin);
	
	pinMode(this->analogPin, INPUT);
	this->sensor_id = this->num_sesnsors;
	this->num_sensors++;
}

IR::~IR()
{
}

int IR::read_data()
{
	return this->IRSensor.getDistanceCentimeter();
}
