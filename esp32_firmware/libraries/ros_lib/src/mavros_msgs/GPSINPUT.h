#ifndef _ROS_mavros_msgs_GPSINPUT_h
#define _ROS_mavros_msgs_GPSINPUT_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class GPSINPUT : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _fix_type_type;
      _fix_type_type fix_type;
      typedef uint8_t _gps_id_type;
      _gps_id_type gps_id;
      typedef uint16_t _ignore_flags_type;
      _ignore_flags_type ignore_flags;
      typedef uint32_t _time_week_ms_type;
      _time_week_ms_type time_week_ms;
      typedef uint16_t _time_week_type;
      _time_week_type time_week;
      typedef int32_t _lat_type;
      _lat_type lat;
      typedef int32_t _lon_type;
      _lon_type lon;
      typedef float _alt_type;
      _alt_type alt;
      typedef float _hdop_type;
      _hdop_type hdop;
      typedef float _vdop_type;
      _vdop_type vdop;
      typedef float _vn_type;
      _vn_type vn;
      typedef float _ve_type;
      _ve_type ve;
      typedef float _vd_type;
      _vd_type vd;
      typedef float _speed_accuracy_type;
      _speed_accuracy_type speed_accuracy;
      typedef float _horiz_accuracy_type;
      _horiz_accuracy_type horiz_accuracy;
      typedef float _vert_accuracy_type;
      _vert_accuracy_type vert_accuracy;
      typedef uint8_t _satellites_visible_type;
      _satellites_visible_type satellites_visible;
      typedef uint16_t _yaw_type;
      _yaw_type yaw;
      enum { GPS_FIX_TYPE_NO_GPS =  0     };
      enum { GPS_FIX_TYPE_NO_FIX =  1     };
      enum { GPS_FIX_TYPE_2D_FIX =  2     };
      enum { GPS_FIX_TYPE_3D_FIX =  3     };
      enum { GPS_FIX_TYPE_DGPS =  4     };
      enum { GPS_FIX_TYPE_RTK_FLOATR =  5     };
      enum { GPS_FIX_TYPE_RTK_FIXEDR =  6     };
      enum { GPS_FIX_TYPE_STATIC =  7     };
      enum { GPS_FIX_TYPE_PPP =  8     };

    GPSINPUT():
      header(),
      fix_type(0),
      gps_id(0),
      ignore_flags(0),
      time_week_ms(0),
      time_week(0),
      lat(0),
      lon(0),
      alt(0),
      hdop(0),
      vdop(0),
      vn(0),
      ve(0),
      vd(0),
      speed_accuracy(0),
      horiz_accuracy(0),
      vert_accuracy(0),
      satellites_visible(0),
      yaw(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->fix_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->fix_type);
      *(outbuffer + offset + 0) = (this->gps_id >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gps_id);
      *(outbuffer + offset + 0) = (this->ignore_flags >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ignore_flags >> (8 * 1)) & 0xFF;
      offset += sizeof(this->ignore_flags);
      *(outbuffer + offset + 0) = (this->time_week_ms >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_week_ms >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_week_ms >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_week_ms >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_week_ms);
      *(outbuffer + offset + 0) = (this->time_week >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_week >> (8 * 1)) & 0xFF;
      offset += sizeof(this->time_week);
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
        float real;
        uint32_t base;
      } u_alt;
      u_alt.real = this->alt;
      *(outbuffer + offset + 0) = (u_alt.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_alt.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_alt.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_alt.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->alt);
      union {
        float real;
        uint32_t base;
      } u_hdop;
      u_hdop.real = this->hdop;
      *(outbuffer + offset + 0) = (u_hdop.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_hdop.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_hdop.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_hdop.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->hdop);
      union {
        float real;
        uint32_t base;
      } u_vdop;
      u_vdop.real = this->vdop;
      *(outbuffer + offset + 0) = (u_vdop.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vdop.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vdop.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vdop.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vdop);
      union {
        float real;
        uint32_t base;
      } u_vn;
      u_vn.real = this->vn;
      *(outbuffer + offset + 0) = (u_vn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vn.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vn.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vn.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vn);
      union {
        float real;
        uint32_t base;
      } u_ve;
      u_ve.real = this->ve;
      *(outbuffer + offset + 0) = (u_ve.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ve.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ve.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ve.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ve);
      union {
        float real;
        uint32_t base;
      } u_vd;
      u_vd.real = this->vd;
      *(outbuffer + offset + 0) = (u_vd.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vd.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vd.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vd.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vd);
      union {
        float real;
        uint32_t base;
      } u_speed_accuracy;
      u_speed_accuracy.real = this->speed_accuracy;
      *(outbuffer + offset + 0) = (u_speed_accuracy.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_speed_accuracy.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_speed_accuracy.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_speed_accuracy.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->speed_accuracy);
      union {
        float real;
        uint32_t base;
      } u_horiz_accuracy;
      u_horiz_accuracy.real = this->horiz_accuracy;
      *(outbuffer + offset + 0) = (u_horiz_accuracy.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_horiz_accuracy.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_horiz_accuracy.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_horiz_accuracy.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->horiz_accuracy);
      union {
        float real;
        uint32_t base;
      } u_vert_accuracy;
      u_vert_accuracy.real = this->vert_accuracy;
      *(outbuffer + offset + 0) = (u_vert_accuracy.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vert_accuracy.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vert_accuracy.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vert_accuracy.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vert_accuracy);
      *(outbuffer + offset + 0) = (this->satellites_visible >> (8 * 0)) & 0xFF;
      offset += sizeof(this->satellites_visible);
      *(outbuffer + offset + 0) = (this->yaw >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->yaw >> (8 * 1)) & 0xFF;
      offset += sizeof(this->yaw);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->fix_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->fix_type);
      this->gps_id =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->gps_id);
      this->ignore_flags =  ((uint16_t) (*(inbuffer + offset)));
      this->ignore_flags |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->ignore_flags);
      this->time_week_ms =  ((uint32_t) (*(inbuffer + offset)));
      this->time_week_ms |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_week_ms |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_week_ms |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_week_ms);
      this->time_week =  ((uint16_t) (*(inbuffer + offset)));
      this->time_week |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->time_week);
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
        float real;
        uint32_t base;
      } u_alt;
      u_alt.base = 0;
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_alt.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->alt = u_alt.real;
      offset += sizeof(this->alt);
      union {
        float real;
        uint32_t base;
      } u_hdop;
      u_hdop.base = 0;
      u_hdop.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_hdop.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_hdop.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_hdop.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->hdop = u_hdop.real;
      offset += sizeof(this->hdop);
      union {
        float real;
        uint32_t base;
      } u_vdop;
      u_vdop.base = 0;
      u_vdop.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vdop.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vdop.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vdop.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vdop = u_vdop.real;
      offset += sizeof(this->vdop);
      union {
        float real;
        uint32_t base;
      } u_vn;
      u_vn.base = 0;
      u_vn.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vn.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vn.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vn.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vn = u_vn.real;
      offset += sizeof(this->vn);
      union {
        float real;
        uint32_t base;
      } u_ve;
      u_ve.base = 0;
      u_ve.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ve.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ve.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ve.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ve = u_ve.real;
      offset += sizeof(this->ve);
      union {
        float real;
        uint32_t base;
      } u_vd;
      u_vd.base = 0;
      u_vd.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vd.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vd.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vd.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vd = u_vd.real;
      offset += sizeof(this->vd);
      union {
        float real;
        uint32_t base;
      } u_speed_accuracy;
      u_speed_accuracy.base = 0;
      u_speed_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_speed_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_speed_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_speed_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->speed_accuracy = u_speed_accuracy.real;
      offset += sizeof(this->speed_accuracy);
      union {
        float real;
        uint32_t base;
      } u_horiz_accuracy;
      u_horiz_accuracy.base = 0;
      u_horiz_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_horiz_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_horiz_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_horiz_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->horiz_accuracy = u_horiz_accuracy.real;
      offset += sizeof(this->horiz_accuracy);
      union {
        float real;
        uint32_t base;
      } u_vert_accuracy;
      u_vert_accuracy.base = 0;
      u_vert_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vert_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vert_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vert_accuracy.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vert_accuracy = u_vert_accuracy.real;
      offset += sizeof(this->vert_accuracy);
      this->satellites_visible =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->satellites_visible);
      this->yaw =  ((uint16_t) (*(inbuffer + offset)));
      this->yaw |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->yaw);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/GPSINPUT"; };
    virtual const char * getMD5() override { return "928ef4ffec7b9af7c6e4748f0542b6a0"; };

  };

}
#endif
