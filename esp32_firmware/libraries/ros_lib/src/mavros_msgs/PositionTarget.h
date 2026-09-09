#ifndef _ROS_mavros_msgs_PositionTarget_h
#define _ROS_mavros_msgs_PositionTarget_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Vector3.h"

namespace mavros_msgs
{

  class PositionTarget : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _coordinate_frame_type;
      _coordinate_frame_type coordinate_frame;
      typedef uint16_t _type_mask_type;
      _type_mask_type type_mask;
      typedef geometry_msgs::Point _position_type;
      _position_type position;
      typedef geometry_msgs::Vector3 _velocity_type;
      _velocity_type velocity;
      typedef geometry_msgs::Vector3 _acceleration_or_force_type;
      _acceleration_or_force_type acceleration_or_force;
      typedef float _yaw_type;
      _yaw_type yaw;
      typedef float _yaw_rate_type;
      _yaw_rate_type yaw_rate;
      enum { FRAME_LOCAL_NED =  1 };
      enum { FRAME_LOCAL_OFFSET_NED =  7 };
      enum { FRAME_BODY_NED =  8 };
      enum { FRAME_BODY_OFFSET_NED =  9 };
      enum { IGNORE_PX =  1	 };
      enum { IGNORE_PY =  2 };
      enum { IGNORE_PZ =  4 };
      enum { IGNORE_VX =  8	 };
      enum { IGNORE_VY =  16 };
      enum { IGNORE_VZ =  32 };
      enum { IGNORE_AFX =  64	 };
      enum { IGNORE_AFY =  128 };
      enum { IGNORE_AFZ =  256 };
      enum { FORCE =  512	 };
      enum { IGNORE_YAW =  1024 };
      enum { IGNORE_YAW_RATE =  2048 };

    PositionTarget():
      header(),
      coordinate_frame(0),
      type_mask(0),
      position(),
      velocity(),
      acceleration_or_force(),
      yaw(0),
      yaw_rate(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->coordinate_frame >> (8 * 0)) & 0xFF;
      offset += sizeof(this->coordinate_frame);
      *(outbuffer + offset + 0) = (this->type_mask >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->type_mask >> (8 * 1)) & 0xFF;
      offset += sizeof(this->type_mask);
      offset += this->position.serialize(outbuffer + offset);
      offset += this->velocity.serialize(outbuffer + offset);
      offset += this->acceleration_or_force.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_yaw;
      u_yaw.real = this->yaw;
      *(outbuffer + offset + 0) = (u_yaw.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_yaw.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_yaw.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_yaw.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->yaw);
      union {
        float real;
        uint32_t base;
      } u_yaw_rate;
      u_yaw_rate.real = this->yaw_rate;
      *(outbuffer + offset + 0) = (u_yaw_rate.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_yaw_rate.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_yaw_rate.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_yaw_rate.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->yaw_rate);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->coordinate_frame =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->coordinate_frame);
      this->type_mask =  ((uint16_t) (*(inbuffer + offset)));
      this->type_mask |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->type_mask);
      offset += this->position.deserialize(inbuffer + offset);
      offset += this->velocity.deserialize(inbuffer + offset);
      offset += this->acceleration_or_force.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_yaw;
      u_yaw.base = 0;
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->yaw = u_yaw.real;
      offset += sizeof(this->yaw);
      union {
        float real;
        uint32_t base;
      } u_yaw_rate;
      u_yaw_rate.base = 0;
      u_yaw_rate.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_yaw_rate.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_yaw_rate.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_yaw_rate.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->yaw_rate = u_yaw_rate.real;
      offset += sizeof(this->yaw_rate);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/PositionTarget"; };
    virtual const char * getMD5() override { return "dedb0081aaf8cb20209737746bb49117"; };

  };

}
#endif
