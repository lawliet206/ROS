#ifndef _ROS_SERVICE_CommandLong_h
#define _ROS_SERVICE_CommandLong_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char COMMANDLONG[] = "mavros_msgs/CommandLong";

  class CommandLongRequest : public ros::Msg
  {
    public:
      typedef bool _broadcast_type;
      _broadcast_type broadcast;
      typedef uint16_t _command_type;
      _command_type command;
      typedef uint8_t _confirmation_type;
      _confirmation_type confirmation;
      typedef float _param1_type;
      _param1_type param1;
      typedef float _param2_type;
      _param2_type param2;
      typedef float _param3_type;
      _param3_type param3;
      typedef float _param4_type;
      _param4_type param4;
      typedef float _param5_type;
      _param5_type param5;
      typedef float _param6_type;
      _param6_type param6;
      typedef float _param7_type;
      _param7_type param7;

    CommandLongRequest():
      broadcast(0),
      command(0),
      confirmation(0),
      param1(0),
      param2(0),
      param3(0),
      param4(0),
      param5(0),
      param6(0),
      param7(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_broadcast;
      u_broadcast.real = this->broadcast;
      *(outbuffer + offset + 0) = (u_broadcast.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->broadcast);
      *(outbuffer + offset + 0) = (this->command >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->command >> (8 * 1)) & 0xFF;
      offset += sizeof(this->command);
      *(outbuffer + offset + 0) = (this->confirmation >> (8 * 0)) & 0xFF;
      offset += sizeof(this->confirmation);
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
      union {
        float real;
        uint32_t base;
      } u_param5;
      u_param5.real = this->param5;
      *(outbuffer + offset + 0) = (u_param5.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param5.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param5.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param5.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param5);
      union {
        float real;
        uint32_t base;
      } u_param6;
      u_param6.real = this->param6;
      *(outbuffer + offset + 0) = (u_param6.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param6.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param6.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param6.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param6);
      union {
        float real;
        uint32_t base;
      } u_param7;
      u_param7.real = this->param7;
      *(outbuffer + offset + 0) = (u_param7.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_param7.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_param7.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_param7.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param7);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_broadcast;
      u_broadcast.base = 0;
      u_broadcast.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->broadcast = u_broadcast.real;
      offset += sizeof(this->broadcast);
      this->command =  ((uint16_t) (*(inbuffer + offset)));
      this->command |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->command);
      this->confirmation =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->confirmation);
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
      union {
        float real;
        uint32_t base;
      } u_param5;
      u_param5.base = 0;
      u_param5.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param5.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param5.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param5.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param5 = u_param5.real;
      offset += sizeof(this->param5);
      union {
        float real;
        uint32_t base;
      } u_param6;
      u_param6.base = 0;
      u_param6.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param6.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param6.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param6.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param6 = u_param6.real;
      offset += sizeof(this->param6);
      union {
        float real;
        uint32_t base;
      } u_param7;
      u_param7.base = 0;
      u_param7.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_param7.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_param7.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_param7.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->param7 = u_param7.real;
      offset += sizeof(this->param7);
     return offset;
    }

    virtual const char * getType() override { return COMMANDLONG; };
    virtual const char * getMD5() override { return "0ad16dd8ca2c8f209bfc6c32c71c0dd8"; };

  };

  class CommandLongResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint8_t _result_type;
      _result_type result;

    CommandLongResponse():
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

    virtual const char * getType() override { return COMMANDLONG; };
    virtual const char * getMD5() override { return "1cd894375e4e3d2861d2222772894fdb"; };

  };

  class CommandLong {
    public:
    typedef CommandLongRequest Request;
    typedef CommandLongResponse Response;
  };

}
#endif
