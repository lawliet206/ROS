#ifndef _ROS_mavros_msgs_Waypoint_h
#define _ROS_mavros_msgs_Waypoint_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class Waypoint : public ros::Msg
  {
    public:
      typedef uint8_t _frame_type;
      _frame_type frame;
      typedef uint16_t _command_type;
      _command_type command;
      typedef bool _is_current_type;
      _is_current_type is_current;
      typedef bool _autocontinue_type;
      _autocontinue_type autocontinue;
      typedef float _param1_type;
      _param1_type param1;
      typedef float _param2_type;
      _param2_type param2;
      typedef float _param3_type;
      _param3_type param3;
      typedef float _param4_type;
      _param4_type param4;
      typedef float _x_lat_type;
      _x_lat_type x_lat;
      typedef float _y_long_type;
      _y_long_type y_long;
      typedef float _z_alt_type;
      _z_alt_type z_alt;
      enum { FRAME_GLOBAL =  0 };
      enum { FRAME_LOCAL_NED =  1 };
      enum { FRAME_MISSION =  2 };
      enum { FRAME_GLOBAL_REL_ALT =  3 };
      enum { FRAME_LOCAL_ENU =  4 };
      enum { FRAME_GLOBAL_INT =  5 };
      enum { FRAME_GLOBAL_RELATIVE_ALT_INT =  6 };
      enum { FRAME_LOCAL_OFFSET_NED =  7 };
      enum { FRAME_BODY_NED =  8 };
      enum { FRAME_BODY_OFFSET_NED =  9 };
      enum { FRAME_GLOBAL_TERRAIN_ALT =  10 };
      enum { FRAME_GLOBAL_TERRAIN_ALT_INT =  11 };
      enum { FRAME_BODY_FRD =  12 };
      enum { FRAME_RESERVED_13 =  13 };
      enum { FRAME_RESERVED_14 =  14 };
      enum { FRAME_RESERVED_15 =  15 };
      enum { FRAME_RESERVED_16 =  16 };
      enum { FRAME_RESERVED_17 =  17 };
      enum { FRAME_RESERVED_18 =  18 };
      enum { FRAME_RESERVED_19 =  19 };
      enum { FRAME_LOCAL_FRD =  20 };
      enum { FRAME_LOCAL_FLU =  21 };

    Waypoint():
      frame(0),
      command(0),
      is_current(0),
      autocontinue(0),
      param1(0),
      param2(0),
      param3(0),
      param4(0),
      x_lat(0),
      y_long(0),
      z_alt(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->frame >> (8 * 0)) & 0xFF;
      offset += sizeof(this->frame);
      *(outbuffer + offset + 0) = (this->command >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->command >> (8 * 1)) & 0xFF;
      offset += sizeof(this->command);
      union {
        bool real;
        uint8_t base;
      } u_is_current;
      u_is_current.real = this->is_current;
      *(outbuffer + offset + 0) = (u_is_current.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->is_current);
      union {
        bool real;
        uint8_t base;
      } u_autocontinue;
      u_autocontinue.real = this->autocontinue;
      *(outbuffer + offset + 0) = (u_autocontinue.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->autocontinue);
      union {
        float real;
        uint32_t base;
      } u_param1;
      u_param1.real = this->param1;
      *(outbuffer + offset + 0) = (u_param1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param1);
      union {
        float real;
        uint32_t base;
      } u_param2;
      u_param2.real = this->param2;
      *(outbuffer + offset + 0) = (u_param2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param2);
      union {
        float real;
        uint32_t base;
      } u_param3;
      u_param3.real = this->param3;
      *(outbuffer + offset + 0) = (u_param3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param3);
      union {
        float real;
        uint32_t base;
      } u_param4;
      u_param4.real = this->param4;
      *(outbuffer + offset + 0) = (u_param4.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param4.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param4.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param4.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param4);
      offset += serializeAvrFloat64(outbuffer + offset, this->x_lat);
      offset += serializeAvrFloat64(outbuffer + offset, this->y_long);
      offset += serializeAvrFloat64(outbuffer + offset, this->z_alt);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->frame =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->frame);
      this->command =  ((uint16_t) (*(inbuffer + offset)));
      this->command |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->command);
      union {
        bool real;
        uint8_t base;
      } u_is_current;
      u_is_current.base = 0;
      u_is_current.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->is_current = u_is_current.real;
      offset += sizeof(this->is_current);
      union {
        bool real;
        uint8_t base;
      } u_autocontinue;
      u_autocontinue.base = 0;
      u_autocontinue.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->autocontinue = u_autocontinue.real;
      offset += sizeof(this->autocontinue);
      union {
        float real;
        uint32_t base;
      } u_param1;
      u_param1.base = 0;
      u_param1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param1 = u_param1.real;
      offset += sizeof(this->param1);
      union {
        float real;
        uint32_t base;
      } u_param2;
      u_param2.base = 0;
      u_param2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param2 = u_param2.real;
      offset += sizeof(this->param2);
      union {
        float real;
        uint32_t base;
      } u_param3;
      u_param3.base = 0;
      u_param3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param3 = u_param3.real;
      offset += sizeof(this->param3);
      union {
        float real;
        uint32_t base;
      } u_param4;
      u_param4.base = 0;
      u_param4.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param4.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param4.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param4.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param4 = u_param4.real;
      offset += sizeof(this->param4);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->x_lat));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->y_long));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->z_alt));
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Waypoint"; };
    virtual const char * getMD5() override { return "f7090ce9f0c7ad0665de1ddadfd11ace"; };

  };

}
#endif
