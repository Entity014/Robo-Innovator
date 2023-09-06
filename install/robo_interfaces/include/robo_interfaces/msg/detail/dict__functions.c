// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from robo_interfaces:msg/Dict.idl
// generated code does not contain a copyright notice
#include "robo_interfaces/msg/detail/dict__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `key_axes`
// Member `key_buttons`
#include "rosidl_runtime_c/string_functions.h"
// Member `value_axes`
// Member `value_buttons`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
robo_interfaces__msg__Dict__init(robo_interfaces__msg__Dict * msg)
{
  if (!msg) {
    return false;
  }
  // key_axes
  if (!rosidl_runtime_c__String__Sequence__init(&msg->key_axes, 0)) {
    robo_interfaces__msg__Dict__fini(msg);
    return false;
  }
  // key_buttons
  if (!rosidl_runtime_c__String__Sequence__init(&msg->key_buttons, 0)) {
    robo_interfaces__msg__Dict__fini(msg);
    return false;
  }
  // value_axes
  if (!rosidl_runtime_c__float__Sequence__init(&msg->value_axes, 0)) {
    robo_interfaces__msg__Dict__fini(msg);
    return false;
  }
  // value_buttons
  if (!rosidl_runtime_c__int32__Sequence__init(&msg->value_buttons, 0)) {
    robo_interfaces__msg__Dict__fini(msg);
    return false;
  }
  return true;
}

void
robo_interfaces__msg__Dict__fini(robo_interfaces__msg__Dict * msg)
{
  if (!msg) {
    return;
  }
  // key_axes
  rosidl_runtime_c__String__Sequence__fini(&msg->key_axes);
  // key_buttons
  rosidl_runtime_c__String__Sequence__fini(&msg->key_buttons);
  // value_axes
  rosidl_runtime_c__float__Sequence__fini(&msg->value_axes);
  // value_buttons
  rosidl_runtime_c__int32__Sequence__fini(&msg->value_buttons);
}

bool
robo_interfaces__msg__Dict__are_equal(const robo_interfaces__msg__Dict * lhs, const robo_interfaces__msg__Dict * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // key_axes
  if (!rosidl_runtime_c__String__Sequence__are_equal(
      &(lhs->key_axes), &(rhs->key_axes)))
  {
    return false;
  }
  // key_buttons
  if (!rosidl_runtime_c__String__Sequence__are_equal(
      &(lhs->key_buttons), &(rhs->key_buttons)))
  {
    return false;
  }
  // value_axes
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->value_axes), &(rhs->value_axes)))
  {
    return false;
  }
  // value_buttons
  if (!rosidl_runtime_c__int32__Sequence__are_equal(
      &(lhs->value_buttons), &(rhs->value_buttons)))
  {
    return false;
  }
  return true;
}

bool
robo_interfaces__msg__Dict__copy(
  const robo_interfaces__msg__Dict * input,
  robo_interfaces__msg__Dict * output)
{
  if (!input || !output) {
    return false;
  }
  // key_axes
  if (!rosidl_runtime_c__String__Sequence__copy(
      &(input->key_axes), &(output->key_axes)))
  {
    return false;
  }
  // key_buttons
  if (!rosidl_runtime_c__String__Sequence__copy(
      &(input->key_buttons), &(output->key_buttons)))
  {
    return false;
  }
  // value_axes
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->value_axes), &(output->value_axes)))
  {
    return false;
  }
  // value_buttons
  if (!rosidl_runtime_c__int32__Sequence__copy(
      &(input->value_buttons), &(output->value_buttons)))
  {
    return false;
  }
  return true;
}

robo_interfaces__msg__Dict *
robo_interfaces__msg__Dict__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robo_interfaces__msg__Dict * msg = (robo_interfaces__msg__Dict *)allocator.allocate(sizeof(robo_interfaces__msg__Dict), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(robo_interfaces__msg__Dict));
  bool success = robo_interfaces__msg__Dict__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
robo_interfaces__msg__Dict__destroy(robo_interfaces__msg__Dict * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    robo_interfaces__msg__Dict__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
robo_interfaces__msg__Dict__Sequence__init(robo_interfaces__msg__Dict__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robo_interfaces__msg__Dict * data = NULL;

  if (size) {
    data = (robo_interfaces__msg__Dict *)allocator.zero_allocate(size, sizeof(robo_interfaces__msg__Dict), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = robo_interfaces__msg__Dict__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        robo_interfaces__msg__Dict__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
robo_interfaces__msg__Dict__Sequence__fini(robo_interfaces__msg__Dict__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      robo_interfaces__msg__Dict__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

robo_interfaces__msg__Dict__Sequence *
robo_interfaces__msg__Dict__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robo_interfaces__msg__Dict__Sequence * array = (robo_interfaces__msg__Dict__Sequence *)allocator.allocate(sizeof(robo_interfaces__msg__Dict__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = robo_interfaces__msg__Dict__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
robo_interfaces__msg__Dict__Sequence__destroy(robo_interfaces__msg__Dict__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    robo_interfaces__msg__Dict__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
robo_interfaces__msg__Dict__Sequence__are_equal(const robo_interfaces__msg__Dict__Sequence * lhs, const robo_interfaces__msg__Dict__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!robo_interfaces__msg__Dict__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
robo_interfaces__msg__Dict__Sequence__copy(
  const robo_interfaces__msg__Dict__Sequence * input,
  robo_interfaces__msg__Dict__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(robo_interfaces__msg__Dict);
    robo_interfaces__msg__Dict * data =
      (robo_interfaces__msg__Dict *)realloc(output->data, allocation_size);
    if (!data) {
      return false;
    }
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!robo_interfaces__msg__Dict__init(&data[i])) {
        /* free currently allocated and return false */
        for (; i-- > output->capacity; ) {
          robo_interfaces__msg__Dict__fini(&data[i]);
        }
        free(data);
        return false;
      }
    }
    output->data = data;
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!robo_interfaces__msg__Dict__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
