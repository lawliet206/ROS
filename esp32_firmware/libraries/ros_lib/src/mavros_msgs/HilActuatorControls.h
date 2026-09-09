#ifndef _ROS_mavros_msgs_HilActuatorControls_h
#define _ROS_mavros_msgs_HilActuatorControls_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class HilActuatorControls : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      float controls[16];
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef uint64_t _flags_type;
      _flags_type flags;

    HilActuatorControls():
      header(),
      controls(),
      mode(0),
      flags(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 16; i++){
      union {
        float real;
        uint32_t base;
      } u_controlsi;
      u_controlsi.real = this->controls[i];
      *(outbuffer + offset + 0) = (u_controlsi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_controlsi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_controlsi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_controlsi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->controls[i]);
      }
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      *(outbuffer + offset + 0) = (this->flags >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->flags >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->flags >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->flags >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->flags >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->flags >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->flags >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->flags >> (8 * 7)) & 0xFF;
      offset += sizeof(this->flags);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 16; i++){
      union {
        float real;
        uint32_t base;
      } u_controlsi;
      u_controlsi.base = 0;
      u_controlsi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_controlsi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_controlsi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_controlsi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->controls[i] = u_controlsi.real;
      offset += sizeof(this->controls[i]);
      }
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      this->flags =  ((uint64_t) (*(inbuffer + offset)));
      this->flags |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->flags |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->flags);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HilActuatorControls"; };
    virtual const char * getMD5() override { return "18482e8ef0330ac2fc9a0421be1d11c3"; };

  };

}
#endif
