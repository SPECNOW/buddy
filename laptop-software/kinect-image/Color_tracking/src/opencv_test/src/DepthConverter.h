#ifndef DEPTH_CONVERTER_H
#define DEPTH_CONVERTER_H

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <sstream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//--------------------------------------------------------------------------------------------------------


class DepthConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::NodeHandle position_NH;
  ros::Publisher position_Pub;
  int *X, *Y;
public:
  DepthConverter(int *X_value, int *Y_value);
  ~DepthConverter();
  void imageCb(const sensor_msgs::ImageConstPtr& msg);
};


#endif
