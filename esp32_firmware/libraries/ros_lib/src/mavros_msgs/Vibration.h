#ifndef _ROS_mavros_msgs_Vibration_h
#define _ROS_mavros_msgs_Vibration_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Vector3.h"

namespace mavros_msgs
{

  class Vibration : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geometry_msgs::Vector3 _vibration_type;
      _vibration_type vibration;
      float clipping[3];

    Vibration():
      header(),
      vibration(),
      clipping()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->vibration.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 3; i++){
      union {
        float real;
        uint32_t base;
      } u_clippingi;
      u_clippingi.real = this->clipping[i];
      *(outbuffer + offset + 0) = (u_clippingi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_clippingi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_clippingi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_clippingi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->clipping[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->vibration.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 3; i++){
      union {
        float real;
        uint32_t base;
      } u_clippingi;
      u_clippingi.base = 0;
      u_clippingi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_clippingi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_clippingi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_clippingi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->clipping[i] = u_clippingi.real;
      offset += sizeof(this->clipping[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Vibration"; };
    virtual const char * getMD5() override { return "eb92bf9b7aa735dfcd06b3ede5027d02"; };

  };

}
#endif
