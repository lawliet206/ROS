#ifndef _ROS_mavros_msgs_Param_h
#define _ROS_mavros_msgs_Param_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/ParamValue.h"

namespace mavros_msgs
{

  class Param : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef const char* _param_id_type;
      _param_id_type param_id;
      typedef mavros_msgs::ParamValue _value_type;
      _value_type value;
      typedef uint16_t _param_index_type;
      _param_index_type param_index;
      typedef uint16_t _param_count_type;
      _param_count_type param_count;

    Param():
      header(),
      param_id(""),
      value(),
      param_index(0),
      param_count(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      uint32_t length_param_id = strlen(this->param_id);
      varToArr(outbuffer + offset, length_param_id);
      offset += 4;
      memcpy(outbuffer + offset, this->param_id, length_param_id);
      offset += length_param_id;
      offset += this->value.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->param_index >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->param_index >> (8 * 1)) & 0xFF;
      offset += sizeof(this->param_index);
      *(outbuffer + offset + 0) = (this->param_count >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->param_count >> (8 * 1)) & 0xFF;
      offset += sizeof(this->param_count);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t length_param_id;
      arrToVar(length_param_id, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_param_id; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_param_id-1]=0;
      this->param_id = (char *)(inbuffer + offset-1);
      offset += length_param_id;
      offset += this->value.deserialize(inbuffer + offset);
      this->param_index =  ((uint16_t) (*(inbuffer + offset)));
      this->param_index |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->param_index);
      this->param_count =  ((uint16_t) (*(inbuffer + offset)));
      this->param_count |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->param_count);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Param"; };
    virtual const char * getMD5() override { return "62165a8f212050223dda9583b0f22c3c"; };

  };

}
#endif
