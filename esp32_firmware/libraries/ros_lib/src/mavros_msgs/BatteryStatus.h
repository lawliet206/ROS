#ifndef _ROS_mavros_msgs_BatteryStatus_h
#define _ROS_mavros_msgs_BatteryStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class BatteryStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _voltage_type;
      _voltage_type voltage;
      typedef float _current_type;
      _current_type current;
      typedef float _remaining_type;
      _remaining_type remaining;

    BatteryStatus():
      header(),
      voltage(0),
      current(0),
      remaining(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_voltage;
      u_voltage.real = this->voltage;
      *(outbuffer + offset + 0) = (u_voltage.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_voltage.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_voltage.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_voltage.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->voltage);
      union {
        float real;
        uint32_t base;
      } u_current;
      u_current.real = this->current;
      *(outbuffer + offset + 0) = (u_current.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_current.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_current.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->current);
      union {
        float real;
        uint32_t base;
      } u_remaining;
      u_remaining.real = this->remaining;
      *(outbuffer + offset + 0) = (u_remaining.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_remaining.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_remaining.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_remaining.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->remaining);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_voltage;
      u_voltage.base = 0;
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->voltage = u_voltage.real;
      offset += sizeof(this->voltage);
      union {
        float real;
        uint32_t base;
      } u_current;
      u_current.base = 0;
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->current = u_current.real;
      offset += sizeof(this->current);
      union {
        float real;
        uint32_t base;
      } u_remaining;
      u_remaining.base = 0;
      u_remaining.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_remaining.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_remaining.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_remaining.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->remaining = u_remaining.real;
      offset += sizeof(this->remaining);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/BatteryStatus"; };
    virtual const char * getMD5() override { return "8ba4ae7c602c5ae6a7e8f3ffb652dc5f"; };

  };

}
#endif
