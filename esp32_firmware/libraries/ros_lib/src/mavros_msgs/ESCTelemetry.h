#ifndef _ROS_mavros_msgs_ESCTelemetry_h
#define _ROS_mavros_msgs_ESCTelemetry_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/ESCTelemetryItem.h"

namespace mavros_msgs
{

  class ESCTelemetry : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t esc_telemetry_length;
      typedef mavros_msgs::ESCTelemetryItem _esc_telemetry_type;
      _esc_telemetry_type st_esc_telemetry;
      _esc_telemetry_type * esc_telemetry;

    ESCTelemetry():
      header(),
      esc_telemetry_length(0), st_esc_telemetry(), esc_telemetry(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->esc_telemetry_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->esc_telemetry_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->esc_telemetry_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->esc_telemetry_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->esc_telemetry_length);
      for( uint32_t i = 0; i < esc_telemetry_length; i++){
      offset += this->esc_telemetry[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t esc_telemetry_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      esc_telemetry_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      esc_telemetry_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      esc_telemetry_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->esc_telemetry_length);
      if(esc_telemetry_lengthT > esc_telemetry_length)
        this->esc_telemetry = (mavros_msgs::ESCTelemetryItem*)realloc(this->esc_telemetry, esc_telemetry_lengthT * sizeof(mavros_msgs::ESCTelemetryItem));
      esc_telemetry_length = esc_telemetry_lengthT;
      for( uint32_t i = 0; i < esc_telemetry_length; i++){
      offset += this->st_esc_telemetry.deserialize(inbuffer + offset);
        memcpy( &(this->esc_telemetry[i]), &(this->st_esc_telemetry), sizeof(mavros_msgs::ESCTelemetryItem));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ESCTelemetry"; };
    virtual const char * getMD5() override { return "7b1fb252ca6aa175fe8dd23d029b3362"; };

  };

}
#endif
