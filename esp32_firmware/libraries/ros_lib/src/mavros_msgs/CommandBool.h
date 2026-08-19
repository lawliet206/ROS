#ifndef _ROS_SERVICE_CommandBool_h
#define _ROS_SERVICE_CommandBool_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char COMMANDBOOL[] = "mavros_msgs/CommandBool";

  class CommandBoolRequest : public ros::Msg
  {
    public:
      typedef bool _value_type;
      _value_type value;

    CommandBoolRequest():
      value(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_value;
      u_value.real = this->value;
      *(outbuffer + offset + 0) = (u_value.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->value);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_value;
      u_value.base = 0;
      u_value.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->value = u_value.real;
      offset += sizeof(this->value);
     return offset;
    }

    virtual const char * getType() override { return COMMANDBOOL; };
    virtual const char * getMD5() override { return "e431d687bf4b2c65fbd94b12ae0cb5d9"; };

  };

  class CommandBoolResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    CommandBoolResponse():
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

    virtual const char * getType() override { return COMMANDBOOL; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class CommandBool {
    public:
    typedef CommandBoolRequest Request;
    typedef CommandBoolResponse Response;
  };

}
#endif
