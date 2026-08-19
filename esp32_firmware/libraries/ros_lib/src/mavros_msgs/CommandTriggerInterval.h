#ifndef _ROS_SERVICE_CommandTriggerInterval_h
#define _ROS_SERVICE_CommandTriggerInterval_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char COMMANDTRIGGERINTERVAL[] = "mavros_msgs/CommandTriggerInterval";

  class CommandTriggerIntervalRequest : public ros::Msg
  {
    public:
      typedef float _cycle_time_type;
      _cycle_time_type cycle_time;
      typedef float _integration_time_type;
      _integration_time_type integration_time;

    CommandTriggerIntervalRequest():
      cycle_time(0),
      integration_time(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_cycle_time;
      u_cycle_time.real = this->cycle_time;
      *(outbuffer + offset + 0) = (u_cycle_time.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_cycle_time.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_cycle_time.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_cycle_time.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cycle_time);
      union {
        float real;
        uint32_t base;
      } u_integration_time;
      u_integration_time.real = this->integration_time;
      *(outbuffer + offset + 0) = (u_integration_time.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_integration_time.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_integration_time.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_integration_time.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->integration_time);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        float real;
        uint32_t base;
      } u_cycle_time;
      u_cycle_time.base = 0;
      u_cycle_time.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_cycle_time.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_cycle_time.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_cycle_time.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->cycle_time = u_cycle_time.real;
      offset += sizeof(this->cycle_time);
      union {
        float real;
        uint32_t base;
      } u_integration_time;
      u_integration_time.base = 0;
      u_integration_time.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_integration_time.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_integration_time.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_integration_time.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->integration_time = u_integration_time.real;
      offset += sizeof(this->integration_time);
     return offset;
    }

    virtual const char * getType() override { return COMMANDTRIGGERINTERVAL; };
    virtual const char * getMD5() override { return "54f6167a076ced593aa096ea6eb1a519"; };

  };

  class CommandTriggerIntervalResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    CommandTriggerIntervalResponse():
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

    virtual const char * getType() override { return COMMANDTRIGGERINTERVAL; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class CommandTriggerInterval {
    public:
    typedef CommandTriggerIntervalRequest Request;
    typedef CommandTriggerIntervalResponse Response;
  };

}
#endif
