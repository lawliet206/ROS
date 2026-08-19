#ifndef _ROS_mbf_msgs_GetPathResult_h
#define _ROS_mbf_msgs_GetPathResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nav_msgs/Path.h"

namespace mbf_msgs
{

  class GetPathResult : public ros::Msg
  {
    public:
      typedef uint32_t _outcome_type;
      _outcome_type outcome;
      typedef const char* _message_type;
      _message_type message;
      typedef nav_msgs::Path _path_type;
      _path_type path;
      typedef float _cost_type;
      _cost_type cost;
      enum { SUCCESS =  0 };
      enum { FAILURE =  50   };
      enum { CANCELED =  51   };
      enum { INVALID_START =  52   };
      enum { INVALID_GOAL =  53 };
      enum { NO_PATH_FOUND =  54 };
      enum { PAT_EXCEEDED =  55 };
      enum { EMPTY_PATH =  56 };
      enum { TF_ERROR =  57 };
      enum { NOT_INITIALIZED =  58 };
      enum { INVALID_PLUGIN =  59 };
      enum { INTERNAL_ERROR =  60 };
      enum { OUT_OF_MAP =  61 };
      enum { MAP_ERROR =  62 };
      enum { STOPPED =  63   };

    GetPathResult():
      outcome(0),
      message(""),
      path(),
      cost(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->outcome >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->outcome >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->outcome >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->outcome >> (8 * 3)) & 0xFF;
      offset += sizeof(this->outcome);
      uint32_t length_message = strlen(this->message);
      varToArr(outbuffer + offset, length_message);
      offset += 4;
      memcpy(outbuffer + offset, this->message, length_message);
      offset += length_message;
      offset += this->path.serialize(outbuffer + offset);
      offset += serializeAvrFloat64(outbuffer + offset, this->cost);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->outcome =  ((uint32_t) (*(inbuffer + offset)));
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->outcome);
      uint32_t length_message;
      arrToVar(length_message, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_message; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_message-1]=0;
      this->message = (char *)(inbuffer + offset-1);
      offset += length_message;
      offset += this->path.deserialize(inbuffer + offset);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->cost));
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/GetPathResult"; };
    virtual const char * getMD5() override { return "b737d51aec954f878a4cd57e83f5767c"; };

  };

}
#endif
