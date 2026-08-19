#ifndef _ROS_mavros_msgs_RTKBaseline_h
#define _ROS_mavros_msgs_RTKBaseline_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class RTKBaseline : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint32_t _time_last_baseline_ms_type;
      _time_last_baseline_ms_type time_last_baseline_ms;
      typedef uint8_t _rtk_receiver_id_type;
      _rtk_receiver_id_type rtk_receiver_id;
      typedef uint16_t _wn_type;
      _wn_type wn;
      typedef uint32_t _tow_type;
      _tow_type tow;
      typedef uint8_t _rtk_health_type;
      _rtk_health_type rtk_health;
      typedef uint8_t _rtk_rate_type;
      _rtk_rate_type rtk_rate;
      typedef uint8_t _nsats_type;
      _nsats_type nsats;
      typedef uint8_t _baseline_coords_type_type;
      _baseline_coords_type_type baseline_coords_type;
      typedef int32_t _baseline_a_mm_type;
      _baseline_a_mm_type baseline_a_mm;
      typedef int32_t _baseline_b_mm_type;
      _baseline_b_mm_type baseline_b_mm;
      typedef int32_t _baseline_c_mm_type;
      _baseline_c_mm_type baseline_c_mm;
      typedef uint32_t _accuracy_type;
      _accuracy_type accuracy;
      typedef int32_t _iar_num_hypotheses_type;
      _iar_num_hypotheses_type iar_num_hypotheses;
      enum { RTK_BASELINE_COORDINATE_SYSTEM_ECEF =  0    };
      enum { RTK_BASELINE_COORDINATE_SYSTEM_NED =  1     };

    RTKBaseline():
      header(),
      time_last_baseline_ms(0),
      rtk_receiver_id(0),
      wn(0),
      tow(0),
      rtk_health(0),
      rtk_rate(0),
      nsats(0),
      baseline_coords_type(0),
      baseline_a_mm(0),
      baseline_b_mm(0),
      baseline_c_mm(0),
      accuracy(0),
      iar_num_hypotheses(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->time_last_baseline_ms >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_last_baseline_ms >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_last_baseline_ms >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_last_baseline_ms >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_last_baseline_ms);
      *(outbuffer + offset + 0) = (this->rtk_receiver_id >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rtk_receiver_id);
      *(outbuffer + offset + 0) = (this->wn >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wn >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wn);
      *(outbuffer + offset + 0) = (this->tow >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->tow >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->tow >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->tow >> (8 * 3)) & 0xFF;
      offset += sizeof(this->tow);
      *(outbuffer + offset + 0) = (this->rtk_health >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rtk_health);
      *(outbuffer + offset + 0) = (this->rtk_rate >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rtk_rate);
      *(outbuffer + offset + 0) = (this->nsats >> (8 * 0)) & 0xFF;
      offset += sizeof(this->nsats);
      *(outbuffer + offset + 0) = (this->baseline_coords_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->baseline_coords_type);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_a_mm;
      u_baseline_a_mm.real = this->baseline_a_mm;
      *(outbuffer + offset + 0) = (u_baseline_a_mm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_a_mm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_a_mm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_a_mm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_a_mm);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_b_mm;
      u_baseline_b_mm.real = this->baseline_b_mm;
      *(outbuffer + offset + 0) = (u_baseline_b_mm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_b_mm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_b_mm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_b_mm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_b_mm);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_c_mm;
      u_baseline_c_mm.real = this->baseline_c_mm;
      *(outbuffer + offset + 0) = (u_baseline_c_mm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_c_mm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_c_mm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_c_mm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_c_mm);
      *(outbuffer + offset + 0) = (this->accuracy >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->accuracy >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->accuracy >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->accuracy >> (8 * 3)) & 0xFF;
      offset += sizeof(this->accuracy);
      union {
        int32_t real;
        uint32_t base;
      } u_iar_num_hypotheses;
      u_iar_num_hypotheses.real = this->iar_num_hypotheses;
      *(outbuffer + offset + 0) = (u_iar_num_hypotheses.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_iar_num_hypotheses.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_iar_num_hypotheses.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_iar_num_hypotheses.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->iar_num_hypotheses);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->time_last_baseline_ms =  ((uint32_t) (*(inbuffer + offset)));
      this->time_last_baseline_ms |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_last_baseline_ms |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_last_baseline_ms |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_last_baseline_ms);
      this->rtk_receiver_id =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rtk_receiver_id);
      this->wn =  ((uint16_t) (*(inbuffer + offset)));
      this->wn |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->wn);
      this->tow =  ((uint32_t) (*(inbuffer + offset)));
      this->tow |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->tow |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->tow |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->tow);
      this->rtk_health =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rtk_health);
      this->rtk_rate =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rtk_rate);
      this->nsats =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->nsats);
      this->baseline_coords_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->baseline_coords_type);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_a_mm;
      u_baseline_a_mm.base = 0;
      u_baseline_a_mm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_a_mm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_a_mm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_a_mm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_a_mm = u_baseline_a_mm.real;
      offset += sizeof(this->baseline_a_mm);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_b_mm;
      u_baseline_b_mm.base = 0;
      u_baseline_b_mm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_b_mm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_b_mm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_b_mm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_b_mm = u_baseline_b_mm.real;
      offset += sizeof(this->baseline_b_mm);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_c_mm;
      u_baseline_c_mm.base = 0;
      u_baseline_c_mm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_c_mm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_c_mm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_c_mm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_c_mm = u_baseline_c_mm.real;
      offset += sizeof(this->baseline_c_mm);
      this->accuracy =  ((uint32_t) (*(inbuffer + offset)));
      this->accuracy |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->accuracy |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->accuracy |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->accuracy);
      union {
        int32_t real;
        uint32_t base;
      } u_iar_num_hypotheses;
      u_iar_num_hypotheses.base = 0;
      u_iar_num_hypotheses.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_iar_num_hypotheses.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_iar_num_hypotheses.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_iar_num_hypotheses.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->iar_num_hypotheses = u_iar_num_hypotheses.real;
      offset += sizeof(this->iar_num_hypotheses);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/RTKBaseline"; };
    virtual const char * getMD5() override { return "bd5852b76aa13136cec34a65089dfdb2"; };

  };

}
#endif
