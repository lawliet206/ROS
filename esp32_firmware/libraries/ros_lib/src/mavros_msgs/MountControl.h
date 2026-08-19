#ifndef _ROS_mavros_msgs_MountControl_h
#define _ROS_mavros_msgs_MountControl_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class MountControl : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _mode_type;
      _mode_type mode;
      typedef float _pitch_type;
      _pitch_type pitch;
      typedef float _roll_type;
      _roll_type roll;
      typedef float _yaw_type;
      _yaw_type yaw;
      typedef float _altitude_type;
      _altitude_type altitude;
      typedef float _latitude_type;
      _latitude_type latitude;
      typedef float _longitude_type;
      _longitude_type longitude;
      enum { MAV_MOUNT_MODE_RETRACT =  0 };
      enum { MAV_MOUNT_MODE_NEUTRAL =  1 };
      enum { MAV_MOUNT_MODE_MAVLINK_TARGETING =  2 };
      enum { MAV_MOUNT_MODE_RC_TARGETING =  3 };
      enum { MAV_MOUNT_MODE_GPS_POINT =  4 };

    MountControl():
      header(),
      mode(0),
      pitch(0),
      roll(0),
      yaw(0),
      altitude(0),
      latitude(0),
      longitude(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mode);
      union {
        float real;
        uint32_t base;
      } u_pitch;
      u_pitch.real = this->pitch;
      *(outbuffer + offset + 0) = (u_pitch.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pitch.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_pitch.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_pitch.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pitch);
      union {
        float real;
        uint32_t base;
      } u_roll;
      u_roll.real = this->roll;
      *(outbuffer + offset + 0) = (u_roll.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_roll.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_roll.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_roll.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->roll);
      union {
        float real;
        uint32_t base;
      } u_yaw;
      u_yaw.real = this->yaw;
      *(outbuffer + offset + 0) = (u_yaw.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_yaw.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_yaw.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_yaw.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->yaw);
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
      } u_latitude;
      u_latitude.real = this->latitude;
      *(outbuffer + offset + 0) = (u_latitude.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_latitude.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_latitude.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_latitude.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->latitude);
      union {
        float real;
        uint32_t base;
      } u_longitude;
      u_longitude.real = this->longitude;
      *(outbuffer + offset + 0) = (u_longitude.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_longitude.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_longitude.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_longitude.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->longitude);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mode);
      union {
        float real;
        uint32_t base;
      } u_pitch;
      u_pitch.base = 0;
      u_pitch.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pitch.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_pitch.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_pitch.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->pitch = u_pitch.real;
      offset += sizeof(this->pitch);
      union {
        float real;
        uint32_t base;
      } u_roll;
      u_roll.base = 0;
      u_roll.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_roll.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_roll.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_roll.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->roll = u_roll.real;
      offset += sizeof(this->roll);
      union {
        float real;
        uint32_t base;
      } u_yaw;
      u_yaw.base = 0;
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_yaw.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->yaw = u_yaw.real;
      offset += sizeof(this->yaw);
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
      } u_latitude;
      u_latitude.base = 0;
      u_latitude.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_latitude.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_latitude.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_latitude.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->latitude = u_latitude.real;
      offset += sizeof(this->latitude);
      union {
        float real;
        uint32_t base;
      } u_longitude;
      u_longitude.base = 0;
      u_longitude.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_longitude.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_longitude.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_longitude.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->longitude = u_longitude.real;
      offset += sizeof(this->longitude);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/MountControl"; };
    virtual const char * getMD5() override { return "214cf13a68b4fed9e2a77b1b436f144e"; };

  };

}
#endif
