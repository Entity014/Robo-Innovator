// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice

#ifndef ROBO_INTERFACES__MSG__DETAIL__DICT__STRUCT_H_
#define ROBO_INTERFACES__MSG__DETAIL__DICT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'key_axes'
// Member 'key_buttons'
#include "rosidl_runtime_c/string.h"
// Member 'value_axes'
// Member 'value_buttons'
#include "rosidl_runtime_c/primitives_sequence.h"

// Struct defined in msg/Dict in the package robo_interfaces.
typedef struct robo_interfaces__msg__Dict
{
  rosidl_runtime_c__String__Sequence key_axes;
  rosidl_runtime_c__String__Sequence key_buttons;
  rosidl_runtime_c__float__Sequence value_axes;
  rosidl_runtime_c__int32__Sequence value_buttons;
} robo_interfaces__msg__Dict;

// Struct for a sequence of robo_interfaces__msg__Dict.
typedef struct robo_interfaces__msg__Dict__Sequence
{
  robo_interfaces__msg__Dict * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robo_interfaces__msg__Dict__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBO_INTERFACES__MSG__DETAIL__DICT__STRUCT_H_
