#ifndef _ROS_mavros_msgs_ParamValue_h
#define _ROS_mavros_msgs_ParamValue_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class ParamValue : public ros::Msg
  {
    public:
      typedef int64_t _integer_type;
      _integer_type integer;
      typedef float _real_type;
      _real_type real;

    ParamValue():
      integer(0),
      real(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int64_t real;
        uint64_t base;
      } u_integer;
      u_integer.real = this->integer;
      *(outbuffer + offset + 0) = (u_integer.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_integer.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_integer.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_integer.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_integer.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_integer.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_integer.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_integer.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->integer);
      offset += serializeAvrFloat64(outbuffer + offset, this->real);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int64_t real;
        uint64_t base;
      } u_integer;
      u_integer.base = 0;
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_integer.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->integer = u_integer.real;
      offset += sizeof(this->integer);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->real));
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ParamValue"; };
    virtual const char * getMD5() override { return "e2cb1c7a0f6ef0c62d450cd9362c980d"; };

  };

}
#endif
