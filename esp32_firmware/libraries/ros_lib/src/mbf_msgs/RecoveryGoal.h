#ifndef _ROS_mbf_msgs_RecoveryGoal_h
#define _ROS_mbf_msgs_RecoveryGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mbf_msgs
{

  class RecoveryGoal : public ros::Msg
  {
    public:
      typedef const char* _behavior_type;
      _behavior_type behavior;
      typedef uint8_t _concurrency_slot_type;
      _concurrency_slot_type concurrency_slot;

    RecoveryGoal():
      behavior(""),
      concurrency_slot(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_behavior = strlen(this->behavior);
      varToArr(outbuffer + offset, length_behavior);
      offset += 4;
      memcpy(outbuffer + offset, this->behavior, length_behavior);
      offset += length_behavior;
      *(outbuffer + offset + 0) = (this->concurrency_slot >> (8 * 0)) & 0xFF;
      offset += sizeof(this->concurrency_slot);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_behavior;
      arrToVar(length_behavior, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_behavior; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_behavior-1]=0;
      this->behavior = (char *)(inbuffer + offset-1);
      offset += length_behavior;
      this->concurrency_slot =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->concurrency_slot);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/RecoveryGoal"; };
    virtual const char * getMD5() override { return "ce28884316a172b85e57b78a84014451"; };

  };

}
#endif
