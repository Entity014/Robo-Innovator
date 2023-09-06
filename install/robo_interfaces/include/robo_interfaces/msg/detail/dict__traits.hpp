// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice

#ifndef ROBO_INTERFACES__MSG__DETAIL__DICT__TRAITS_HPP_
#define ROBO_INTERFACES__MSG__DETAIL__DICT__TRAITS_HPP_

#include "robo_interfaces/msg/detail/dict__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<robo_interfaces::msg::Dict>()
{
  return "robo_interfaces::msg::Dict";
}

template<>
inline const char * name<robo_interfaces::msg::Dict>()
{
  return "robo_interfaces/msg/Dict";
}

template<>
struct has_fixed_size<robo_interfaces::msg::Dict>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<robo_interfaces::msg::Dict>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<robo_interfaces::msg::Dict>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROBO_INTERFACES__MSG__DETAIL__DICT__TRAITS_HPP_
