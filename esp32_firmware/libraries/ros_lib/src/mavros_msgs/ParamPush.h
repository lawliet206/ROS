#ifndef _ROS_SERVICE_ParamPush_h
#define _ROS_SERVICE_ParamPush_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char PARAMPUSH[] = "mavros_msgs/ParamPush";

  class ParamPushRequest : public ros::Msg
  {
    public:

    ParamPushRequest()
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

    virtual const char * getType() override { return PARAMPUSH; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ParamPushResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint32_t _param_transfered_type;
      _param_transfered_type param_transfered;

    ParamPushResponse():
      success(0),
      param_transfered(0)
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
      *(outbuffer + offset + 0) = (this->param_transfered >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->param_transfered >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->param_transfered >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->param_transfered >> (8 * 3)) & 0xFF;
      offset += sizeof(this->param_transfered);
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
      this->param_transfered =  ((uint32_t) (*(inbuffer + offset)));
      this->param_transfered |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->param_transfered |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->param_transfered |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->param_transfered);
     return offset;
    }

    virtual const char * getType() override { return PARAMPUSH; };
    virtual const char * getMD5() override { return "d3fc4d8cefa193382985a92a041a2a3d"; };

  };

  class ParamPush {
    public:
    typedef ParamPushRequest Request;
    typedef ParamPushResponse Response;
  };

}
#endif
