#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "zbar_ros/barcodewithlocation.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

	ros::Publisher barcode_depth_pub;
	sensor_msgs::ImageConstPtr last_depth_img;

	void imageCb(const sensor_msgs::ImageConstPtr &image){
	last_depth_img = image;
	}

	void barcodeCb(const zbar_ros::barcodewithlocationConstPtr &bcode){
	  cv::Mat depth_img = cv_bridge::toCvShare(last_depth_img)->image;
	  zbar_ros::barcodewithlocation barcode_depth;
      barcode_depth.data = bcode->data;
      barcode_depth.x = bcode->x;
      barcode_depth.y = bcode->y;
      barcode_depth.depth = depth_img.at<uint16_t>(barcode_depth.x, barcode_depth.y);
      barcode_depth.size = bcode->size;
      barcode_depth_pub.publish(barcode_depth);
	}

	int main(int argc, char **argv){
		ros::init(argc, argv, "qr_kinect_depth");
		ros::NodeHandle n;
		ros::Subscriber depth_sub = n.subscribe("camera/depth/image_raw", 10, imageCb);
		barcode_depth_pub = n.advertise<zbar_ros::barcodewithlocation>("barcodewithdepth", 10);
		ros::Subscriber barcode_sub = n.subscribe("barcode", 10, barcodeCb);
		ros::spin();
	}