#ifndef _ROS_mavros_msgs_CellularStatus_h
#define _ROS_mavros_msgs_CellularStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class CellularStatus : public ros::Msg
  {
    public:
      typedef uint8_t _status_type;
      _status_type status;
      typedef uint8_t _failure_reason_type;
      _failure_reason_type failure_reason;
      typedef uint8_t _type_type;
      _type_type type;
      typedef uint8_t _quality_type;
      _quality_type quality;
      typedef uint16_t _mcc_type;
      _mcc_type mcc;
      typedef uint16_t _mnc_type;
      _mnc_type mnc;
      typedef uint16_t _lac_type;
      _lac_type lac;

    CellularStatus():
      status(0),
      failure_reason(0),
      type(0),
      quality(0),
      mcc(0),
      mnc(0),
      lac(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->status);
      *(outbuffer + offset + 0) = (this->failure_reason >> (8 * 0)) & 0xFF;
      offset += sizeof(this->failure_reason);
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      *(outbuffer + offset + 0) = (this->quality >> (8 * 0)) & 0xFF;
      offset += sizeof(this->quality);
      *(outbuffer + offset + 0) = (this->mcc >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->mcc >> (8 * 1)) & 0xFF;
      offset += sizeof(this->mcc);
      *(outbuffer + offset + 0) = (this->mnc >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->mnc >> (8 * 1)) & 0xFF;
      offset += sizeof(this->mnc);
      *(outbuffer + offset + 0) = (this->lac >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->lac >> (8 * 1)) & 0xFF;
      offset += sizeof(this->lac);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->status);
      this->failure_reason =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->failure_reason);
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      this->quality =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->quality);
      this->mcc =  ((uint16_t) (*(inbuffer + offset)));
      this->mcc |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->mcc);
      this->mnc =  ((uint16_t) (*(inbuffer + offset)));
      this->mnc |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->mnc);
      this->lac =  ((uint16_t) (*(inbuffer + offset)));
      this->lac |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->lac);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/CellularStatus"; };
    virtual const char * getMD5() override { return "a474bdb9df111b4e16fab4c29f7a6956"; };

  };

}
#endif
