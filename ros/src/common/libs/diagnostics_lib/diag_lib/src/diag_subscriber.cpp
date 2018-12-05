#include <diag_lib/diag_subscriber.h>

DiagSubscriber::DiagSubscriber(std::string target_node,int target_node_number) : target_node_(target_node), target_node_number_(target_node_number)
{
    diag_sub_ = nh_.subscribe(target_node+"/diag", 1, &DiagSubscriber::callback_, this);
}

DiagSubscriber::~DiagSubscriber()
{

}

void DiagSubscriber::callback_(diag_msgs::diag_error msg)
{
    mtx_.lock();
    buffer_.push_back(msg);
    //ROS_ERROR_STREAM(msg);
    mtx_.unlock();
    return;
}

diag_msgs::diag_node_errors DiagSubscriber::get_diag_node_errors()
{
    diag_msgs::diag_node_errors ret;
    mtx_.lock();
    std::copy(buffer_.begin(), buffer_.end(), back_inserter(ret.errors));
    buffer_.clear();
    mtx_.unlock();
    ret.header.stamp = ros::Time::now();
    ret.node_number = target_node_number_;
    return ret;
}