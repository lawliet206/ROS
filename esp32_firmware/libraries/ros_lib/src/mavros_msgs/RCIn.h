#ifndef _ROS_mavros_msgs_RCIn_h
#define _ROS_mavros_msgs_RCIn_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class RCIn : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _rssi_type;
      _rssi_type rssi;
      uint32_t channels_length;
      typedef uint16_t _channels_type;
      _channels_type st_channels;
      _channels_type * channels;

    RCIn():
      header(),
      rssi(0),
      channels_length(0), st_channels(), channels(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->rssi >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rssi);
      *(outbuffer + offset + 0) = (this->channels_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->channels_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->channels_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->channels_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->channels_length);
      for( uint32_t i = 0; i < channels_length; i++){
      *(outbuffer + offset + 0) = (this->channels[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->channels[i] >> (8 * 1)) & 0xFF;
      offset += sizeof(this->channels[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->rssi =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rssi);
      uint32_t channels_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      channels_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      channels_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      channels_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->channels_length);
      if(channels_lengthT > channels_length)
        this->channels = (uint16_t*)realloc(this->channels, channels_lengthT * sizeof(uint16_t));
      channels_length = channels_lengthT;
      for( uint32_t i = 0; i < channels_length; i++){
      this->st_channels =  ((uint16_t) (*(inbuffer + offset)));
      this->st_channels |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->st_channels);
        memcpy( &(this->channels[i]), &(this->st_channels), sizeof(uint16_t));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/RCIn"; };
    virtual const char * getMD5() override { return "1c3eafdb5efbcda3c334e1788bbcfe39"; };

  };

}
#endif
