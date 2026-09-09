#ifndef _ROS_SERVICE_MessageInterval_h
#define _ROS_SERVICE_MessageInterval_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char MESSAGEINTERVAL[] = "mavros_msgs/MessageInterval";

  class MessageIntervalRequest : public ros::Msg
  {
    public:
      typedef uint32_t _message_id_type;
      _message_id_type message_id;
      typedef float _message_rate_type;
      _message_rate_type message_rate;

    MessageIntervalRequest():
      message_id(0),
      message_rate(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->message_id >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->message_id >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->message_id >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->message_id >> (8 * 3)) & 0xFF;
      offset += sizeof(this->message_id);
      union {
        float real;
        uint32_t base;
      } u_message_rate;
      u_message_rate.real = this->message_rate;
      *(outbuffer + offset + 0) = (u_message_rate.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_message_rate.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_message_rate.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_message_rate.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->message_rate);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->message_id =  ((uint32_t) (*(inbuffer + offset)));
      this->message_id |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->message_id |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->message_id |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->message_id);
      union {
        float real;
        uint32_t base;
      } u_message_rate;
      u_message_rate.base = 0;
      u_message_rate.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_message_rate.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_message_rate.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_message_rate.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->message_rate = u_message_rate.real;
      offset += sizeof(this->message_rate);
     return offset;
    }

    virtual const char * getType() override { return MESSAGEINTERVAL; };
    virtual const char * getMD5() override { return "e0211a7928924521de24f3981706be52"; };

  };

  class MessageIntervalResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    MessageIntervalResponse():
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

    virtual const char * getType() override { return MESSAGEINTERVAL; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class MessageInterval {
    public:
    typedef MessageIntervalRequest Request;
    typedef MessageIntervalResponse Response;
  };

}
#endif
