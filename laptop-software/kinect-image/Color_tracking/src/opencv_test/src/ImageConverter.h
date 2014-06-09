#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

#include <ros/ros.h>
#include "std_msgs/String.h"
#include <sstream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

#define max(a,b) ((a) < (b) ? (b) : (a))
#define min(a,b) (a > b ? b : a)

static const std::string OPENCV_WINDOW = "Image window";

class colorTracker
{
  ros::NodeHandle nh_;
  
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  struct location {
	  int X_value;
	  int Y_value;
  };

public:
  std::vector<cv::Vec3f>::iterator itrCircles;	// iterator for the circles vector
  location objectPosition;
  
  colorTracker();
  ~colorTracker();
  void imageCb(const sensor_msgs::ImageConstPtr& msg);
};

#endif
