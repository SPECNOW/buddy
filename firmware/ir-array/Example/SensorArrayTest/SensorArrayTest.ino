#include <Sensor.h>
#include <DistanceGP2Y0A21YK.h>

Sonar sensor1 = Sonar(8, 9, 50);
IR sensor2 = IR(A1);

void setup()
{
  Serial.begin(9600);
}

void loop()
{  
  sensor1.fetchData();
  sensor2.fetchData();
}
