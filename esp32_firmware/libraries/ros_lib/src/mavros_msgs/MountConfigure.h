#ifndef _ROS_SERVICE_MountConfigure_h
#define _ROS_SERVICE_MountConfigure_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

static const char MOUNTCONFIGURE[] = "mavros_msgs/MountConfigure";

  class MountConfigureRequest : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef bool _stabilize_roll_type;
      _stabilize_roll_type stabilize_roll;
      typedef bool _stabilize_pitch_type;
      _stabilize_pitch_type stabilize_pitch;
      typedef bool _stabilize_yaw_type;
      _stabilize_yaw_type stabilize_yaw;
      typedef uint8_t _roll_input_type;
      _roll_input_type roll_input;
      typedef uint8_t _pitch_input_type;
      _pitch_input_type pitch_input;
      typedef uint8_t _yaw_input_type;
      _yaw_input_type yaw_input;
      enum { MODE_RETRACT =  0 };
      enum { MODE_NEUTRAL =  1 };
      enum { MODE_MAVLINK_TARGETING =  2 };
      enum { MODE_RC_TARGETING =  3 };
      enum { MODE_GPS_POINT =  4 };
      enum { INPUT_ANGLE_BODY_FRAME =  0 };
      enum { INPUT_ANGULAR_RATE =  1 };
      enum { INPUT_ANGLE_ABSOLUTE_FRAME =  2 };

    MountConfigureRequest():
      header(),
      mode(0),
      stabilize_roll(0),
      stabilize_pitch(0),
      stabilize_yaw(0),
      roll_input(0),
      pitch_input(0),
      yaw_input(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_roll;
      u_stabilize_roll.real = this->stabilize_roll;
      *(outbuffer + offset + 0) = (u_stabilize_roll.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stabilize_roll);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_pitch;
      u_stabilize_pitch.real = this->stabilize_pitch;
      *(outbuffer + offset + 0) = (u_stabilize_pitch.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stabilize_pitch);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_yaw;
      u_stabilize_yaw.real = this->stabilize_yaw;
      *(outbuffer + offset + 0) = (u_stabilize_yaw.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stabilize_yaw);
      *(outbuffer + offset + 0) = (this->roll_input >> (8 * 0)) & 0xFF;
      offset += sizeof(this->roll_input);
      *(outbuffer + offset + 0) = (this->pitch_input >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pitch_input);
      *(outbuffer + offset + 0) = (this->yaw_input >> (8 * 0)) & 0xFF;
      offset += sizeof(this->yaw_input);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_roll;
      u_stabilize_roll.base = 0;
      u_stabilize_roll.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->stabilize_roll = u_stabilize_roll.real;
      offset += sizeof(this->stabilize_roll);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_pitch;
      u_stabilize_pitch.base = 0;
      u_stabilize_pitch.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->stabilize_pitch = u_stabilize_pitch.real;
      offset += sizeof(this->stabilize_pitch);
      union {
        bool real;
        uint8_t base;
      } u_stabilize_yaw;
      u_stabilize_yaw.base = 0;
      u_stabilize_yaw.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->stabilize_yaw = u_stabilize_yaw.real;
      offset += sizeof(this->stabilize_yaw);
      this->roll_input =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->roll_input);
      this->pitch_input =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->pitch_input);
      this->yaw_input =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->yaw_input);
     return offset;
    }

    virtual const char * getType() override { return MOUNTCONFIGURE; };
    virtual const char * getMD5() override { return "6abfbffc4f7b14d5b05955b1813ae50e"; };

  };

  class MountConfigureResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    MountConfigureResponse():
      success(0),
      result(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      *(outbuffer + offset + 0) = (this->result >> (8 * 0)) & 0xFF;
      offset += sizeof(this->result);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
      this->result =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->result);
     return offset;
    }

    virtual const char * getType() override { return MOUNTCONFIGURE; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class MountConfigure {
    public:
    typedef MountConfigureRequest Request;
    typedef MountConfigureResponse Response;
  };

}
#endif
