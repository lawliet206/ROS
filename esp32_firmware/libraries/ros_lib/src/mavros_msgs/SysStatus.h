#ifndef _ROS_mavros_msgs_SysStatus_h
#define _ROS_mavros_msgs_SysStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class SysStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint32_t _sensors_present_type;
      _sensors_present_type sensors_present;
      typedef uint32_t _sensors_enabled_type;
      _sensors_enabled_type sensors_enabled;
      typedef uint32_t _sensors_health_type;
      _sensors_health_type sensors_health;
      typedef uint16_t _load_type;
      _load_type load;
      typedef uint16_t _voltage_battery_type;
      _voltage_battery_type voltage_battery;
      typedef int16_t _current_battery_type;
      _current_battery_type current_battery;
      typedef int8_t _battery_remaining_type;
      _battery_remaining_type battery_remaining;
      typedef uint16_t _drop_rate_comm_type;
      _drop_rate_comm_type drop_rate_comm;
      typedef uint16_t _errors_comm_type;
      _errors_comm_type errors_comm;
      typedef uint16_t _errors_count1_type;
      _errors_count1_type errors_count1;
      typedef uint16_t _errors_count2_type;
      _errors_count2_type errors_count2;
      typedef uint16_t _errors_count3_type;
      _errors_count3_type errors_count3;
      typedef uint16_t _errors_count4_type;
      _errors_count4_type errors_count4;

    SysStatus():
      header(),
      sensors_present(0),
      sensors_enabled(0),
      sensors_health(0),
      load(0),
      voltage_battery(0),
      current_battery(0),
      battery_remaining(0),
      drop_rate_comm(0),
      errors_comm(0),
      errors_count1(0),
      errors_count2(0),
      errors_count3(0),
      errors_count4(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->sensors_present >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sensors_present >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->sensors_present >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->sensors_present >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sensors_present);
      *(outbuffer + offset + 0) = (this->sensors_enabled >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sensors_enabled >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->sensors_enabled >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->sensors_enabled >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sensors_enabled);
      *(outbuffer + offset + 0) = (this->sensors_health >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sensors_health >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->sensors_health >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->sensors_health >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sensors_health);
      *(outbuffer + offset + 0) = (this->load >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->load >> (8 * 1)) & 0xFF;
      offset += sizeof(this->load);
      *(outbuffer + offset + 0) = (this->voltage_battery >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->voltage_battery >> (8 * 1)) & 0xFF;
      offset += sizeof(this->voltage_battery);
      union {
        int16_t real;
        uint16_t base;
      } u_current_battery;
      u_current_battery.real = this->current_battery;
      *(outbuffer + offset + 0) = (u_current_battery.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current_battery.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->current_battery);
      union {
        int8_t real;
        uint8_t base;
      } u_battery_remaining;
      u_battery_remaining.real = this->battery_remaining;
      *(outbuffer + offset + 0) = (u_battery_remaining.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->battery_remaining);
      *(outbuffer + offset + 0) = (this->drop_rate_comm >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->drop_rate_comm >> (8 * 1)) & 0xFF;
      offset += sizeof(this->drop_rate_comm);
      *(outbuffer + offset + 0) = (this->errors_comm >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->errors_comm >> (8 * 1)) & 0xFF;
      offset += sizeof(this->errors_comm);
      *(outbuffer + offset + 0) = (this->errors_count1 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->errors_count1 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->errors_count1);
      *(outbuffer + offset + 0) = (this->errors_count2 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->errors_count2 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->errors_count2);
      *(outbuffer + offset + 0) = (this->errors_count3 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->errors_count3 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->errors_count3);
      *(outbuffer + offset + 0) = (this->errors_count4 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->errors_count4 >> (8 * 1)) & 0xFF;
      offset += sizeof(this->errors_count4);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->sensors_present =  ((uint32_t) (*(inbuffer + offset)));
      this->sensors_present |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sensors_present |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->sensors_present |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->sensors_present);
      this->sensors_enabled =  ((uint32_t) (*(inbuffer + offset)));
      this->sensors_enabled |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sensors_enabled |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->sensors_enabled |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->sensors_enabled);
      this->sensors_health =  ((uint32_t) (*(inbuffer + offset)));
      this->sensors_health |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->sensors_health |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->sensors_health |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->sensors_health);
      this->load =  ((uint16_t) (*(inbuffer + offset)));
      this->load |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->load);
      this->voltage_battery =  ((uint16_t) (*(inbuffer + offset)));
      this->voltage_battery |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->voltage_battery);
      union {
        int16_t real;
        uint16_t base;
      } u_current_battery;
      u_current_battery.base = 0;
      u_current_battery.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current_battery.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->current_battery = u_current_battery.real;
      offset += sizeof(this->current_battery);
      union {
        int8_t real;
        uint8_t base;
      } u_battery_remaining;
      u_battery_remaining.base = 0;
      u_battery_remaining.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->battery_remaining = u_battery_remaining.real;
      offset += sizeof(this->battery_remaining);
      this->drop_rate_comm =  ((uint16_t) (*(inbuffer + offset)));
      this->drop_rate_comm |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->drop_rate_comm);
      this->errors_comm =  ((uint16_t) (*(inbuffer + offset)));
      this->errors_comm |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->errors_comm);
      this->errors_count1 =  ((uint16_t) (*(inbuffer + offset)));
      this->errors_count1 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->errors_count1);
      this->errors_count2 =  ((uint16_t) (*(inbuffer + offset)));
      this->errors_count2 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->errors_count2);
      this->errors_count3 =  ((uint16_t) (*(inbuffer + offset)));
      this->errors_count3 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->errors_count3);
      this->errors_count4 =  ((uint16_t) (*(inbuffer + offset)));
      this->errors_count4 |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->errors_count4);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/SysStatus"; };
    virtual const char * getMD5() override { return "4039be26d76b32d20c569c754da6e25c"; };

  };

}
#endif
