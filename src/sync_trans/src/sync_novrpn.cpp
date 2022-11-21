#include <ros/ros.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>

#include <sensor_msgs/Image.h>
#include <geometry_msgs/PoseStamped.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <std_msgs/UInt16.h>

#include <cmath>
#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <typeinfo>
#include <fstream>
#include <string>
#include <vector>

#include <thread>

using namespace std;
typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image>  mySyncPolicy;

string basedir = "";
int frame_n = 0;
bool save_image = false;

void show_rgb(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("rgb_view", cv_bridge::toCvShare(msg, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void show_depth(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::Mat depth_image = cv_bridge::toCvShare(msg, "16UC1")->image;
    cv::imshow("depth_view", depth_image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

Eigen::Matrix3d Quaternion2RotationMatrix(const double x, const double y, const double z,const double w){

  Eigen::Quaterniond q;
  q.x() = x;
  q.y() = y;
  q.z() = z;
  q.w() = w;
  Eigen::Matrix3d R = q.normalized().toRotationMatrix();
  return R;

}

class talker{
    public:
      talker();
      void registerNodeHandle(ros::NodeHandle& _nh);
      void registerPubSub();
      void cd_callback(const sensor_msgs::ImageConstPtr rgb, const sensor_msgs::ImageConstPtr depth);
    
    private:
      message_filters::Subscriber<sensor_msgs::Image>* rgb_sub;             // topic1 input
      message_filters::Subscriber<sensor_msgs::Image>* depth_sub;           // topic2 input
      message_filters::Synchronizer<mySyncPolicy>* sync; 
      ros::NodeHandle nh; 
};

talker::talker(){
};

void talker::registerNodeHandle(ros::NodeHandle& _nh){
   nh = _nh;
}
void talker::registerPubSub(){
  image_transport::ImageTransport it(nh);           
  rgb_sub = new message_filters::Subscriber<sensor_msgs::Image>(nh, "/camera/color/image_raw", 10);
  depth_sub  = new message_filters::Subscriber<sensor_msgs::Image>(nh, "/camera/aligned_depth_to_color/image_raw", 10);
  sync = new  message_filters::Synchronizer<mySyncPolicy>(mySyncPolicy(10), *rgb_sub, *depth_sub);
  sync->registerCallback(boost::bind(&talker::cd_callback,this, _1, _2));
}

void talker::cd_callback(const sensor_msgs::ImageConstPtr rgb, const sensor_msgs::ImageConstPtr depth) 
{
  show_rgb(rgb);
  show_depth(depth);
  // cout << "rgb stamp = " << rgb -> header.stamp <<endl;
  // cout << "depth stamp = " << depth -> header.stamp <<endl;

  if (save_image){
    cout << "save success" << endl;
    cv::imwrite(basedir + to_string(frame_n) + "_main.png",cv_bridge::toCvShare(rgb, "bgr8")->image);
    cv::imwrite(basedir + to_string(frame_n) + "_depth.png",cv_bridge::toCvShare(depth, "16UC1")->image);
    save_image = false;
  }
}


void key_callback(std_msgs::UInt16ConstPtr msg){
  if(msg->data == 1){
    save_image = true;
  }
  else{
    save_image = false;
  }
}



int main(int argc, char** argv){
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  cv::namedWindow("rgb_view");
  cv::namedWindow("depth_view");
  cv::startWindowThread();
  talker mytalker;
  mytalker.registerNodeHandle(nh);
  mytalker.registerPubSub();
  ros::Subscriber kb_sub = nh.subscribe("/key_command", 10, key_callback);
  ros::spin();
  cv::destroyWindow("rgb_view");
  cv::destroyWindow("depth_view");
}