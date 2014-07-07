#include "DepthConverter.h"

/*	Initialize DepthConverter Object. Provide pointer to the X and Y Position to
 * 	check depth for.
 */
DepthConverter::DepthConverter(int *X_value, int *Y_value)//: it_(nh_)
{
  this->X = (int *)X_value;
  this->Y = (int *)Y_value;

  // Subscrive to input video feed and publish output video feed
  this->image_sub_ = this->nh_.subscribe(  "/camera/depth/points",
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
void DepthConverter::imageCb(const sensor_msgs::PointCloud2ConstPtr& msg)
{

  float* data = (float*)&msg->data[0];
  float x = 0, y = 0, z = 0;
  //ROS_INFO("HEIGHT: %d WIDTH: %d", msg->height, msg->width);
  //ROS_INFO("%f %f %f", *((float*)&msg->data[0]), *((float*)(&msg->data[4])), *((float*)(&msg->data[8])) );
  //ROS_INFO("%f %f %f", *(data), *(data+1), *(data+2) );
  unsigned int offset = 0;//(320)*649*4 + (240);
  offset =  (*this->Y)*msg->row_step/4 + (*this->X)*4;//(460)*msg->row_step/4 + (320)*4;
  ROS_INFO("\n\n%i,\tX=%i,\tY=%i", offset, *this->X, *this->Y);
  x = *( ((float*)(&msg->data[0])) + offset + 0);
  y = *( ((float*)(&msg->data[0])) + offset + 1);
  z = *( ((float*)(&msg->data[0])) + offset + 2);
  ROS_INFO("X = %f, Y = %f, Z = %f\n\n", x, y, z);

  /* cv_bridge::CvImagePtr cv_ptr;
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
  }*/
}
