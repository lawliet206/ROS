#ifndef _ROS_mavros_msgs_ESCInfoItem_h
#define _ROS_mavros_msgs_ESCInfoItem_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class ESCInfoItem : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint16_t _failure_flags_type;
      _failure_flags_type failure_flags;
      typedef uint32_t _error_count_type;
      _error_count_type error_count;
      typedef int32_t _temperature_type;
      _temperature_type temperature;

    ESCInfoItem():
      header(),
      failure_flags(0),
      error_count(0),
      temperature(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->failure_flags >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->failure_flags >> (8 * 1)) & 0xFF;
      offset += sizeof(this->failure_flags);
      *(outbuffer + offset + 0) = (this->error_count >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->error_count >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->error_count >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->error_count >> (8 * 3)) & 0xFF;
      offset += sizeof(this->error_count);
      union {
        int32_t real;
        uint32_t base;
      } u_temperature;
      u_temperature.real = this->temperature;
      *(outbuffer + offset + 0) = (u_temperature.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_temperature.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_temperature.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_temperature.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->temperature);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->failure_flags =  ((uint16_t) (*(inbuffer + offset)));
      this->failure_flags |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->failure_flags);
      this->error_count =  ((uint32_t) (*(inbuffer + offset)));
      this->error_count |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->error_count |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->error_count |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->error_count);
      union {
        int32_t real;
        uint32_t base;
      } u_temperature;
      u_temperature.base = 0;
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->temperature = u_temperature.real;
      offset += sizeof(this->temperature);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ESCInfoItem"; };
    virtual const char * getMD5() override { return "968983ff768dda90f04c5aa11caf6e74"; };

  };

}
#endif
