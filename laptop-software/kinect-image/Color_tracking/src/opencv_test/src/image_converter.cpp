#include "image_converter.h"
#include "DepthConverter.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  colorTracker ic;
  DepthConverter dc(&(ic.objectPosition.X_value),&(ic.objectPosition.Y_value));
  ros::spin();
  return 0;
}
