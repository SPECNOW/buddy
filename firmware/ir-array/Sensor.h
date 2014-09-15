#ifndef SENSOR_CLASS
#define SENSOR_CLASS

#include <DistanceGP2Y0A21YK.h>

#define NUM_SAMPLES 10
#define ping_duration 50
#define time_out 500


class Sensor
{
protected:
	virtual int read_data();
public:
	Sensor();
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
	int read_data();
public:
	Sonar(unsigned int echo_Pin, unsigned int trig_Pin, double conversion_const);
	~Sonar();
};

class IR	:	public Sensor
{
private:
	unsigned int analogPin;
	DistanceGP2Y0A21YK IRSensor;
	int read_data();
public:
	IR(unsigned int analog_Pin);
	~IR();
};

#endif