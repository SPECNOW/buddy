#include "ImageConverter.h"
  
colorTracker::colorTracker(): it_(nh_)
{
  objectPosition.X_value = 320;
  objectPosition.Y_value = 160;

  // Subscrive to input video feed and publish output video feed
  image_sub_ = it_.subscribe(
		  	  	  	  	  	  "/camera/rgb/image_raw",
		  	  	  	  	  	  1,
		  	  	  	  	  	  &colorTracker::imageCb,
		  	  	  	  	  	  this
		  	  	  	  	  	);
  image_pub_ = it_.advertise(
		  	  	  	  	  	  "/image_converter/output_video",
		  	  	  	  	  	  1
		  	  	  	  	  	);
  cv::namedWindow(OPENCV_WINDOW);
}

colorTracker::~colorTracker()
{
  cv::destroyWindow(OPENCV_WINDOW);
}

void colorTracker::imageCb(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr cv_ptr;
  try
  {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("objectPosition", 1000);
    cv::Mat matProcessed;							// "Matrix object, used for images in OpenCV2, this will be the processed image
    std::vector<cv::Vec3f> vecCircles;			// 3 element vector of floats, this will be the pass by reference output of HoughCircles()

    static unsigned int minx, maxx, miny, maxy;

    minx=max(0, objectPosition.X_value - 120);
    maxx=min(cv_ptr->image.cols-1, objectPosition.X_value + 120);
    miny=max(0, objectPosition.Y_value - 60);
    maxy=min(cv_ptr->image.rows-1, objectPosition.Y_value + 60);

    ROS_DEBUG("minx=%i, maxx=%i, miny=%i, maxy=%i",minx, maxx, miny, maxy);

    cv::Mat whatever = cv::Mat::zeros(
    									cv_ptr->image.rows+1,
    									cv_ptr->image.cols+1,
    									CV_8UC3
									  );
    ( ( (cv_ptr->image.colRange(minx, maxx) ).rowRange(miny, maxy) ) ).copyTo( ( ( whatever.colRange(minx, maxx) ).rowRange(miny, maxy) ) );
    //cv::imshow("SUBMAT", whatever);

    // call inRange to take original image and turn into processed image
    cv::inRange(
    				whatever,//cv_ptr->image,	// funcion input
    				cv::Scalar(0,95,80),		// min filtering value (if greater than or equal to this) (in BGR format)
    				cv::Scalar(80,210,160),		// max filtering value (and if less than this) (in BGR format)
    				matProcessed				// function output
				);
    cv::imshow("INRANGE", matProcessed);

    // smooth the processed image, this will make it easier for the next function to identify circles
    cv::GaussianBlur(
    					matProcessed,			// function input
    					matProcessed,			// function output
    					cv::Size(9,9),			// smoothing window width and height in pixels
    					4						// sigma value, determines how much the image will be blurred
					);
    cv::imshow("BLUR", matProcessed);


    // fill circles vector with all circles in processed image
    cv::HoughCircles(
    					matProcessed,				// input image
    					vecCircles,					// function output (must be a standard template library vector
    					CV_HOUGH_GRADIENT,			// two-pass algorithm for detecting circles, this is the only choice available
    					2,							// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
    					matProcessed.rows / .1,		// min distance in pixels between the centers of the detected circles
    					100,						// high threshold of Canny edge detector (called by cvHoughCircles)
    					50,							// low threshold of Canny edge detector (set at 1/2 previous value)
    					5,							// min circle radius (any circles with smaller radius will not be returned)
    					50							// max circle radius (any circles with larger radius will not be returned)
					);

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++)
    {
		std::cout << "object position x = " << (*itrCircles)[0]		// x position of center point of circle
		<< ", y = " << (*itrCircles)[1]								// y position of center point of circle
		<< ", z = " << (*itrCircles)[2] << "\n";					// radius of circle

		objectPosition.X_value = (int)(*itrCircles)[0];
		objectPosition.Y_value = (int)(*itrCircles)[1];

		std_msgs::String msg;

		std::stringstream ss;
		ss << "\n" << (*itrCircles)[0] << "\n" << (*itrCircles)[1];
		msg.data = ss.str();
		chatter_pub.publish(msg);

		//ROS_INFO("%s", msg.data.c_str());

		// draw small green circle at center of detected object
		cv::circle(
					cv_ptr->image,													// draw on original image
					cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
					3,																// radius of circle in pixels
					cv::Scalar(0,255,0),											// draw pure green (remember, its BGR, not RGB)
					CV_FILLED														// thickness, fill in the circle
				  );

		// draw red circle around the detected object
		cv::circle(
					cv_ptr->image,													// draw on original image
					cv::Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),		// center point of circle
					(int)(*itrCircles)[2],											// radius of circle in pixels
					cv::Scalar(0,0,255),											// draw pure red (remember, its BGR, not RGB)
					3																// thickness of circle in pixels
				  );
    }

    // Update GUI Window
    cv::Mat RED, GREEN, BLUE;
    std::vector<cv::Mat>channels(3);
    cv::split(cv_ptr->image, channels);
    BLUE = channels[0];
    GREEN = channels[1];
    RED = channels[2];

    cv::imwrite("~/grab.bmp", cv_ptr->image);
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::imshow("RED", RED);
    cv::imshow("GREEN", GREEN);
    cv::imshow("BLUE", BLUE);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }
}
