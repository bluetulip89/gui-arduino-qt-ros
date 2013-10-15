/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/eros_tutor_listener/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace eros_tutor_listener {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init(const std::string &topic_name) {
	ros::init(init_argc,init_argv,"eros_tutor_listener");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
    //chatter_subscriber= n.subscribe(topic_name, 1000, &QNode::chatterCallback, this);
    chatter_subscriber= n.subscribe(topic_name, 1000, &QNode::chatterCallback, this);
	start();
	return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url, const std::string &topic_name) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"eros_tutor_listener");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
    //chatter_subscriber = n.subscribe(topic_name, 1000, &QNode::chatterCallback, this);
    chatter_subscriber = n.subscribe(topic_name, 1000, &QNode::chatterCallback, this);
	start();
	return true;
}

void QNode::run() {
    ros::spin();
}


void QNode::chatterCallback(const std_msgs::String::ConstPtr &msg){
    ROS_INFO("I heard: [%s]", msg->data.c_str());
    logging.insertRows(0,1);
    std::stringstream logging_msg;
    logging_msg << "[ INFO][" << ros::Time::now() << "]: I heard:" << msg->data;
    QVariant new_row(QString(logging_msg.str().c_str()));
    logging.setData(logging.index(0),new_row);
}

}  // namespace eros_tutor_listener
