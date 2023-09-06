// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "robo_interfaces/msg/detail/dict__rosidl_typesupport_introspection_c.h"
#include "robo_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "robo_interfaces/msg/detail/dict__functions.h"
#include "robo_interfaces/msg/detail/dict__struct.h"


// Include directives for member types
// Member `key_axes`
// Member `key_buttons`
#include "rosidl_runtime_c/string_functions.h"
// Member `value_axes`
// Member `value_buttons`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void Dict__rosidl_typesupport_introspection_c__Dict_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  robo_interfaces__msg__Dict__init(message_memory);
}

void Dict__rosidl_typesupport_introspection_c__Dict_fini_function(void * message_memory)
{
  robo_interfaces__msg__Dict__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember Dict__rosidl_typesupport_introspection_c__Dict_message_member_array[4] = {
  {
    "key_axes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robo_interfaces__msg__Dict, key_axes),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "key_buttons",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robo_interfaces__msg__Dict, key_buttons),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "value_axes",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robo_interfaces__msg__Dict, value_axes),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "value_buttons",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(robo_interfaces__msg__Dict, value_buttons),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers Dict__rosidl_typesupport_introspection_c__Dict_message_members = {
  "robo_interfaces__msg",  // message namespace
  "Dict",  // message name
  4,  // number of fields
  sizeof(robo_interfaces__msg__Dict),
  Dict__rosidl_typesupport_introspection_c__Dict_message_member_array,  // message members
  Dict__rosidl_typesupport_introspection_c__Dict_init_function,  // function to initialize message memory (memory has to be allocated)
  Dict__rosidl_typesupport_introspection_c__Dict_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t Dict__rosidl_typesupport_introspection_c__Dict_message_type_support_handle = {
  0,
  &Dict__rosidl_typesupport_introspection_c__Dict_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_robo_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, robo_interfaces, msg, Dict)() {
  if (!Dict__rosidl_typesupport_introspection_c__Dict_message_type_support_handle.typesupport_identifier) {
    Dict__rosidl_typesupport_introspection_c__Dict_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &Dict__rosidl_typesupport_introspection_c__Dict_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
