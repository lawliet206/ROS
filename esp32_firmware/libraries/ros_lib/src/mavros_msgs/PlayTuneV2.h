#ifndef _ROS_mavros_msgs_PlayTuneV2_h
#define _ROS_mavros_msgs_PlayTuneV2_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class PlayTuneV2 : public ros::Msg
  {
    public:
      typedef uint8_t _format_type;
      _format_type format;
      typedef const char* _tune_type;
      _tune_type tune;
      enum { QBASIC1_1 =  1 };
      enum { MML_MODERN =  2 };

    PlayTuneV2():
      format(0),
      tune("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->format >> (8 * 0)) & 0xFF;
      offset += sizeof(this->format);
      uint32_t length_tune = strlen(this->tune);
      varToArr(outbuffer + offset, length_tune);
      offset += 4;
      memcpy(outbuffer + offset, this->tune, length_tune);
      offset += length_tune;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->format =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->format);
      uint32_t length_tune;
      arrToVar(length_tune, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_tune; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_tune-1]=0;
      this->tune = (char *)(inbuffer + offset-1);
      offset += length_tune;
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/PlayTuneV2"; };
    virtual const char * getMD5() override { return "177f99ce3572342a75a8b5cdefc8286e"; };

  };

}
#endif
