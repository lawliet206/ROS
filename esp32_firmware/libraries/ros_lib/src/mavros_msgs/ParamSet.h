#ifndef _ROS_SERVICE_ParamSet_h
#define _ROS_SERVICE_ParamSet_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mavros_msgs/ParamValue.h"

namespace mavros_msgs
{

static const char PARAMSET[] = "mavros_msgs/ParamSet";

  class ParamSetRequest : public ros::Msg
  {
    public:
      typedef const char* _param_id_type;
      _param_id_type param_id;
      typedef mavros_msgs::ParamValue _value_type;
      _value_type value;

    ParamSetRequest():
      param_id(""),
      value()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_param_id = strlen(this->param_id);
      varToArr(outbuffer + offset, length_param_id);
      offset += 4;
      memcpy(outbuffer + offset, this->param_id, length_param_id);
      offset += length_param_id;
      offset += this->value.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
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
     return offset;
    }

    virtual const char * getType() override { return PARAMSET; };
    virtual const char * getMD5() override { return "4a17f346bc27984b348c799c674a04d9"; };

  };

  class ParamSetResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef mavros_msgs::ParamValue _value_type;
      _value_type value;

    ParamSetResponse():
      success(0),
      value()
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
      offset += this->value.serialize(outbuffer + offset);
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
      offset += this->value.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return PARAMSET; };
    virtual const char * getMD5() override { return "790d22b22b9dbf32a8e8d55578e25477"; };

  };

  class ParamSet {
    public:
    typedef ParamSetRequest Request;
    typedef ParamSetResponse Response;
  };

}
#endif
