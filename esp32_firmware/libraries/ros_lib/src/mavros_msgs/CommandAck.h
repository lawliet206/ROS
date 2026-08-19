#ifndef _ROS_SERVICE_CommandAck_h
#define _ROS_SERVICE_CommandAck_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char COMMANDACK[] = "mavros_msgs/CommandAck";

  class CommandAckRequest : public ros::Msg
  {
    public:
      typedef uint16_t _command_type;
      _command_type command;
      typedef uint8_t _result_type;
      _result_type result;
      typedef uint8_t _progress_type;
      _progress_type progress;
      typedef uint32_t _result_param2_type;
      _result_param2_type result_param2;

    CommandAckRequest():
      command(0),
      result(0),
      progress(0),
      result_param2(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->command >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->command >> (8 * 1)) & 0xFF;
      offset += sizeof(this->command);
      *(outbuffer + offset + 0) = (this->result >> (8 * 0)) & 0xFF;
      offset += sizeof(this->result);
      *(outbuffer + offset + 0) = (this->progress >> (8 * 0)) & 0xFF;
      offset += sizeof(this->progress);
      *(outbuffer + offset + 0) = (this->result_param2 >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->result_param2 >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->result_param2 >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->result_param2 >> (8 * 3)) & 0xFF;
      offset += sizeof(this->result_param2);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->command =  ((uint16_t) (*(inbuffer + offset)));
      this->command |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->command);
      this->result =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->result);
      this->progress =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->progress);
      this->result_param2 =  ((uint32_t) (*(inbuffer + offset)));
      this->result_param2 |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->result_param2 |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->result_param2 |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->result_param2);
     return offset;
    }

    virtual const char * getType() override { return COMMANDACK; };
    virtual const char * getMD5() override { return "1972b10ca14298d11f768e548d4d9e68"; };

  };

  class CommandAckResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    CommandAckResponse():
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

    virtual const char * getType() override { return COMMANDACK; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class CommandAck {
    public:
    typedef CommandAckRequest Request;
    typedef CommandAckResponse Response;
  };

}
#endif
