#ifndef _ROS_mavros_msgs_GPSRAW_h
#define _ROS_mavros_msgs_GPSRAW_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class GPSRAW : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _fix_type_type;
      _fix_type_type fix_type;
      typedef int32_t _lat_type;
      _lat_type lat;
      typedef int32_t _lon_type;
      _lon_type lon;
      typedef int32_t _alt_type;
      _alt_type alt;
      typedef uint16_t _eph_type;
      _eph_type eph;
      typedef uint16_t _epv_type;
      _epv_type epv;
      typedef uint16_t _vel_type;
      _vel_type vel;
      typedef uint16_t _cog_type;
      _cog_type cog;
      typedef uint8_t _satellites_visible_type;
      _satellites_visible_type satellites_visible;
      typedef int32_t _alt_ellipsoid_type;
      _alt_ellipsoid_type alt_ellipsoid;
      typedef uint32_t _h_acc_type;
      _h_acc_type h_acc;
      typedef uint32_t _v_acc_type;
      _v_acc_type v_acc;
      typedef uint32_t _vel_acc_type;
      _vel_acc_type vel_acc;
      typedef int32_t _hdg_acc_type;
      _hdg_acc_type hdg_acc;
      typedef uint16_t _yaw_type;
      _yaw_type yaw;
      typedef uint8_t _dgps_numch_type;
      _dgps_numch_type dgps_numch;
      typedef uint32_t _dgps_age_type;
      _dgps_age_type dgps_age;
      enum { GPS_FIX_TYPE_NO_GPS =  0     };
      enum { GPS_FIX_TYPE_NO_FIX =  1     };
      enum { GPS_FIX_TYPE_2D_FIX =  2     };
      enum { GPS_FIX_TYPE_3D_FIX =  3     };
      enum { GPS_FIX_TYPE_DGPS =  4     };
      enum { GPS_FIX_TYPE_RTK_FLOATR =  5     };
      enum { GPS_FIX_TYPE_RTK_FIXEDR =  6     };
      enum { GPS_FIX_TYPE_STATIC =  7     };
      enum { GPS_FIX_TYPE_PPP =  8     };

    GPSRAW():
      header(),
      fix_type(0),
      lat(0),
      lon(0),
      alt(0),
      eph(0),
      epv(0),
      vel(0),
      cog(0),
      satellites_visible(0),
      alt_ellipsoid(0),
      h_acc(0),
      v_acc(0),
      vel_acc(0),
      hdg_acc(0),
      yaw(0),
      dgps_numch(0),
      dgps_age(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->fix_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->fix_type);
      union {
        int32_t real;
        uint32_t base;
      } u_lat;
      u_lat.real = this->lat;
      *(outbuffer + offset + 0) = (u_lat.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lat.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lat.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lat.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lat);
      union {
        int32_t real;
        uint32_t base;
      } u_lon;
      u_lon.real = this->lon;
      *(outbuffer + offset + 0) = (u_lon.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lon.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lon.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lon.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lon);
      union {
        int32_t real;
        uint32_t base;
      } u_alt;
      u_alt.real = this->alt;
      *(outbuffer + offset + 0) = (u_alt.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_alt.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_alt.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_alt.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->alt);
      *(outbuffer + offset + 0) = (this->eph >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->eph >> (8 * 1)) & 0xFF;
      offset += sizeof(this->eph);
      *(outbuffer + offset + 0) = (this->epv >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->epv >> (8 * 1)) & 0xFF;
      offset += sizeof(this->epv);
      *(outbuffer + offset + 0) = (this->vel >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vel >> (8 * 1)) & 0xFF;
      offset += sizeof(this->vel);
      *(outbuffer + offset + 0) = (this->cog >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cog >> (8 * 1)) & 0xFF;
      offset += sizeof(this->cog);
      *(outbuffer + offset + 0) = (this->satellites_visible >> (8 * 0)) & 0xFF;
      offset += sizeof(this->satellites_visible);
      union {
        int32_t real;
        uint32_t base;
      } u_alt_ellipsoid;
      u_alt_ellipsoid.real = this->alt_ellipsoid;
      *(outbuffer + offset + 0) = (u_alt_ellipsoid.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_alt_ellipsoid.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_alt_ellipsoid.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_alt_ellipsoid.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->alt_ellipsoid);
      *(outbuffer + offset + 0) = (this->h_acc >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->h_acc >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->h_acc >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->h_acc >> (8 * 3)) & 0xFF;
      offset += sizeof(this->h_acc);
      *(outbuffer + offset + 0) = (this->v_acc >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->v_acc >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->v_acc >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->v_acc >> (8 * 3)) & 0xFF;
      offset += sizeof(this->v_acc);
      *(outbuffer + offset + 0) = (this->vel_acc >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vel_acc >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->vel_acc >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->vel_acc >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vel_acc);
      union {
        int32_t real;
        uint32_t base;
      } u_hdg_acc;
      u_hdg_acc.real = this->hdg_acc;
      *(outbuffer + offset + 0) = (u_hdg_acc.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_hdg_acc.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_hdg_acc.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_hdg_acc.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->hdg_acc);
      *(outbuffer + offset + 0) = (this->yaw >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->yaw >> (8 * 1)) & 0xFF;
      offset += sizeof(this->yaw);
      *(outbuffer + offset + 0) = (this->dgps_numch >> (8 * 0)) & 0xFF;
      offset += sizeof(this->dgps_numch);
      *(outbuffer + offset + 0) = (this->dgps_age >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->dgps_age >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->dgps_age >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->dgps_age >> (8 * 3)) & 0xFF;
      offset += sizeof(this->dgps_age);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->fix_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->fix_type);
      union {
        int32_t real;
        uint32_t base;
      } u_lat;
      u_lat.base = 0;
      u_lat.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lat.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lat.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lat.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lat = u_lat.real;
      offset += sizeof(this->lat);
      union {
        int32_t real;
        uint32_t base;
      } u_lon;
      u_lon.base = 0;
      u_lon.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lon.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lon.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lon.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lon = u_lon.real;
      offset += sizeof(this->lon);
      union {
        int32_t real;
        uint32_t base;
      } u_alt;
      u_alt.base = 0;
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->alt = u_alt.real;
      offset += sizeof(this->alt);
      this->eph =  ((uint16_t) (*(inbuffer + offset)));
      this->eph |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->eph);
      this->epv =  ((uint16_t) (*(inbuffer + offset)));
      this->epv |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->epv);
      this->vel =  ((uint16_t) (*(inbuffer + offset)));
      this->vel |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->vel);
      this->cog =  ((uint16_t) (*(inbuffer + offset)));
      this->cog |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->cog);
      this->satellites_visible =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->satellites_visible);
      union {
        int32_t real;
        uint32_t base;
      } u_alt_ellipsoid;
      u_alt_ellipsoid.base = 0;
      u_alt_ellipsoid.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_alt_ellipsoid.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_alt_ellipsoid.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_alt_ellipsoid.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->alt_ellipsoid = u_alt_ellipsoid.real;
      offset += sizeof(this->alt_ellipsoid);
      this->h_acc =  ((uint32_t) (*(inbuffer + offset)));
      this->h_acc |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->h_acc |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->h_acc |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->h_acc);
      this->v_acc =  ((uint32_t) (*(inbuffer + offset)));
      this->v_acc |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->v_acc |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->v_acc |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->v_acc);
      this->vel_acc =  ((uint32_t) (*(inbuffer + offset)));
      this->vel_acc |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->vel_acc |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->vel_acc |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->vel_acc);
      union {
        int32_t real;
        uint32_t base;
      } u_hdg_acc;
      u_hdg_acc.base = 0;
      u_hdg_acc.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_hdg_acc.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_hdg_acc.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_hdg_acc.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->hdg_acc = u_hdg_acc.real;
      offset += sizeof(this->hdg_acc);
      this->yaw =  ((uint16_t) (*(inbuffer + offset)));
      this->yaw |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->yaw);
      this->dgps_numch =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->dgps_numch);
      this->dgps_age =  ((uint32_t) (*(inbuffer + offset)));
      this->dgps_age |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->dgps_age |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->dgps_age |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->dgps_age);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/GPSRAW"; };
    virtual const char * getMD5() override { return "58a85dbc1516a2d4302f256cca54bbbf"; };

  };

}
#endif
