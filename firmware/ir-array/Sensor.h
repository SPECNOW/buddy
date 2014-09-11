#ifndef SENSOR_CLASS
#define SENSOR_CLASS

#include <DistanceGP2Y0A21YK.h>

#define NUMBER_SAMPLES 10

class Sensor
{
private:
	static int num_sensors;
	int sensor_id;
	virtual int read_data();
public:
	Sensor(unsigned int sensor_pin);
	~Sensor();
	void fetchData();
};

class Sonar	:	public Sensor
{
private:
	unsigned int echoPin;
	unsigned int trigPin;
	double conversionConstant;
	int get_sample();
public:
	Sonar(unsigned int echo_Pin, unsigned int trig_Pin, double conversion_const);
	~Sonar();
};

class IR	:	public Sensor
{
private:
	unsigned int analogPin;
	DistanceGP2Y0A21YK IRSensor;
public:
	IR(unsigned int analog_Pin);
	~IR();
};

#endif