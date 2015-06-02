#include <mcr_geometric_relation_monitors/component_wise_pose_error_monitor_node.h>

ComponentWisePoseErrorMonitorNode::ComponentWisePoseErrorMonitorNode(ros::NodeHandle &nh) : node_handler_(nh)
{
    dynamic_reconfig_server_.setCallback(boost::bind(&ComponentWisePoseErrorMonitorNode::dynamicReconfigCallback, this, _1, _2));
    event_sub_ = node_handler_.subscribe("event_in", 1, &ComponentWisePoseErrorMonitorNode::eventCallback, this);
    error_sub_ = node_handler_.subscribe("pose_error", 1, &ComponentWisePoseErrorMonitorNode::errorCallback, this);
    event_pub_ = node_handler_.advertise<std_msgs::String>("event_out", 1);
    run_state_ = INIT;
    start_pose_error_monitor_ = false;
    pose_error_sub_status_ = false;  
}

ComponentWisePoseErrorMonitorNode::~ComponentWisePoseErrorMonitorNode()
{
    event_sub_.shutdown();
    error_sub_.shutdown();
    event_pub_.shutdown();  
}

void ComponentWisePoseErrorMonitorNode::dynamicReconfigCallback(mcr_geometric_relation_monitors::ComponentWisePoseErrorMonitorConfig &config, uint32_t level)
{
    threshold_linear_x_ = config.threshold_linear_x;
    threshold_linear_y_ = config.threshold_linear_y;
    threshold_linear_z_ = config.threshold_linear_z;
    threshold_angular_x_ = config.threshold_angular_x;
    threshold_angular_y_ = config.threshold_angular_y;
    threshold_angular_z_ = config.threshold_angular_z;
}

void ComponentWisePoseErrorMonitorNode::eventCallback(const std_msgs::String &event_command)
{
    if (event_command.data == "e_start") {
        start_pose_error_monitor_ = true;
        ROS_INFO("ENABLED");
    } else if (event_command.data == "e_stop") {
        start_pose_error_monitor_ = false;
        ROS_INFO("DISABLED");
    }
}

void ComponentWisePoseErrorMonitorNode::errorCallback(const mcr_manipulation_msgs::ComponentWiseCartesianDifference::ConstPtr &pose_eror)
{
    error_ = *pose_eror;
    pose_error_sub_status_ = true;
}

void ComponentWisePoseErrorMonitorNode::states()
{
    switch (run_state_) {
        case INIT:
            initState();
            break;
        case IDLE:
            idleState();
            break;
        case RUNNING:
            runState();
            break;
        default:
            initState();
    }
}

void ComponentWisePoseErrorMonitorNode::initState()
{
    if (start_pose_error_monitor_) {
        run_state_ = IDLE;  
    } else {
        run_state_ = INIT;
    }
}

void ComponentWisePoseErrorMonitorNode::idleState()
{
    if (pose_error_sub_status_) {
        run_state_ = RUNNING;
        pose_error_sub_status_ = false;
    } else {
        run_state_ = IDLE;
    }
}

void ComponentWisePoseErrorMonitorNode::runState()
{
    if(isComponentWisePoseErrorWithinThreshold()){
        status_msg_.data = "e_error_within_threshold";
        event_pub_.publish(status_msg_);
    }

    if (start_pose_error_monitor_) {
        run_state_ = IDLE;
    } else {
        run_state_ = INIT;
    }
}

bool ComponentWisePoseErrorMonitorNode::isComponentWisePoseErrorWithinThreshold()
{

    if( (fabs(error_.linear.x) < threshold_linear_x_) && (fabs(error_.linear.y) < threshold_linear_y_) && (fabs(error_.linear.z) < threshold_linear_z_))
    {
        if( (fabs(error_.angular.x) < threshold_angular_x_) && (fabs(error_.angular.y) < threshold_angular_y_) && (fabs(error_.angular.z) < threshold_angular_z_)){
            return true;
        }
    }

    return false;

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "component_wise_pose_error_monitor");
    ros::NodeHandle nh("~");
    ROS_INFO("Initialised");
    ComponentWisePoseErrorMonitorNode cwpem(nh);

    double loop_rate = 30;
    nh.param<double>("loop_rate", loop_rate, 30);
    ros::Rate rate(loop_rate);

    while (ros::ok()) {
        ros::spinOnce();
        cwpem.states();
        rate.sleep();
    }
    
    return 0;
}
