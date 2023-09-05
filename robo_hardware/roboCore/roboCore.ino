#include <micro_ros_arduino.h>
#include <Arduino.h>
#include <Servo.h>
#include <stdio.h>
#include <Encoder.h>
#include <math.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <rmw_microros/rmw_microros.h>
#include <geometry_msgs/msg/twist.h>
#include <nav_msgs/msg/odometry.h>
#include <std_msgs/msg/float32.h>
#include <sensor_msgs/msg/imu.h>
#include <sensor_msgs/msg/magnetic_field.h>

#include "Wire.h"
#include "config/pin_robo.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>
#include "HMC5883L.h"
#include <SimpleKalmanFilter.h>

#define RCCHECK(fn)              \
  {                              \
    rcl_ret_t temp_rc = fn;      \
    if ((temp_rc != RCL_RET_OK)) \
    {                            \
      return false;              \
    }                            \
  }
#define EXECUTE_EVERY_N_MS(MS, X)      \
  do                                   \
  {                                    \
    static volatile int64_t init = -1; \
    if (init == -1)                    \
    {                                  \
      init = uxr_millis();             \
    }                                  \
    if (uxr_millis() - init > MS)      \
    {                                  \
      X;                               \
      init = uxr_millis();             \
    }                                  \
  } while (0)

//-----------------------------------------------------------------------------------//

int ina1 = 0, ina2 = 0, ina3 = 0, ina4 = 0;
int inb1 = 0, inb2 = 0, inb3 = 0, inb4 = 0;

static uint32_t preT = 0;
bool preTS = false;

Servo servoFront;
Servo servoLeft;
Servo servoRight;
Servo servoArm;
Servo servoHandLeft;
Servo servoHandRight;

Encoder Encoder_spin(encoderB_1, encoderA_1);

MPU6050 accelgyro;
Adafruit_BMP085 bmp;
HMC5883L mag;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
float heading_;

const float accel_scale_ = 1 / 16384.0;
const float gyro_scale_ = 1 / 131.0;
const float g_to_accel_ = 9.80665;

int preServoF = 90, preServoL = 90, preServoR = 90;
int preMotorF = -1, preMotorL = -1, preMotorR = -1;
int preLimit = -1;

int state_drive = 0;
int state_running = 0;

unsigned long prev_odom_update = 0;

double pos_x = 0, pos_y = 0;
float q[4];

const int RESOLUTION = 20;
const unsigned long RPM_UPDATE_INTERVAL = 500;
const int RPM_OFFSET = 0;

float theta = 0.0;
float average_rps_x = 0.0;
float average_rps_y = 0.0;
int RPM_F = 0, RPM_L = 0, RPM_R = 0;
volatile unsigned long pulseCountF = 0, pulseCountL = 0, pulseCountR = 0;
volatile unsigned long lastPulseTime = 0;
volatile bool pulseDetected_F = false, pulseDetected_L = false, pulseDetected_R = false;

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

//-----------------------------------------------------------------------------------//
int lim_switch()
{
  return digitalRead(limit_s0);
}

void euler_to_quat(float roll, float pitch, float yaw, float *q)
{
  float cy = cos(yaw * 0.5);
  float sy = sin(yaw * 0.5);
  float cp = cos(pitch * 0.5);
  float sp = sin(pitch * 0.5);
  float cr = cos(roll * 0.5);
  float sr = sin(roll * 0.5);

  q[0] = cy * cp * cr + sy * sp * sr;
  q[1] = cy * cp * sr - sy * sp * cr;
  q[2] = sy * cp * sr + cy * sp * cr;
  q[3] = sy * cp * cr - cy * sp * sr;
}

void countF()
{
  pulseCountF++;
  pulseDetected_F = true;
}

void countL()
{
  pulseCountL++;
  pulseDetected_L = true;
}

void countR()
{
  pulseCountR++;
  pulseDetected_R = true;
}

