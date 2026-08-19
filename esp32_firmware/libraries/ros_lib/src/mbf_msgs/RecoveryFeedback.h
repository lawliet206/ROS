#ifndef _ROS_mbf_msgs_RecoveryFeedback_h
#define _ROS_mbf_msgs_RecoveryFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mbf_msgs
{

  class RecoveryFeedback : public ros::Msg
  {
    public:

    RecoveryFeedback()
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

    virtual const char * getType() override { return "mbf_msgs/RecoveryFeedback"; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

}
#endif
