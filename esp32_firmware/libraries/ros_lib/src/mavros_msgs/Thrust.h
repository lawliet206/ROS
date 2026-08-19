#ifndef _ROS_mavros_msgs_Thrust_h
#define _ROS_mavros_msgs_Thrust_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class Thrust : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _thrust_type;
      _thrust_type thrust;

    Thrust():
      header(),
      thrust(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_thrust;
      u_thrust.real = this->thrust;
      *(outbuffer + offset + 0) = (u_thrust.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_thrust.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_thrust.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_thrust.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->thrust);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_thrust;
      u_thrust.base = 0;
      u_thrust.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_thrust.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_thrust.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_thrust.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->thrust = u_thrust.real;
      offset += sizeof(this->thrust);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Thrust"; };
    virtual const char * getMD5() override { return "c61da3a8868a8b502eaf0b0abd839f57"; };

  };

}
#endif
