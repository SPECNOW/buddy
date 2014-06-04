#include <ros/ros.h>
#include "std_msgs/String.h"
#include <sstream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";

class colorTracker
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  ros::NodeHandle nh2_;
  image_transport::ImageTransport it2_;
  image_transport::Subscriber image_sub2_;

  ros::NodeHandle n;
  ros::Publisher chatter_pub;

  std::vector<cv::Vec3f>::iterator itrCircles;	// iterator for the circles vector

public:
  colorTracker()
    : it_(nh_), it2_(nh2_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
      &colorTracker::imageCb, this);

    image_pub_ = it_.advertise("/colorTracker/output_video", 1);

    image_sub2_ = it2_.subscribe("/camera/depth/image_raw", 1,
      &colorTracker::imageCb2, this);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~colorTracker()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
      chatter_pub = n.advertise<std_msgs::String>("objectPosition", 1000);

      cv::Mat matProcessed;		// "Matrix object, used for images in OpenCV2, this will be the processed image
      std::vector<cv::Vec3f> vecCircles;			// 3 element vector of floats, this will be the pass by reference output of HoughCircles()


      // call inRange to take original image and turn into processed image
      cv::inRange(cv_ptr->image,				// funcion input
                  cv::Scalar(150,100,0),		// min filtering value (if greater than or equal to this) (in BGR format)
                  cv::Scalar(220,255,100),	// max filtering value (and if less than this) (in BGR format)
                  matProcessed);				// function output

      // smooth the processed image, this will make it easier for the next function to identify circles
      cv::GaussianBlur(matProcessed,			// function input
                       matProcessed,			// function output
                       cv::Size(9,9),			// smoothing window width and height in pixels
                       4);					// sigma value, determines how much the image will be blurred

      // fill circles vector with all circles in processed image
      cv::HoughCircles(matProcessed,				// input image
                       vecCircles,				// function output (must be a standard template library vector
                       CV_HOUGH_GRADIENT,			// two-pass algorithm for detecting circles, this is the only choice available
                       2,							// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
                       matProcessed.rows / .1,		// min distance in pixels between the centers of the detected circles
                       100,						// high threshold of Canny edge detector (called by cvHoughCircles)
                       50,						// low threshold of Canny edge detector (set at 1/2 previous value)
                       10,						// min circle radius (any circles with smaller radius will not be returned)
                       100);						// max circle radius (any circles with larger radius will not be returned)


      for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++) {

          std::cout << "object position x = " << (*itrCircles)[0]				// x position of center point of circle
          << ", y = " << (*itrCircles)[1]				// y position of center point of circle
          << ", z = " << (*itrCircles)[2] << "\n";		// radius of circle

    	//ROS_INFO("%s", msg.data.c_str());

          // draw small green circle at center of detected object
          cv::circle(cv_ptr->image,							// draw on original image
                     cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
                     3,									// radius of circle in pixels
                     cv::Scalar(0,255,0),						// draw pure green (remember, its BGR, not RGB)
                     CV_FILLED);							// thickness, fill in the circle

          // draw red circle around the detected object
          cv::circle(cv_ptr->image,							// draw on original image
                     cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
                     (int)(*itrCircles)[2],						// radius of circle in pixels
                     cv::Scalar(0,0,255),						// draw pure red (remember, its BGR, not RGB)
                     3);								// thickness of circle in pixels
      }	// end for

    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());

    std_msgs::String msg;

    std::stringstream ss;
    ss << "\n" << (*itrCircles)[0] << "\n" << (*itrCircles)[1];
    msg.data = ss.str();
    chatter_pub.publish(msg);
  }

  void imageCb2(const sensor_msgs::ImageConstPtr& msg)
    {
      cv_bridge::CvImagePtr cv_ptr2;
      try
      {
        cv_ptr2 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
      }
      catch (cv_bridge::Exception& e)
      {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
      }
    }








};
