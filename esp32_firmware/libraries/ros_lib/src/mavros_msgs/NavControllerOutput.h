#ifndef _ROS_mavros_msgs_NavControllerOutput_h
#define _ROS_mavros_msgs_NavControllerOutput_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class NavControllerOutput : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _nav_roll_type;
      _nav_roll_type nav_roll;
      typedef float _nav_pitch_type;
      _nav_pitch_type nav_pitch;
      typedef int16_t _nav_bearing_type;
      _nav_bearing_type nav_bearing;
      typedef int16_t _target_bearing_type;
      _target_bearing_type target_bearing;
      typedef uint16_t _wp_dist_type;
      _wp_dist_type wp_dist;
      typedef float _alt_error_type;
      _alt_error_type alt_error;
      typedef float _aspd_error_type;
      _aspd_error_type aspd_error;
      typedef float _xtrack_error_type;
      _xtrack_error_type xtrack_error;

    NavControllerOutput():
      header(),
      nav_roll(0),
      nav_pitch(0),
      nav_bearing(0),
      target_bearing(0),
      wp_dist(0),
      alt_error(0),
      aspd_error(0),
      xtrack_error(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_nav_roll;
      u_nav_roll.real = this->nav_roll;
      *(outbuffer + offset + 0) = (u_nav_roll.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_nav_roll.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_nav_roll.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_nav_roll.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->nav_roll);
      union {
        float real;
        uint32_t base;
      } u_nav_pitch;
      u_nav_pitch.real = this->nav_pitch;
      *(outbuffer + offset + 0) = (u_nav_pitch.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_nav_pitch.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_nav_pitch.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_nav_pitch.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->nav_pitch);
      union {
        int16_t real;
        uint16_t base;
      } u_nav_bearing;
      u_nav_bearing.real = this->nav_bearing;
      *(outbuffer + offset + 0) = (u_nav_bearing.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_nav_bearing.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->nav_bearing);
      union {
        int16_t real;
        uint16_t base;
      } u_target_bearing;
      u_target_bearing.real = this->target_bearing;
      *(outbuffer + offset + 0) = (u_target_bearing.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_target_bearing.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->target_bearing);
      *(outbuffer + offset + 0) = (this->wp_dist >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wp_dist >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wp_dist);
      union {
        float real;
        uint32_t base;
      } u_alt_error;
      u_alt_error.real = this->alt_error;
      *(outbuffer + offset + 0) = (u_alt_error.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_alt_error.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_alt_error.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_alt_error.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->alt_error);
      union {
        float real;
        uint32_t base;
      } u_aspd_error;
      u_aspd_error.real = this->aspd_error;
      *(outbuffer + offset + 0) = (u_aspd_error.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_aspd_error.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_aspd_error.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_aspd_error.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->aspd_error);
      union {
        float real;
        uint32_t base;
      } u_xtrack_error;
      u_xtrack_error.real = this->xtrack_error;
      *(outbuffer + offset + 0) = (u_xtrack_error.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_xtrack_error.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_xtrack_error.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_xtrack_error.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->xtrack_error);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_nav_roll;
      u_nav_roll.base = 0;
      u_nav_roll.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_nav_roll.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_nav_roll.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_nav_roll.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->nav_roll = u_nav_roll.real;
      offset += sizeof(this->nav_roll);
      union {
        float real;
        uint32_t base;
      } u_nav_pitch;
      u_nav_pitch.base = 0;
      u_nav_pitch.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_nav_pitch.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_nav_pitch.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_nav_pitch.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->nav_pitch = u_nav_pitch.real;
      offset += sizeof(this->nav_pitch);
      union {
        int16_t real;
        uint16_t base;
      } u_nav_bearing;
      u_nav_bearing.base = 0;
      u_nav_bearing.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_nav_bearing.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->nav_bearing = u_nav_bearing.real;
      offset += sizeof(this->nav_bearing);
      union {
        int16_t real;
        uint16_t base;
      } u_target_bearing;
      u_target_bearing.base = 0;
      u_target_bearing.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_target_bearing.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->target_bearing = u_target_bearing.real;
      offset += sizeof(this->target_bearing);
      this->wp_dist =  ((uint16_t) (*(inbuffer + offset)));
      this->wp_dist |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->wp_dist);
      union {
        float real;
        uint32_t base;
      } u_alt_error;
      u_alt_error.base = 0;
      u_alt_error.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_alt_error.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_alt_error.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_alt_error.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->alt_error = u_alt_error.real;
      offset += sizeof(this->alt_error);
      union {
        float real;
        uint32_t base;
      } u_aspd_error;
      u_aspd_error.base = 0;
      u_aspd_error.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_aspd_error.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_aspd_error.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_aspd_error.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->aspd_error = u_aspd_error.real;
      offset += sizeof(this->aspd_error);
      union {
        float real;
        uint32_t base;
      } u_xtrack_error;
      u_xtrack_error.base = 0;
      u_xtrack_error.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_xtrack_error.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_xtrack_error.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_xtrack_error.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->xtrack_error = u_xtrack_error.real;
      offset += sizeof(this->xtrack_error);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/NavControllerOutput"; };
    virtual const char * getMD5() override { return "f6340c9bb79e3ac2a6142ce592e66756"; };

  };

}
#endif
