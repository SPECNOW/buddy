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

  DepthConverter(int *X_value, int *Y_value)
  	  : it_(nh_)
  {
	  this->X = (int *)X_value;
	  this->Y = (int *)Y_value;
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/depth/image_raw", 1,
      &DepthConverter::imageCb, this);
  }
  ~DepthConverter()
  {

  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    //ROS_DEBUG("DEPTH %i",*(this->X));
    uint16_t  d= cv_ptr->image.at<uint16_t>(*(this->X),*(this->Y));
    ROS_INFO("DEPTH %u",d);



  }

};
