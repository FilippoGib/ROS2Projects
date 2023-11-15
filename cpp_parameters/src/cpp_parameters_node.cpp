#include <chrono>
#include <functional>
#include <string>

#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class MinimalParam : public rclcpp::Node
{
public:
  MinimalParam()
  : Node("minimal_param_node")
  {

    auto param_desc = rcl_interfaces::msg::ParameterDescriptor{}; //we provied a description for the parameter
    param_desc.description = "This parameter is mine!";

    this->declare_parameter("my_parameter", "world",param_desc);

    timer_ = this->create_wall_timer(
      1000ms, std::bind(&MinimalParam::timer_callback, this));
  }

  void timer_callback()
  {
    std::string my_param = this->get_parameter("my_parameter").as_string();

    RCLCPP_INFO(this->get_logger(), "Hello %s!", my_param.c_str());

    //the two lines below set the parameters back to the default values in case the user externally changes them
    std::vector<rclcpp::Parameter> all_new_parameters{rclcpp::Parameter("my_parameter", "world")};
    this->set_parameters(all_new_parameters);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalParam>()); //"spin" starte processing data from the node. We pass to spin() a new instance of MinimalParam
  rclcpp::shutdown(); //when the node is done, shut it down.
  return 0;
}