int calculateRPM(volatile unsigned long &pulseCount, volatile bool &pulseDetected)
{
  int currentRPM = 0;
  noInterrupts();
  unsigned long elapsedTime = millis() - lastPulseTime;
  if (pulseDetected && elapsedTime != 0)
  {
    currentRPM = (60000 * pulseCount) / (RESOLUTION * elapsedTime);
    currentRPM -= RPM_OFFSET; // Adjust for offset
  }
  else
  {
    currentRPM = 0;
  }
  pulseCount = 0;
  pulseDetected = false;
  interrupts();

  return currentRPM;
}

sensor_msgs__msg__Imu imu_getDate()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  sensor_msgs__msg__Imu imu;
  imu.header.frame_id.data = const_cast<char *>("imu_link");
  imu.header.stamp.sec = millis() / 1000;
  imu.header.stamp.nanosec = (millis() % 1000) * 1000000;
  imu.angular_velocity.x = gx * (double)gyro_scale_ * DEG_TO_RAD;
  imu.angular_velocity.y = gy * (double)gyro_scale_ * DEG_TO_RAD;
  imu.angular_velocity.z = gz * (double)gyro_scale_ * DEG_TO_RAD;
  imu.linear_acceleration.x = ax * (double)accel_scale_ * g_to_accel_;
  imu.linear_acceleration.y = ay * (double)accel_scale_ * g_to_accel_;
  imu.linear_acceleration.z = az * (double)accel_scale_ * g_to_accel_;

  return imu;
}

sensor_msgs__msg__MagneticField mag_getData()
{
  mag.getHeading(&mx, &my, &mz);
  sensor_msgs__msg__MagneticField magnet;
  magnet.header.frame_id.data = const_cast<char *>("imu_link");
  magnet.header.stamp.sec = millis() / 1000;
  magnet.header.stamp.nanosec = (millis() % 1000) * 1000000;
  magnet.magnetic_field.x = mx;
  magnet.magnetic_field.y = my;
  magnet.magnetic_field.z = mz;
  return magnet;
}

//-----------------------------------------------------------------------------------//

void drive_fun(float pwm_f, float pwm_l, float pwm_r)
{
  if (pwm_f > 0)
  {
    ina1 = 1;
    inb1 = 0;
  }
  else if (pwm_f == 0)
  {
    ina1 = 1;
    inb1 = 1;
  }
  else
  {
    ina1 = 0;
    inb1 = 1;
  }
  if (pwm_l > 0)
  {
    ina2 = 1;
    inb2 = 0;
  }
  else if (pwm_l == 0)
  {
    ina2 = 1;
    inb2 = 1;
  }
  else
  {
    ina2 = 0;
    inb2 = 1;
  }
  if (pwm_r > 0)
  {
    ina3 = 1;
    inb3 = 0;
  }
  else if (pwm_r == 0)
  {
    ina3 = 1;
    inb3 = 1;
  }
  else
  {
    ina3 = 0;
    inb3 = 1;
  }
  analogWrite(PWMF, abs(pwm_f));
  analogWrite(PWML, abs(pwm_l));
  analogWrite(PWMR, abs(pwm_r));
  digitalWrite(INAF, ina1);
  digitalWrite(INBF, inb1);
  digitalWrite(INAL, ina2);
  digitalWrite(INBL, inb2);
  digitalWrite(INAR, ina3);
  digitalWrite(INBR, inb3);
}

void servo_rotation(float pwm_f, float pwm_l, float pwm_r, float thetaF, float thetaL, float thetaR)
{
  if ((pwm_f != 0 || pwm_l != 0 || pwm_r != 0)) //) && (state_drive == 0))
  {
    servoFront.write(thetaF);
    servoLeft.write(thetaL);
    servoRight.write(thetaR);
    // state_drive++;
  }
}

void spinning(float pwm)
{
  if (pwm > 0)
  {
    digitalWrite(INAS, 1);
    digitalWrite(INBS, 0);
  }
  else if (pwm < 0)
  {
    digitalWrite(INAS, 0);
    digitalWrite(INBS, 1);
  }
  else
  {
    digitalWrite(INAS, 1);
    digitalWrite(INBS, 1);
  }
  analogWrite(PWMS, abs(pwm));
}

