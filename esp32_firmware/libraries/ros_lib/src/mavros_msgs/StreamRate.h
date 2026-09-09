#ifndef _ROS_SERVICE_StreamRate_h
#define _ROS_SERVICE_StreamRate_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char STREAMRATE[] = "mavros_msgs/StreamRate";

  class StreamRateRequest : public ros::Msg
  {
    public:
      typedef uint8_t _stream_id_type;
      _stream_id_type stream_id;
      typedef uint16_t _message_rate_type;
      _message_rate_type message_rate;
      typedef bool _on_off_type;
      _on_off_type on_off;
      enum { STREAM_ALL =  0 };
      enum { STREAM_RAW_SENSORS =  1 };
      enum { STREAM_EXTENDED_STATUS =  2 };
      enum { STREAM_RC_CHANNELS =  3 };
      enum { STREAM_RAW_CONTROLLER =  4 };
      enum { STREAM_POSITION =  6 };
      enum { STREAM_EXTRA1 =  10 };
      enum { STREAM_EXTRA2 =  11 };
      enum { STREAM_EXTRA3 =  12 };

    StreamRateRequest():
      stream_id(0),
      message_rate(0),
      on_off(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->stream_id >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stream_id);
      *(outbuffer + offset + 0) = (this->message_rate >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->message_rate >> (8 * 1)) & 0xFF;
      offset += sizeof(this->message_rate);
      union {
        bool real;
        uint8_t base;
      } u_on_off;
      u_on_off.real = this->on_off;
      *(outbuffer + offset + 0) = (u_on_off.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->on_off);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->stream_id =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->stream_id);
      this->message_rate =  ((uint16_t) (*(inbuffer + offset)));
      this->message_rate |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->message_rate);
      union {
        bool real;
        uint8_t base;
      } u_on_off;
      u_on_off.base = 0;
      u_on_off.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->on_off = u_on_off.real;
      offset += sizeof(this->on_off);
     return offset;
    }

    virtual const char * getType() override { return STREAMRATE; };
    virtual const char * getMD5() override { return "d12f7661724c8ba25f67ba597bb7d039"; };

  };

  class StreamRateResponse : public ros::Msg
  {
    public:

    StreamRateResponse()
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

    virtual const char * getType() override { return STREAMRATE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class StreamRate {
    public:
    typedef StreamRateRequest Request;
    typedef StreamRateResponse Response;
  };

}
#endif
