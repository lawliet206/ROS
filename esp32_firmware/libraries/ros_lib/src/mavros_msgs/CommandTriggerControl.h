#ifndef _ROS_SERVICE_CommandTriggerControl_h
#define _ROS_SERVICE_CommandTriggerControl_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char COMMANDTRIGGERCONTROL[] = "mavros_msgs/CommandTriggerControl";

  class CommandTriggerControlRequest : public ros::Msg
  {
    public:
      typedef bool _trigger_enable_type;
      _trigger_enable_type trigger_enable;
      typedef bool _sequence_reset_type;
      _sequence_reset_type sequence_reset;
      typedef bool _trigger_pause_type;
      _trigger_pause_type trigger_pause;

    CommandTriggerControlRequest():
      trigger_enable(0),
      sequence_reset(0),
      trigger_pause(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_trigger_enable;
      u_trigger_enable.real = this->trigger_enable;
      *(outbuffer + offset + 0) = (u_trigger_enable.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->trigger_enable);
      union {
        bool real;
        uint8_t base;
      } u_sequence_reset;
      u_sequence_reset.real = this->sequence_reset;
      *(outbuffer + offset + 0) = (u_sequence_reset.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sequence_reset);
      union {
        bool real;
        uint8_t base;
      } u_trigger_pause;
      u_trigger_pause.real = this->trigger_pause;
      *(outbuffer + offset + 0) = (u_trigger_pause.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->trigger_pause);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_trigger_enable;
      u_trigger_enable.base = 0;
      u_trigger_enable.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->trigger_enable = u_trigger_enable.real;
      offset += sizeof(this->trigger_enable);
      union {
        bool real;
        uint8_t base;
      } u_sequence_reset;
      u_sequence_reset.base = 0;
      u_sequence_reset.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->sequence_reset = u_sequence_reset.real;
      offset += sizeof(this->sequence_reset);
      union {
        bool real;
        uint8_t base;
      } u_trigger_pause;
      u_trigger_pause.base = 0;
      u_trigger_pause.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->trigger_pause = u_trigger_pause.real;
      offset += sizeof(this->trigger_pause);
     return offset;
    }

    virtual const char * getType() override { return COMMANDTRIGGERCONTROL; };
    virtual const char * getMD5() override { return "5231f3f21be52f9682a8e030770339a5"; };

  };

  class CommandTriggerControlResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    CommandTriggerControlResponse():
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

    virtual const char * getType() override { return COMMANDTRIGGERCONTROL; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class CommandTriggerControl {
    public:
    typedef CommandTriggerControlRequest Request;
    typedef CommandTriggerControlResponse Response;
  };

}
#endif
