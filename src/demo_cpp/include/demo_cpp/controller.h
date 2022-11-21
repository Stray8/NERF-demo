#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <geometry_msgs/PoseStamped.h>

#include <mavros_msgs/PositionTarget.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <vector>
#include <ros/ros.h>
#include <cmath>
#include <angles/angles.h>

#include <tf/tf.h>
#include <tf/transform_datatypes.h>

#include <fstream>
#include <sstream>


class Controller{
private:
    std::vector<std::vector<double> >  waypoints;
    int idx;
    bool isarrived;
public:
    mavros_msgs::State current_state;
    geometry_msgs::PoseStamped local_pos;
    mavros_msgs::PositionTarget target_pos;
    ros::Publisher target_pub;
    ros::Publisher vision_pub;

public:
    Controller();
    void setWaypoints();    
    geometry_msgs::PoseStamped getPose();
    mavros_msgs::PositionTarget getWaypoint();
    void pos_callback(geometry_msgs::PoseStampedConstPtr msg);
    void state_callback(mavros_msgs::StateConstPtr state);
};
inline void Quaternion2Euler(geometry_msgs::PoseStamped& pose_msg){
    tf::Quaternion quat;
    tf::quaternionMsgToTF(pose_msg.pose.orientation, quat);
    double roll, pitch, yaw;
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
    ROS_INFO_STREAM("RPY is " << roll*180/M_PI << " " << pitch*180/M_PI << " " << yaw*180/M_PI);
}
#endif