#ifndef _ROS_mavros_msgs_ESCInfo_h
#define _ROS_mavros_msgs_ESCInfo_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/ESCInfoItem.h"

namespace mavros_msgs
{

  class ESCInfo : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint16_t _counter_type;
      _counter_type counter;
      typedef uint8_t _count_type;
      _count_type count;
      typedef uint8_t _connection_type_type;
      _connection_type_type connection_type;
      typedef uint8_t _info_type;
      _info_type info;
      uint32_t esc_info_length;
      typedef mavros_msgs::ESCInfoItem _esc_info_type;
      _esc_info_type st_esc_info;
      _esc_info_type * esc_info;

    ESCInfo():
      header(),
      counter(0),
      count(0),
      connection_type(0),
      info(0),
      esc_info_length(0), st_esc_info(), esc_info(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->counter >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->counter >> (8 * 1)) & 0xFF;
      offset += sizeof(this->counter);
      *(outbuffer + offset + 0) = (this->count >> (8 * 0)) & 0xFF;
      offset += sizeof(this->count);
      *(outbuffer + offset + 0) = (this->connection_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->connection_type);
      *(outbuffer + offset + 0) = (this->info >> (8 * 0)) & 0xFF;
      offset += sizeof(this->info);
      *(outbuffer + offset + 0) = (this->esc_info_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->esc_info_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->esc_info_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->esc_info_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->esc_info_length);
      for( uint32_t i = 0; i < esc_info_length; i++){
      offset += this->esc_info[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->counter =  ((uint16_t) (*(inbuffer + offset)));
      this->counter |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->counter);
      this->count =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->count);
      this->connection_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->connection_type);
      this->info =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->info);
      uint32_t esc_info_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      esc_info_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      esc_info_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      esc_info_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->esc_info_length);
      if(esc_info_lengthT > esc_info_length)
        this->esc_info = (mavros_msgs::ESCInfoItem*)realloc(this->esc_info, esc_info_lengthT * sizeof(mavros_msgs::ESCInfoItem));
      esc_info_length = esc_info_lengthT;
      for( uint32_t i = 0; i < esc_info_length; i++){
      offset += this->st_esc_info.deserialize(inbuffer + offset);
        memcpy( &(this->esc_info[i]), &(this->st_esc_info), sizeof(mavros_msgs::ESCInfoItem));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ESCInfo"; };
    virtual const char * getMD5() override { return "63fa145856ba5b9bdcaf4b1f55eaa549"; };

  };

}
#endif
