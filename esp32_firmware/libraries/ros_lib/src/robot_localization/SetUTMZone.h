#ifndef _ROS_SERVICE_SetUTMZone_h
#define _ROS_SERVICE_SetUTMZone_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace robot_localization
{

static const char SETUTMZONE[] = "robot_localization/SetUTMZone";

  class SetUTMZoneRequest : public ros::Msg
  {
    public:
      typedef const char* _utm_zone_type;
      _utm_zone_type utm_zone;

    SetUTMZoneRequest():
      utm_zone("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_utm_zone = strlen(this->utm_zone);
      varToArr(outbuffer + offset, length_utm_zone);
      offset += 4;
      memcpy(outbuffer + offset, this->utm_zone, length_utm_zone);
      offset += length_utm_zone;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_utm_zone;
      arrToVar(length_utm_zone, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_utm_zone; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_utm_zone-1]=0;
      this->utm_zone = (char *)(inbuffer + offset-1);
      offset += length_utm_zone;
     return offset;
    }

    virtual const char * getType() override { return SETUTMZONE; };
    virtual const char * getMD5() override { return "893fd74d45efde020666acda18d3cccc"; };

  };

  class SetUTMZoneResponse : public ros::Msg
  {
    public:

    SetUTMZoneResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return SETUTMZONE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetUTMZone {
    public:
    typedef SetUTMZoneRequest Request;
    typedef SetUTMZoneResponse Response;
  };

}
#endif
