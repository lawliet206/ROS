#ifndef _ROS_mavros_msgs_OverrideRCIn_h
#define _ROS_mavros_msgs_OverrideRCIn_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class OverrideRCIn : public ros::Msg
  {
    public:
      uint16_t channels[18];
      enum { CHAN_RELEASE = 0 };
      enum { CHAN_NOCHANGE = 65535 };

    OverrideRCIn():
      channels()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 18; i++){
      *(outbuffer + offset + 0) = (this->channels[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->channels[i] >> (8 * 1)) & 0xFF;
      offset += sizeof(this->channels[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 18; i++){
      this->channels[i] =  ((uint16_t) (*(inbuffer + offset)));
      this->channels[i] |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->channels[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/OverrideRCIn"; };
    virtual const char * getMD5() override { return "fd1e1c08fa504ec32737c41f45223398"; };

  };

}
#endif
