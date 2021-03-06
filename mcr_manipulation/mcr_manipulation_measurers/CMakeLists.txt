cmake_minimum_required(VERSION 2.8.3)
project(mcr_manipulation_measurers)

find_package(catkin REQUIRED
  COMPONENTS
    rospy
    roslint
)

catkin_python_setup()

catkin_package(
  CATKIN_DEPENDS
    rospy
    std_msgs
    sensor_msgs
    geometry_msgs
    mcr_manipulation_msgs
    tf
    message_runtime
)

### TESTS
roslint_python(
  ros/scripts/joint_distance_measurer_node
)

if(CATKIN_ENABLE_TESTING)
  find_package(roslaunch REQUIRED)
  find_package(rostest REQUIRED)

  roslaunch_add_file_check(ros/launch)
  add_rostest(ros/launch/joint_distance_measurer.test)
  catkin_add_nosetests(ros/test/component_wise_pose_error_calculator_unittest.py)
  catkin_add_nosetests(ros/test/joint_distance_measurer_unittest.py)
endif()

install(PROGRAMS
  ros/scripts/joint_distance_measurer_node
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
