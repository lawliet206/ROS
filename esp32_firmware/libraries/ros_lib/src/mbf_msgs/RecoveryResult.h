#ifndef _ROS_mbf_msgs_RecoveryResult_h
#define _ROS_mbf_msgs_RecoveryResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mbf_msgs
{

  class RecoveryResult : public ros::Msg
  {
    public:
      typedef uint32_t _outcome_type;
      _outcome_type outcome;
      typedef const char* _message_type;
      _message_type message;
      typedef const char* _used_plugin_type;
      _used_plugin_type used_plugin;
      enum { SUCCESS =  0 };
      enum { FAILURE =  150 };
      enum { CANCELED =  151 };
      enum { PAT_EXCEEDED =  152 };
      enum { TF_ERROR =  153 };
      enum { NOT_INITIALIZED =  154 };
      enum { INVALID_PLUGIN =  155 };
      enum { INTERNAL_ERROR =  156 };
      enum { STOPPED =  157   };
      enum { IMPASSABLE =  158   };

    RecoveryResult():
      outcome(0),
      message(""),
      used_plugin("")
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
      uint32_t length_used_plugin = strlen(this->used_plugin);
      varToArr(outbuffer + offset, length_used_plugin);
      offset += 4;
      memcpy(outbuffer + offset, this->used_plugin, length_used_plugin);
      offset += length_used_plugin;
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
      uint32_t length_used_plugin;
      arrToVar(length_used_plugin, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_used_plugin; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_used_plugin-1]=0;
      this->used_plugin = (char *)(inbuffer + offset-1);
      offset += length_used_plugin;
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/RecoveryResult"; };
    virtual const char * getMD5() override { return "41d522f528f315af4a6c19e2fde7a3d0"; };

  };

}
#endif