void gribbing(float thetaA, float thetaHL, float thetaHR)
{
  servoArm.write(thetaA);
  servoHandLeft.write(thetaHL);
  servoHandRight.write(thetaHR);
}

//-----------------------------------------------------------------------------------//
// basic
rclc_support_t support;
rcl_node_t node;
rcl_timer_t timer;
rcl_allocator_t allocator;
rclc_executor_t executor;

geometry_msgs__msg__Twist genaral_msg;
nav_msgs__msg__Odometry odom_msg;
sensor_msgs__msg__Imu imu_msg;
sensor_msgs__msg__MagneticField mag_msg;

rcl_publisher_t publisher_genaral;
rcl_publisher_t publisher_imu;
rcl_publisher_t publisher_mag;
rcl_publisher_t publisher_odom;

// subscriber 1
rcl_subscription_t subscriber_drive;
rcl_subscription_t subscriber_theta;
rcl_subscription_t subscriber_command;
std_msgs__msg__Float32 theta_msg;
geometry_msgs__msg__Twist drive_msg;
geometry_msgs__msg__Twist command_msg;

rcl_init_options_t init_options;

bool micro_ros_init_successful;

enum states
{
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} state;

//-----------------------------------------------------------------------------------//

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  (void)last_call_time;
  if (timer != NULL)
  {
    if (preLimit != lim_switch())
    {
      preLimit = lim_switch();
      if (lim_switch() == false)
      {
        if (state_running < 2)
        {
          state_running++;
        }
        else
        {
          state_running = 1;
        }
        Encoder_spin.write(0);
        genaral_msg.linear.z = state_running;
      }
    }

    imu_msg = imu_getDate();
    mag_msg = mag_getData();

    unsigned long now = millis();
    float deltha_t = (now - prev_odom_update) / 1000.0;
    prev_odom_update = now;

    if (millis() - lastPulseTime >= RPM_UPDATE_INTERVAL)
    {
      RPM_F = calculateRPM(pulseCountF, pulseDetected_F);
      RPM_L = calculateRPM(pulseCountL, pulseDetected_L);
      RPM_R = calculateRPM(pulseCountR, pulseDetected_R);
      lastPulseTime = millis();
    }

    odom_msg.header.frame_id.data = const_cast<char *>("odom");
    odom_msg.child_frame_id.data = const_cast<char *>("base_footprint");
    odom_msg.header.stamp.sec = millis() / 1000;
    odom_msg.header.stamp.nanosec = (millis() % 1000) * 1000000;

    float delta_heading = (abs(gz) >= 100) ? gz * (double)gyro_scale_ * DEG_TO_RAD * deltha_t : 0.0;
    heading_ -= delta_heading;
    euler_to_quat(0, 0, heading_, q);

    float rps = (((float)((RPM_F * (preMotorF / abs(preMotorF))) + (RPM_L * (preMotorL / abs(preMotorL))) + (RPM_R * (preMotorR / abs(preMotorR)))) / 3) / 60.0);
    average_rps_x = rps * sin(theta * DEG_TO_RAD) * (preMotorF / abs(preMotorF));      // RPM
    average_rps_y = rps * cos(theta * DEG_TO_RAD) * (preMotorR / abs(preMotorR)) * -1; // RPM

    pos_x += average_rps_x * (PI * 0.02465 * 2) * deltha_t;
    pos_y += average_rps_y * (PI * 0.02465 * 2) * deltha_t;

    odom_msg.pose.pose.position.x = pos_x;
    odom_msg.pose.pose.position.y = pos_y;
    odom_msg.pose.pose.position.z = 0.0;

    odom_msg.pose.pose.orientation.x = (double)q[1];
    odom_msg.pose.pose.orientation.y = (double)q[2];
    odom_msg.pose.pose.orientation.z = (double)q[3];
    odom_msg.pose.pose.orientation.w = (double)q[0];

    odom_msg.twist.twist.linear.x = average_rps_x * (PI * 0.02465 * 2);
    odom_msg.twist.twist.linear.y = average_rps_y * (PI * 0.02465 * 2);
    odom_msg.twist.twist.linear.z = 0.0;

    odom_msg.twist.twist.angular.x = 0.0;
    odom_msg.twist.twist.angular.y = 0.0;
    odom_msg.twist.twist.angular.z = gz * (double)gyro_scale_ * DEG_TO_RAD;

    genaral_msg.linear.x = float(Encoder_spin.read());
    genaral_msg.linear.y = q[3];
    genaral_msg.angular.x = pos_x;
    genaral_msg.angular.y = pos_y;
    genaral_msg.angular.z = heading_;

    rcl_publish(&publisher_genaral, &genaral_msg, NULL);
    rcl_publish(&publisher_imu, &imu_msg, NULL);
    rcl_publish(&publisher_mag, &mag_msg, NULL);
    rcl_publish(&publisher_odom, &odom_msg, NULL);
  }
}

