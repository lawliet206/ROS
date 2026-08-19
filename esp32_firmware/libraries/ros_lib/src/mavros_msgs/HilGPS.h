#ifndef _ROS_mavros_msgs_HilGPS_h
#define _ROS_mavros_msgs_HilGPS_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geographic_msgs/GeoPoint.h"

namespace mavros_msgs
{

  class HilGPS : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _fix_type_type;
      _fix_type_type fix_type;
      typedef geographic_msgs::GeoPoint _geo_type;
      _geo_type geo;
      typedef uint16_t _eph_type;
      _eph_type eph;
      typedef uint16_t _epv_type;
      _epv_type epv;
      typedef uint16_t _vel_type;
      _vel_type vel;
      typedef int16_t _vn_type;
      _vn_type vn;
      typedef int16_t _ve_type;
      _ve_type ve;
      typedef int16_t _vd_type;
      _vd_type vd;
      typedef uint16_t _cog_type;
      _cog_type cog;
      typedef uint8_t _satellites_visible_type;
      _satellites_visible_type satellites_visible;

    HilGPS():
      header(),
      fix_type(0),
      geo(),
      eph(0),
      epv(0),
      vel(0),
      vn(0),
      ve(0),
      vd(0),
      cog(0),
      satellites_visible(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->fix_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->fix_type);
      offset += this->geo.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->eph >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->eph >> (8 * 1)) & 0xFF;
      offset += sizeof(this->eph);
      *(outbuffer + offset + 0) = (this->epv >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->epv >> (8 * 1)) & 0xFF;
      offset += sizeof(this->epv);
      *(outbuffer + offset + 0) = (this->vel >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vel >> (8 * 1)) & 0xFF;
      offset += sizeof(this->vel);
      union {
        int16_t real;
        uint16_t base;
      } u_vn;
      u_vn.real = this->vn;
      *(outbuffer + offset + 0) = (u_vn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vn.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->vn);
      union {
        int16_t real;
        uint16_t base;
      } u_ve;
      u_ve.real = this->ve;
      *(outbuffer + offset + 0) = (u_ve.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ve.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->ve);
      union {
        int16_t real;
        uint16_t base;
      } u_vd;
      u_vd.real = this->vd;
      *(outbuffer + offset + 0) = (u_vd.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vd.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->vd);
      *(outbuffer + offset + 0) = (this->cog >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cog >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cog);
      *(outbuffer + offset + 0) = (this->satellites_visible >> (8 * 0)) & 0xFF;
      offset += sizeof(this->satellites_visible);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->fix_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->fix_type);
      offset += this->geo.deserialize(inbuffer + offset);
      this->eph =  ((uint16_t) (*(inbuffer + offset)));
      this->eph |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->eph);
      this->epv =  ((uint16_t) (*(inbuffer + offset)));
      this->epv |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->epv);
      this->vel =  ((uint16_t) (*(inbuffer + offset)));
      this->vel |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->vel);
      union {
        int16_t real;
        uint16_t base;
      } u_vn;
      u_vn.base = 0;
      u_vn.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vn.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->vn = u_vn.real;
      offset += sizeof(this->vn);
      union {
        int16_t real;
        uint16_t base;
      } u_ve;
      u_ve.base = 0;
      u_ve.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ve.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ve = u_ve.real;
      offset += sizeof(this->ve);
      union {
        int16_t real;
        uint16_t base;
      } u_vd;
      u_vd.base = 0;
      u_vd.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vd.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->vd = u_vd.real;
      offset += sizeof(this->vd);
      this->cog =  ((uint16_t) (*(inbuffer + offset)));
      this->cog |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cog);
      this->satellites_visible =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->satellites_visible);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HilGPS"; };
    virtual const char * getMD5() override { return "313b3baf2319db196fa18376a4900a7b"; };

  };

}
#endif
