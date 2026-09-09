#ifndef _ROS_mavros_msgs_Tunnel_h
#define _ROS_mavros_msgs_Tunnel_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class Tunnel : public ros::Msg
  {
    public:
      typedef uint8_t _target_system_type;
      _target_system_type target_system;
      typedef uint8_t _target_component_type;
      _target_component_type target_component;
      typedef uint16_t _payload_type_type;
      _payload_type_type payload_type;
      typedef uint8_t _payload_length_type;
      _payload_length_type payload_length;
      uint8_t payload[128];
      enum { PAYLOAD_TYPE_UNKNOWN =  0           };
      enum { PAYLOAD_TYPE_STORM32_RESERVED0 =  200  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED1 =  201  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED2 =  202  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED3 =  203  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED4 =  204  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED5 =  205  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED6 =  206  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED7 =  207  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED8 =  208  };
      enum { PAYLOAD_TYPE_STORM32_RESERVED9 =  209  };

    Tunnel():
      target_system(0),
      target_component(0),
      payload_type(0),
      payload_length(0),
      payload()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->target_system >> (8 * 0)) & 0xFF;
      offset += sizeof(this->target_system);
      *(outbuffer + offset + 0) = (this->target_component >> (8 * 0)) & 0xFF;
      offset += sizeof(this->target_component);
      *(outbuffer + offset + 0) = (this->payload_type >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->payload_type >> (8 * 1)) & 0xFF;
      offset += sizeof(this->payload_type);
      *(outbuffer + offset + 0) = (this->payload_length >> (8 * 0)) & 0xFF;
      offset += sizeof(this->payload_length);
      for( uint32_t i = 0; i < 128; i++){
      *(outbuffer + offset + 0) = (this->payload[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->payload[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->target_system =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->target_system);
      this->target_component =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->target_component);
      this->payload_type =  ((uint16_t) (*(inbuffer + offset)));
      this->payload_type |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->payload_type);
      this->payload_length =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->payload_length);
      for( uint32_t i = 0; i < 128; i++){
      this->payload[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->payload[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Tunnel"; };
    virtual const char * getMD5() override { return "6d8c215067d3b319bbb219c37c1ebc5d"; };

  };

}
#endif