//-----------------------------------------------------------------------------------//

void subscription_drive_callback(const void *msgin)
{
  const geometry_msgs__msg__Twist *drive_msg = (const geometry_msgs__msg__Twist *)msgin;
  preMotorF = drive_msg->linear.x;
  preMotorL = drive_msg->linear.y;
  preMotorR = drive_msg->linear.z;
  servo_rotation(drive_msg->linear.x, drive_msg->linear.y, drive_msg->linear.z, drive_msg->angular.x, drive_msg->angular.y, drive_msg->angular.z);
  // if (state_drive >= 1 || drive_msg->linear.x == 0.0 || drive_msg->linear.y == 0.0 || drive_msg->linear.z == 0.0)
  // {
  drive_fun(drive_msg->linear.x, drive_msg->linear.y, drive_msg->linear.z);
  //   state_drive = 0;
  // }
}

void subscription_command_callback(const void *msgin)
{
  const geometry_msgs__msg__Twist *command_msg = (const geometry_msgs__msg__Twist *)msgin;
  spinning(command_msg->linear.x);
  gribbing(command_msg->angular.x, command_msg->angular.y, command_msg->angular.z);
}

void subscription_theta_callback(const void *msgin)
{
  const std_msgs__msg__Float32 *theta_msg = (const std_msgs__msg__Float32 *)msgin;
  theta = theta_msg->data;
}
//-----------------------------------------------------------------------------------//

