#include "DepthConverter.h"

/*	Initialize DepthConverter Object. Provide pointer to the X and Y Position to
 * 	check depth for.
 */
DepthConverter::DepthConverter(int *X_value, int *Y_value): it_(nh_)
{
  this->X = (int *)X_value;
  this->Y = (int *)Y_value;

  // Subscrive to input video feed and publish output video feed
  this->image_sub_ = it_.subscribe(  "/camera/depth/image_raw",
				      1,
				      &DepthConverter::imageCb, 
				      this
				   );
}

/*	Destructor for DepthConverter object
 */
DepthConverter::~DepthConverter()
{

}

/*	Callback Function for when new Depth Image message type is captured
 */
void DepthConverter::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr cv_ptr;
  try
  {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
    
    //ROS_DEBUG("DEPTH %i",*(this->X));
    uint16_t  d= cv_ptr->image.at<uint16_t>(*(this->X),*(this->Y));
    ROS_INFO("DEPTH %u",d);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
}
