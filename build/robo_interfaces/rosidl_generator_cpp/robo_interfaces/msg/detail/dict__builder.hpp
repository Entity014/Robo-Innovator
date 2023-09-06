// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice

#ifndef ROBO_INTERFACES__MSG__DETAIL__DICT__BUILDER_HPP_
#define ROBO_INTERFACES__MSG__DETAIL__DICT__BUILDER_HPP_

#include "robo_interfaces/msg/detail/dict__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace robo_interfaces
{

namespace msg
{

namespace builder
{

class Init_Dict_value_buttons
{
public:
  explicit Init_Dict_value_buttons(::robo_interfaces::msg::Dict & msg)
  : msg_(msg)
  {}
  ::robo_interfaces::msg::Dict value_buttons(::robo_interfaces::msg::Dict::_value_buttons_type arg)
  {
    msg_.value_buttons = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robo_interfaces::msg::Dict msg_;
};

class Init_Dict_value_axes
{
public:
  explicit Init_Dict_value_axes(::robo_interfaces::msg::Dict & msg)
  : msg_(msg)
  {}
  Init_Dict_value_buttons value_axes(::robo_interfaces::msg::Dict::_value_axes_type arg)
  {
    msg_.value_axes = std::move(arg);
    return Init_Dict_value_buttons(msg_);
  }

private:
  ::robo_interfaces::msg::Dict msg_;
};

class Init_Dict_key_buttons
{
public:
  explicit Init_Dict_key_buttons(::robo_interfaces::msg::Dict & msg)
  : msg_(msg)
  {}
  Init_Dict_value_axes key_buttons(::robo_interfaces::msg::Dict::_key_buttons_type arg)
  {
    msg_.key_buttons = std::move(arg);
    return Init_Dict_value_axes(msg_);
  }

private:
  ::robo_interfaces::msg::Dict msg_;
};

class Init_Dict_key_axes
{
public:
  Init_Dict_key_axes()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Dict_key_buttons key_axes(::robo_interfaces::msg::Dict::_key_axes_type arg)
  {
    msg_.key_axes = std::move(arg);
    return Init_Dict_key_buttons(msg_);
  }

private:
  ::robo_interfaces::msg::Dict msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::robo_interfaces::msg::Dict>()
{
  return robo_interfaces::msg::builder::Init_Dict_key_axes();
}

}  // namespace robo_interfaces

#endif  // ROBO_INTERFACES__MSG__DETAIL__DICT__BUILDER_HPP_
