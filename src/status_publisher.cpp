#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "ros2_custom_msgs/msg/robot_status.hpp"

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node
{
public:
  StatusPublisher() : Node("status_publisher"), battery_(100.0), missions_(0)
  {
    publisher_ = this->create_publisher<ros2_custom_msgs::msg::RobotStatus>(
      "/robot_status", 10);

    timer_ = this->create_wall_timer(
      1000ms,
      std::bind(&StatusPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto msg = ros2_custom_msgs::msg::RobotStatus();

    msg.robot_name = "Explorer1";
    msg.battery_level = battery_;
    msg.is_active = true;
    msg.mission_count = missions_;

    RCLCPP_INFO(
      this->get_logger(),
      "Publishing: robot=%s, battery=%.1f, active=%s, missions=%d",
      msg.robot_name.c_str(),
      msg.battery_level,
      msg.is_active ? "true" : "true",
      msg.mission_count
    );

    publisher_->publish(msg);

    battery_ -= 0.5;
    missions_++;
  }

  rclcpp::Publisher<ros2_custom_msgs::msg::RobotStatus>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  double battery_;
  int missions_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusPublisher>());
  rclcpp::shutdown();
  return 0;
}
