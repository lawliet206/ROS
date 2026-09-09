#ifndef _ROS_mavros_msgs_ESCStatus_h
#define _ROS_mavros_msgs_ESCStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/ESCStatusItem.h"

namespace mavros_msgs
{

  class ESCStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t esc_status_length;
      typedef mavros_msgs::ESCStatusItem _esc_status_type;
      _esc_status_type st_esc_status;
      _esc_status_type * esc_status;

    ESCStatus():
      header(),
      esc_status_length(0), st_esc_status(), esc_status(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->esc_status_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->esc_status_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->esc_status_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->esc_status_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->esc_status_length);
      for( uint32_t i = 0; i < esc_status_length; i++){
      offset += this->esc_status[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t esc_status_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      esc_status_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      esc_status_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      esc_status_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->esc_status_length);
      if(esc_status_lengthT > esc_status_length)
        this->esc_status = (mavros_msgs::ESCStatusItem*)realloc(this->esc_status, esc_status_lengthT * sizeof(mavros_msgs::ESCStatusItem));
      esc_status_length = esc_status_lengthT;
      for( uint32_t i = 0; i < esc_status_length; i++){
      offset += this->st_esc_status.deserialize(inbuffer + offset);
        memcpy( &(this->esc_status[i]), &(this->st_esc_status), sizeof(mavros_msgs::ESCStatusItem));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ESCStatus"; };
    virtual const char * getMD5() override { return "145d49eaf5cfecbdfd50ae4a22fe82d4"; };

  };

}
#endif
