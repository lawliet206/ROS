#ifndef _ROS_mavros_msgs_TerrainReport_h
#define _ROS_mavros_msgs_TerrainReport_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class TerrainReport : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _latitude_type;
      _latitude_type latitude;
      typedef float _longitude_type;
      _longitude_type longitude;
      typedef uint16_t _spacing_type;
      _spacing_type spacing;
      typedef float _terrain_height_type;
      _terrain_height_type terrain_height;
      typedef float _current_height_type;
      _current_height_type current_height;
      typedef uint16_t _pending_type;
      _pending_type pending;
      typedef uint16_t _loaded_type;
      _loaded_type loaded;

    TerrainReport():
      header(),
      latitude(0),
      longitude(0),
      spacing(0),
      terrain_height(0),
      current_height(0),
      pending(0),
      loaded(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += serializeAvrFloat64(outbuffer + offset, this->latitude);
      offset += serializeAvrFloat64(outbuffer + offset, this->longitude);
      *(outbuffer + offset + 0) = (this->spacing >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->spacing >> (8 * 1)) & 0xFF;
      offset += sizeof(this->spacing);
      union {
        float real;
        uint32_t base;
      } u_terrain_height;
      u_terrain_height.real = this->terrain_height;
      *(outbuffer + offset + 0) = (u_terrain_height.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_terrain_height.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_terrain_height.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_terrain_height.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->terrain_height);
      union {
        float real;
        uint32_t base;
      } u_current_height;
      u_current_height.real = this->current_height;
      *(outbuffer + offset + 0) = (u_current_height.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current_height.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_current_height.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_current_height.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->current_height);
      *(outbuffer + offset + 0) = (this->pending >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->pending >> (8 * 1)) & 0xFF;
      offset += sizeof(this->pending);
      *(outbuffer + offset + 0) = (this->loaded >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->loaded >> (8 * 1)) & 0xFF;
      offset += sizeof(this->loaded);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->latitude));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->longitude));
      this->spacing =  ((uint16_t) (*(inbuffer + offset)));
      this->spacing |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->spacing);
      union {
        float real;
        uint32_t base;
      } u_terrain_height;
      u_terrain_height.base = 0;
      u_terrain_height.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_terrain_height.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_terrain_height.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_terrain_height.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->terrain_height = u_terrain_height.real;
      offset += sizeof(this->terrain_height);
      union {
        float real;
        uint32_t base;
      } u_current_height;
      u_current_height.base = 0;
      u_current_height.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current_height.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_current_height.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_current_height.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->current_height = u_current_height.real;
      offset += sizeof(this->current_height);
      this->pending =  ((uint16_t) (*(inbuffer + offset)));
      this->pending |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->pending);
      this->loaded =  ((uint16_t) (*(inbuffer + offset)));
      this->loaded |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->loaded);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/TerrainReport"; };
    virtual const char * getMD5() override { return "f658be3a775aa38d678b427733ae0139"; };

  };

}
#endif
