#ifndef _ROS_SERVICE_GetState_h
#define _ROS_SERVICE_GetState_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace robot_localization
{

static const char GETSTATE[] = "robot_localization/GetState";

  class GetStateRequest : public ros::Msg
  {
    public:
      typedef ros::Time _time_stamp_type;
      _time_stamp_type time_stamp;
      typedef const char* _frame_id_type;
      _frame_id_type frame_id;

    GetStateRequest():
      time_stamp(),
      frame_id("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->time_stamp.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_stamp.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_stamp.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_stamp.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_stamp.sec);
      *(outbuffer + offset + 0) = (this->time_stamp.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_stamp.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_stamp.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_stamp.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_stamp.nsec);
      uint32_t length_frame_id = strlen(this->frame_id);
      varToArr(outbuffer + offset, length_frame_id);
      offset += 4;
      memcpy(outbuffer + offset, this->frame_id, length_frame_id);
      offset += length_frame_id;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->time_stamp.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_stamp.sec);
      this->time_stamp.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_stamp.nsec);
      uint32_t length_frame_id;
      arrToVar(length_frame_id, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_frame_id; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_frame_id-1]=0;
      this->frame_id = (char *)(inbuffer + offset-1);
      offset += length_frame_id;
     return offset;
    }

    virtual const char * getType() override { return GETSTATE; };
    virtual const char * getMD5() override { return "853815113280ed7c4ea64ad795f27171"; };

  };

  class GetStateResponse : public ros::Msg
  {
    public:
      float state[15];
      float covariance[225];

    GetStateResponse():
      state(),
      covariance()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 15; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->state[i]);
      }
      for( uint32_t i = 0; i < 225; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->covariance[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 15; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->state[i]));
      }
      for( uint32_t i = 0; i < 225; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->covariance[i]));
      }
     return offset;
    }

    virtual const char * getType() override { return GETSTATE; };
    virtual const char * getMD5() override { return "8d49e6249cf8371736e3286b16a7ce83"; };

  };

  class GetState {
    public:
    typedef GetStateRequest Request;
    typedef GetStateResponse Response;
  };

}
#endif
