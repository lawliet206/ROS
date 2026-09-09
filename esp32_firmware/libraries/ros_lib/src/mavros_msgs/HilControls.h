#ifndef _ROS_mavros_msgs_HilControls_h
#define _ROS_mavros_msgs_HilControls_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class HilControls : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _roll_ailerons_type;
      _roll_ailerons_type roll_ailerons;
      typedef float _pitch_elevator_type;
      _pitch_elevator_type pitch_elevator;
      typedef float _yaw_rudder_type;
      _yaw_rudder_type yaw_rudder;
      typedef float _throttle_type;
      _throttle_type throttle;
      typedef float _aux1_type;
      _aux1_type aux1;
      typedef float _aux2_type;
      _aux2_type aux2;
      typedef float _aux3_type;
      _aux3_type aux3;
      typedef float _aux4_type;
      _aux4_type aux4;
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef uint8_t _nav_mode_type;
      _nav_mode_type nav_mode;

    HilControls():
      header(),
      roll_ailerons(0),
      pitch_elevator(0),
      yaw_rudder(0),
      throttle(0),
      aux1(0),
      aux2(0),
      aux3(0),
      aux4(0),
      mode(0),
      nav_mode(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_roll_ailerons;
      u_roll_ailerons.real = this->roll_ailerons;
      *(outbuffer + offset + 0) = (u_roll_ailerons.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_roll_ailerons.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_roll_ailerons.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_roll_ailerons.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->roll_ailerons);
      union {
        float real;
        uint32_t base;
      } u_pitch_elevator;
      u_pitch_elevator.real = this->pitch_elevator;
      *(outbuffer + offset + 0) = (u_pitch_elevator.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pitch_elevator.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_pitch_elevator.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_pitch_elevator.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pitch_elevator);
      union {
        float real;
        uint32_t base;
      } u_yaw_rudder;
      u_yaw_rudder.real = this->yaw_rudder;
      *(outbuffer + offset + 0) = (u_yaw_rudder.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_yaw_rudder.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_yaw_rudder.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_yaw_rudder.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->yaw_rudder);
      union {
        float real;
        uint32_t base;
      } u_throttle;
      u_throttle.real = this->throttle;
      *(outbuffer + offset + 0) = (u_throttle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_throttle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_throttle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_throttle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->throttle);
      union {
        float real;
        uint32_t base;
      } u_aux1;
      u_aux1.real = this->aux1;
      *(outbuffer + offset + 0) = (u_aux1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_aux1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_aux1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_aux1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->aux1);
      union {
        float real;
        uint32_t base;
      } u_aux2;
      u_aux2.real = this->aux2;
      *(outbuffer + offset + 0) = (u_aux2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_aux2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_aux2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_aux2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->aux2);
      union {
        float real;
        uint32_t base;
      } u_aux3;
      u_aux3.real = this->aux3;
      *(outbuffer + offset + 0) = (u_aux3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_aux3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_aux3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_aux3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->aux3);
      union {
        float real;
        uint32_t base;
      } u_aux4;
      u_aux4.real = this->aux4;
      *(outbuffer + offset + 0) = (u_aux4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_aux4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_aux4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_aux4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->aux4);
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      *(outbuffer + offset + 0) = (this->nav_mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->nav_mode);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_roll_ailerons;
      u_roll_ailerons.base = 0;
      u_roll_ailerons.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_roll_ailerons.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_roll_ailerons.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_roll_ailerons.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->roll_ailerons = u_roll_ailerons.real;
      offset += sizeof(this->roll_ailerons);
      union {
        float real;
        uint32_t base;
      } u_pitch_elevator;
      u_pitch_elevator.base = 0;
      u_pitch_elevator.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pitch_elevator.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_pitch_elevator.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_pitch_elevator.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->pitch_elevator = u_pitch_elevator.real;
      offset += sizeof(this->pitch_elevator);
      union {
        float real;
        uint32_t base;
      } u_yaw_rudder;
      u_yaw_rudder.base = 0;
      u_yaw_rudder.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_yaw_rudder.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_yaw_rudder.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_yaw_rudder.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->yaw_rudder = u_yaw_rudder.real;
      offset += sizeof(this->yaw_rudder);
      union {
        float real;
        uint32_t base;
      } u_throttle;
      u_throttle.base = 0;
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->throttle = u_throttle.real;
      offset += sizeof(this->throttle);
      union {
        float real;
        uint32_t base;
      } u_aux1;
      u_aux1.base = 0;
      u_aux1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_aux1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_aux1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_aux1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->aux1 = u_aux1.real;
      offset += sizeof(this->aux1);
      union {
        float real;
        uint32_t base;
      } u_aux2;
      u_aux2.base = 0;
      u_aux2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_aux2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_aux2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_aux2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->aux2 = u_aux2.real;
      offset += sizeof(this->aux2);
      union {
        float real;
        uint32_t base;
      } u_aux3;
      u_aux3.base = 0;
      u_aux3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_aux3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_aux3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_aux3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->aux3 = u_aux3.real;
      offset += sizeof(this->aux3);
      union {
        float real;
        uint32_t base;
      } u_aux4;
      u_aux4.base = 0;
      u_aux4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_aux4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_aux4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_aux4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->aux4 = u_aux4.real;
      offset += sizeof(this->aux4);
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      this->nav_mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->nav_mode);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HilControls"; };
    virtual const char * getMD5() override { return "698148349c3a2e5720afcae2d934acca"; };

  };

}
#endif