bool create_entities()
{
  allocator = rcl_get_default_allocator();

  init_options = rcl_get_zero_initialized_init_options();
  rcl_init_options_init(&init_options, allocator);
  rcl_init_options_set_domain_id(&init_options, 10);

  rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);

  // create node
  RCCHECK(rclc_node_init_default(&node, "int32_publisher_rclc", "", &support));

  // create publisher 1
  RCCHECK(rclc_publisher_init_best_effort(
      &publisher_genaral,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      "genaral_topic"));

  // create publisher 2
  RCCHECK(rclc_publisher_init_best_effort(
      &publisher_imu,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
      "imu/data_raw"));

  RCCHECK(rclc_publisher_init_best_effort(
      &publisher_mag,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, MagneticField),
      "imu/mag"));

  // create publisher 3
  RCCHECK(rclc_publisher_init_best_effort(
      &publisher_odom,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
      "odom/unfiltered"));

  // create timer,
  const unsigned int timer_timeout = 100;
  RCCHECK(rclc_timer_init_default(
      &timer,
      &support,
      RCL_MS_TO_NS(timer_timeout),
      timer_callback));

  // create subscriber 1
  RCCHECK(rclc_subscription_init_default(
      &subscriber_drive,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      "drive_topic"));

  // create subscriber 2
  RCCHECK(rclc_subscription_init_default(
      &subscriber_command,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      "command_topic"));

  // create subscriber 2
  RCCHECK(rclc_subscription_init_default(
      &subscriber_theta,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
      "theta_topic"));

  // create executor
  executor = rclc_executor_get_zero_initialized_executor();
  RCCHECK(rclc_executor_init(&executor, &support.context, 4, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber_drive, &drive_msg, &subscription_drive_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber_command, &command_msg, &subscription_command_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber_theta, &command_msg, &subscription_theta_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  return true;
}

void destroy_entities()
{
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  rcl_publisher_fini(&publisher_genaral, &node);
  rcl_publisher_fini(&publisher_imu, &node);
  rcl_publisher_fini(&publisher_mag, &node);
  rcl_publisher_fini(&publisher_odom, &node);
  rcl_subscription_fini(&subscriber_drive, &node);
  rcl_subscription_fini(&subscriber_command, &node);
  rcl_subscription_fini(&subscriber_theta, &node);
  rcl_timer_fini(&timer);
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void renew()
{
  digitalWrite(PWMF, LOW);
  digitalWrite(PWML, LOW);
  digitalWrite(PWMR, LOW);
  digitalWrite(INAF, HIGH);
  digitalWrite(INBF, HIGH);
  digitalWrite(INAL, HIGH);
  digitalWrite(INBL, HIGH);
  digitalWrite(INAR, HIGH);
  digitalWrite(INBR, HIGH);
  servoFront.write(90);
  servoLeft.write(90);
  servoRight.write(90);
  servoArm.write(90);
  servoHandLeft.write(90);
  servoHandRight.write(90);
}

void setup()
{
  Wire.begin();
  set_microros_transports();
  pinMode(LED_PIN, OUTPUT);
  pinMode(limit_s0, INPUT_PULLUP);

  pinMode(PWMF, OUTPUT);
  pinMode(PWML, OUTPUT);
  pinMode(PWMR, OUTPUT);
  pinMode(INAF, OUTPUT);
  pinMode(INAL, OUTPUT);
  pinMode(INAR, OUTPUT);
  pinMode(INBF, OUTPUT);
  pinMode(INBL, OUTPUT);
  pinMode(INBR, OUTPUT);

  servoFront.attach(servoF);
  servoLeft.attach(servoL);
  servoRight.attach(servoR);
  servoArm.attach(servoA);
  servoHandLeft.attach(servoHL);
  servoHandRight.attach(servoHR);
  servoFront.write(90);
  servoLeft.write(90);
  servoRight.write(90);
  servoArm.write(90);
  servoHandLeft.write(90);
  servoHandRight.write(90);

  accelgyro.initialize();
  accelgyro.setI2CBypassEnabled(true);
  accelgyro.setXAccelOffset(2547.5);
  accelgyro.setYAccelOffset(-268.5);
  accelgyro.setZAccelOffset(1515.5);
  accelgyro.setXGyroOffset(53.5);
  accelgyro.setYGyroOffset(18.5);
  accelgyro.setZGyroOffset(14.5);

  mag.initialize();

  state = WAITING_AGENT;

  attachInterrupt(digitalPinToInterrupt(ENCODER_F), countF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_L), countL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_R), countR, CHANGE);
  // msg_pub1.linear.x = 0.0;
  // msg_pub1.linear.y = 0.0;
  // msg_pub1.linear.z = 0.0;
  // msg_pub1.angular.x = 0.0;
  // msg_pub1.angular.y = 0.0;
  // msg_pub1.angular.z = 0.0;

  // msg_sub1.linear.x = 0.0;
  // msg_sub1.linear.y = 0.0;
  // msg_sub1.linear.z = 0.0;
  // msg_sub1.angular.x = 0.0;
  // msg_sub1.angular.y = 0.0;
  // msg_sub1.angular.z = 0.0;
}

void loop()
{
  switch (state)
  {
  case WAITING_AGENT:
    EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
    break;
  case AGENT_AVAILABLE:
    state = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
    if (state == WAITING_AGENT)
    {
      destroy_entities();
    };
    break;
  case AGENT_CONNECTED:
    EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
    if (state == AGENT_CONNECTED)
    {
      rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
    break;
  case AGENT_DISCONNECTED:
    destroy_entities();
    state = WAITING_AGENT;
    break;
  default:
    break;
  }

  if (state == AGENT_CONNECTED)
  {
    digitalWrite(LED_PIN, 1);
  }
  else
  {
    if (millis() - preT > 250)
    {
      if (preTS)
        digitalWrite(LED_PIN, HIGH);
      else
        digitalWrite(LED_PIN, LOW);
      preT = millis();
      preTS = !preTS;
    }
    renew();
  }
}
