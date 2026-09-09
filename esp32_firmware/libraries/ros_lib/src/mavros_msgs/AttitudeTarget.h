#ifndef _ROS_mavros_msgs_AttitudeTarget_h
#define _ROS_mavros_msgs_AttitudeTarget_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Vector3.h"

namespace mavros_msgs
{

  class AttitudeTarget : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _type_mask_type;
      _type_mask_type type_mask;
      typedef geometry_msgs::Quaternion _orientation_type;
      _orientation_type orientation;
      typedef geometry_msgs::Vector3 _body_rate_type;
      _body_rate_type body_rate;
      typedef float _thrust_type;
      _thrust_type thrust;
      enum { IGNORE_ROLL_RATE =  1	 };
      enum { IGNORE_PITCH_RATE =  2	 };
      enum { IGNORE_YAW_RATE =  4	 };
      enum { IGNORE_THRUST =  64 };
      enum { IGNORE_ATTITUDE =  128	 };

    AttitudeTarget():
      header(),
      type_mask(0),
      orientation(),
      body_rate(),
      thrust(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->type_mask >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type_mask);
      offset += this->orientation.serialize(outbuffer + offset);
      offset += this->body_rate.serialize(outbuffer + offset);
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
      this->type_mask =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type_mask);
      offset += this->orientation.deserialize(inbuffer + offset);
      offset += this->body_rate.deserialize(inbuffer + offset);
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

    virtual const char * getType() override { return "mavros_msgs/AttitudeTarget"; };
    virtual const char * getMD5() override { return "456f3af666b22ccd0222ea053a86b548"; };

  };

}
#endif
