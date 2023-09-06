// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice
#include "robo_interfaces/msg/detail/dict__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "robo_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "robo_interfaces/msg/detail/dict__struct.h"
#include "robo_interfaces/msg/detail/dict__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "rosidl_runtime_c/primitives_sequence.h"  // value_axes, value_buttons
#include "rosidl_runtime_c/primitives_sequence_functions.h"  // value_axes, value_buttons
#include "rosidl_runtime_c/string.h"  // key_axes, key_buttons
#include "rosidl_runtime_c/string_functions.h"  // key_axes, key_buttons

// forward declare type support functions


using _Dict__ros_msg_type = robo_interfaces__msg__Dict;

static bool _Dict__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _Dict__ros_msg_type * ros_message = static_cast<const _Dict__ros_msg_type *>(untyped_ros_message);
  // Field name: key_axes
  {
    size_t size = ros_message->key_axes.size;
    auto array_ptr = ros_message->key_axes.data;
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      const rosidl_runtime_c__String * str = &array_ptr[i];
      if (str->capacity == 0 || str->capacity <= str->size) {
        fprintf(stderr, "string capacity not greater than size\n");
        return false;
      }
      if (str->data[str->size] != '\0') {
        fprintf(stderr, "string not null-terminated\n");
        return false;
      }
      cdr << str->data;
    }
  }

  // Field name: key_buttons
  {
    size_t size = ros_message->key_buttons.size;
    auto array_ptr = ros_message->key_buttons.data;
    cdr << static_cast<uint32_t>(size);
    for (size_t i = 0; i < size; ++i) {
      const rosidl_runtime_c__String * str = &array_ptr[i];
      if (str->capacity == 0 || str->capacity <= str->size) {
        fprintf(stderr, "string capacity not greater than size\n");
        return false;
      }
      if (str->data[str->size] != '\0') {
        fprintf(stderr, "string not null-terminated\n");
        return false;
      }
      cdr << str->data;
    }
  }

  // Field name: value_axes
  {
    size_t size = ros_message->value_axes.size;
    auto array_ptr = ros_message->value_axes.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  // Field name: value_buttons
  {
    size_t size = ros_message->value_buttons.size;
    auto array_ptr = ros_message->value_buttons.data;
    cdr << static_cast<uint32_t>(size);
    cdr.serializeArray(array_ptr, size);
  }

  return true;
}

static bool _Dict__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _Dict__ros_msg_type * ros_message = static_cast<_Dict__ros_msg_type *>(untyped_ros_message);
  // Field name: key_axes
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->key_axes.data) {
      rosidl_runtime_c__String__Sequence__fini(&ros_message->key_axes);
    }
    if (!rosidl_runtime_c__String__Sequence__init(&ros_message->key_axes, size)) {
      return "failed to create array for field 'key_axes'";
    }
    auto array_ptr = ros_message->key_axes.data;
    for (size_t i = 0; i < size; ++i) {
      std::string tmp;
      cdr >> tmp;
      auto & ros_i = array_ptr[i];
      if (!ros_i.data) {
        rosidl_runtime_c__String__init(&ros_i);
      }
      bool succeeded = rosidl_runtime_c__String__assign(
        &ros_i,
        tmp.c_str());
      if (!succeeded) {
        fprintf(stderr, "failed to assign string into field 'key_axes'\n");
        return false;
      }
    }
  }

  // Field name: key_buttons
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->key_buttons.data) {
      rosidl_runtime_c__String__Sequence__fini(&ros_message->key_buttons);
    }
    if (!rosidl_runtime_c__String__Sequence__init(&ros_message->key_buttons, size)) {
      return "failed to create array for field 'key_buttons'";
    }
    auto array_ptr = ros_message->key_buttons.data;
    for (size_t i = 0; i < size; ++i) {
      std::string tmp;
      cdr >> tmp;
      auto & ros_i = array_ptr[i];
      if (!ros_i.data) {
        rosidl_runtime_c__String__init(&ros_i);
      }
      bool succeeded = rosidl_runtime_c__String__assign(
        &ros_i,
        tmp.c_str());
      if (!succeeded) {
        fprintf(stderr, "failed to assign string into field 'key_buttons'\n");
        return false;
      }
    }
  }

  // Field name: value_axes
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->value_axes.data) {
      rosidl_runtime_c__float__Sequence__fini(&ros_message->value_axes);
    }
    if (!rosidl_runtime_c__float__Sequence__init(&ros_message->value_axes, size)) {
      return "failed to create array for field 'value_axes'";
    }
    auto array_ptr = ros_message->value_axes.data;
    cdr.deserializeArray(array_ptr, size);
  }

  // Field name: value_buttons
  {
    uint32_t cdrSize;
    cdr >> cdrSize;
    size_t size = static_cast<size_t>(cdrSize);
    if (ros_message->value_buttons.data) {
      rosidl_runtime_c__int32__Sequence__fini(&ros_message->value_buttons);
    }
    if (!rosidl_runtime_c__int32__Sequence__init(&ros_message->value_buttons, size)) {
      return "failed to create array for field 'value_buttons'";
    }
    auto array_ptr = ros_message->value_buttons.data;
    cdr.deserializeArray(array_ptr, size);
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robo_interfaces
size_t get_serialized_size_robo_interfaces__msg__Dict(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _Dict__ros_msg_type * ros_message = static_cast<const _Dict__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name key_axes
  {
    size_t array_size = ros_message->key_axes.size;
    auto array_ptr = ros_message->key_axes.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        (array_ptr[index].size + 1);
    }
  }
  // field.name key_buttons
  {
    size_t array_size = ros_message->key_buttons.size;
    auto array_ptr = ros_message->key_buttons.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        (array_ptr[index].size + 1);
    }
  }
  // field.name value_axes
  {
    size_t array_size = ros_message->value_axes.size;
    auto array_ptr = ros_message->value_axes.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name value_buttons
  {
    size_t array_size = ros_message->value_buttons.size;
    auto array_ptr = ros_message->value_buttons.data;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    (void)array_ptr;
    size_t item_size = sizeof(array_ptr[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _Dict__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_robo_interfaces__msg__Dict(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_robo_interfaces
size_t max_serialized_size_robo_interfaces__msg__Dict(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: key_axes
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: key_buttons
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    full_bounded = false;
    for (size_t index = 0; index < array_size; ++index) {
      current_alignment += padding +
        eprosima::fastcdr::Cdr::alignment(current_alignment, padding) +
        1;
    }
  }
  // member: value_axes
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: value_buttons
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _Dict__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_robo_interfaces__msg__Dict(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_Dict = {
  "robo_interfaces::msg",
  "Dict",
  _Dict__cdr_serialize,
  _Dict__cdr_deserialize,
  _Dict__get_serialized_size,
  _Dict__max_serialized_size
};

static rosidl_message_type_support_t _Dict__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_Dict,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, robo_interfaces, msg, Dict)() {
  return &_Dict__type_support;
}

#if defined(__cplusplus)
}
#endif
