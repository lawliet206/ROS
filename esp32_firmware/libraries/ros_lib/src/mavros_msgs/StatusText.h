#ifndef _ROS_mavros_msgs_StatusText_h
#define _ROS_mavros_msgs_StatusText_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class StatusText : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _severity_type;
      _severity_type severity;
      typedef const char* _text_type;
      _text_type text;
      enum { EMERGENCY =  0 };
      enum { ALERT =  1 };
      enum { CRITICAL =  2 };
      enum { ERROR =  3 };
      enum { WARNING =  4 };
      enum { NOTICE =  5 };
      enum { INFO =  6 };
      enum { DEBUG =  7 };

    StatusText():
      header(),
      severity(0),
      text("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->severity >> (8 * 0)) & 0xFF;
      offset += sizeof(this->severity);
      uint32_t length_text = strlen(this->text);
      varToArr(outbuffer + offset, length_text);
      offset += 4;
      memcpy(outbuffer + offset, this->text, length_text);
      offset += length_text;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->severity =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->severity);
      uint32_t length_text;
      arrToVar(length_text, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_text; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_text-1]=0;
      this->text = (char *)(inbuffer + offset-1);
      offset += length_text;
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/StatusText"; };
    virtual const char * getMD5() override { return "6cfd33cd09e4abf5841d7be3c770a969"; };

  };

}
#endif
