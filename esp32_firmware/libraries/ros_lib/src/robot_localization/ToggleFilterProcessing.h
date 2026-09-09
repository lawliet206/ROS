#ifndef _ROS_SERVICE_ToggleFilterProcessing_h
#define _ROS_SERVICE_ToggleFilterProcessing_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace robot_localization
{

static const char TOGGLEFILTERPROCESSING[] = "robot_localization/ToggleFilterProcessing";

  class ToggleFilterProcessingRequest : public ros::Msg
  {
    public:
      typedef bool _on_type;
      _on_type on;

    ToggleFilterProcessingRequest():
      on(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_on;
      u_on.real = this->on;
      *(outbuffer + offset + 0) = (u_on.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->on);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_on;
      u_on.base = 0;
      u_on.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->on = u_on.real;
      offset += sizeof(this->on);
     return offset;
    }

    virtual const char * getType() override { return TOGGLEFILTERPROCESSING; };
    virtual const char * getMD5() override { return "74983d2ffe4877de8ae30b7a94625c41"; };

  };

  class ToggleFilterProcessingResponse : public ros::Msg
  {
    public:
      typedef bool _status_type;
      _status_type status;

    ToggleFilterProcessingResponse():
      status(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_status;
      u_status.real = this->status;
      *(outbuffer + offset + 0) = (u_status.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_status;
      u_status.base = 0;
      u_status.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->status = u_status.real;
      offset += sizeof(this->status);
     return offset;
    }

    virtual const char * getType() override { return TOGGLEFILTERPROCESSING; };
    virtual const char * getMD5() override { return "3a1255d4d998bd4d6585c64639b5ee9a"; };

  };

  class ToggleFilterProcessing {
    public:
    typedef ToggleFilterProcessingRequest Request;
    typedef ToggleFilterProcessingResponse Response;
  };

}
#endif
