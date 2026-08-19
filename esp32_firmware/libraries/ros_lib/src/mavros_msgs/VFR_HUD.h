#ifndef _ROS_mavros_msgs_VFR_HUD_h
#define _ROS_mavros_msgs_VFR_HUD_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class VFR_HUD : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _airspeed_type;
      _airspeed_type airspeed;
      typedef float _groundspeed_type;
      _groundspeed_type groundspeed;
      typedef int16_t _heading_type;
      _heading_type heading;
      typedef float _throttle_type;
      _throttle_type throttle;
      typedef float _altitude_type;
      _altitude_type altitude;
      typedef float _climb_type;
      _climb_type climb;

    VFR_HUD():
      header(),
      airspeed(0),
      groundspeed(0),
      heading(0),
      throttle(0),
      altitude(0),
      climb(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_airspeed;
      u_airspeed.real = this->airspeed;
      *(outbuffer + offset + 0) = (u_airspeed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_airspeed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_airspeed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_airspeed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->airspeed);
      union {
        float real;
        uint32_t base;
      } u_groundspeed;
      u_groundspeed.real = this->groundspeed;
      *(outbuffer + offset + 0) = (u_groundspeed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_groundspeed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_groundspeed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_groundspeed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->groundspeed);
      union {
        int16_t real;
        uint16_t base;
      } u_heading;
      u_heading.real = this->heading;
      *(outbuffer + offset + 0) = (u_heading.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_heading.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->heading);
      union {
        float real;
        uint32_t base;
      } u_throttle;
      u_throttle.real = this->throttle;
      *(outbuffer + offset + 0) = (u_throttle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_throttle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_throttle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_throttle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->throttle);
      union {
        float real;
        uint32_t base;
      } u_altitude;
      u_altitude.real = this->altitude;
      *(outbuffer + offset + 0) = (u_altitude.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_altitude.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_altitude.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_altitude.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->altitude);
      union {
        float real;
        uint32_t base;
      } u_climb;
      u_climb.real = this->climb;
      *(outbuffer + offset + 0) = (u_climb.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_climb.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_climb.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_climb.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->climb);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_airspeed;
      u_airspeed.base = 0;
      u_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->airspeed = u_airspeed.real;
      offset += sizeof(this->airspeed);
      union {
        float real;
        uint32_t base;
      } u_groundspeed;
      u_groundspeed.base = 0;
      u_groundspeed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_groundspeed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_groundspeed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_groundspeed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->groundspeed = u_groundspeed.real;
      offset += sizeof(this->groundspeed);
      union {
        int16_t real;
        uint16_t base;
      } u_heading;
      u_heading.base = 0;
      u_heading.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_heading.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->heading = u_heading.real;
      offset += sizeof(this->heading);
      union {
        float real;
        uint32_t base;
      } u_throttle;
      u_throttle.base = 0;
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_throttle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->throttle = u_throttle.real;
      offset += sizeof(this->throttle);
      union {
        float real;
        uint32_t base;
      } u_altitude;
      u_altitude.base = 0;
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->altitude = u_altitude.real;
      offset += sizeof(this->altitude);
      union {
        float real;
        uint32_t base;
      } u_climb;
      u_climb.base = 0;
      u_climb.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_climb.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_climb.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_climb.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->climb = u_climb.real;
      offset += sizeof(this->climb);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/VFR_HUD"; };
    virtual const char * getMD5() override { return "1f55e210c3d39fe105d44d8dc963655f"; };

  };

}
#endif
