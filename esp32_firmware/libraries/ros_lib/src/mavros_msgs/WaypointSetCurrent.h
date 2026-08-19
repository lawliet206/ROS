#ifndef _ROS_SERVICE_WaypointSetCurrent_h
#define _ROS_SERVICE_WaypointSetCurrent_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char WAYPOINTSETCURRENT[] = "mavros_msgs/WaypointSetCurrent";

  class WaypointSetCurrentRequest : public ros::Msg
  {
    public:
      typedef uint16_t _wp_seq_type;
      _wp_seq_type wp_seq;

    WaypointSetCurrentRequest():
      wp_seq(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->wp_seq >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wp_seq >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wp_seq);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->wp_seq =  ((uint16_t) (*(inbuffer + offset)));
      this->wp_seq |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->wp_seq);
     return offset;
    }

    virtual const char * getType() override { return WAYPOINTSETCURRENT; };
    virtual const char * getMD5() override { return "9541369175e0776b0fef1c988db6840f"; };

  };

  class WaypointSetCurrentResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    WaypointSetCurrentResponse():
      success(0)
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
     return offset;
    }

    virtual const char * getType() override { return WAYPOINTSETCURRENT; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class WaypointSetCurrent {
    public:
    typedef WaypointSetCurrentRequest Request;
    typedef WaypointSetCurrentResponse Response;
  };

}
#endif
