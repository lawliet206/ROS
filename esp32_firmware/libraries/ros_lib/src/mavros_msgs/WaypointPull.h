#ifndef _ROS_SERVICE_WaypointPull_h
#define _ROS_SERVICE_WaypointPull_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char WAYPOINTPULL[] = "mavros_msgs/WaypointPull";

  class WaypointPullRequest : public ros::Msg
  {
    public:

    WaypointPullRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return WAYPOINTPULL; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class WaypointPullResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint32_t _wp_received_type;
      _wp_received_type wp_received;

    WaypointPullResponse():
      success(0),
      wp_received(0)
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
      *(outbuffer + offset + 0) = (this->wp_received >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wp_received >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->wp_received >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->wp_received >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wp_received);
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
      this->wp_received =  ((uint32_t) (*(inbuffer + offset)));
      this->wp_received |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->wp_received |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->wp_received |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->wp_received);
     return offset;
    }

    virtual const char * getType() override { return WAYPOINTPULL; };
    virtual const char * getMD5() override { return "a8d9ecef8fb37028d2db2a9aa4ed7e79"; };

  };

  class WaypointPull {
    public:
    typedef WaypointPullRequest Request;
    typedef WaypointPullResponse Response;
  };

}
#endif
