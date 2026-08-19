#ifndef _ROS_SERVICE_LogRequestList_h
#define _ROS_SERVICE_LogRequestList_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char LOGREQUESTLIST[] = "mavros_msgs/LogRequestList";

  class LogRequestListRequest : public ros::Msg
  {
    public:
      typedef uint16_t _start_type;
      _start_type start;
      typedef uint16_t _end_type;
      _end_type end;

    LogRequestListRequest():
      start(0),
      end(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->start >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->start >> (8 * 1)) & 0xFF;
      offset += sizeof(this->start);
      *(outbuffer + offset + 0) = (this->end >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->end >> (8 * 1)) & 0xFF;
      offset += sizeof(this->end);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->start =  ((uint16_t) (*(inbuffer + offset)));
      this->start |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->start);
      this->end =  ((uint16_t) (*(inbuffer + offset)));
      this->end |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->end);
     return offset;
    }

    virtual const char * getType() override { return LOGREQUESTLIST; };
    virtual const char * getMD5() override { return "43d5acd48e3ef1843fa7f45876501c02"; };

  };

  class LogRequestListResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    LogRequestListResponse():
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

    virtual const char * getType() override { return LOGREQUESTLIST; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class LogRequestList {
    public:
    typedef LogRequestListRequest Request;
    typedef LogRequestListResponse Response;
  };

}
#endif
