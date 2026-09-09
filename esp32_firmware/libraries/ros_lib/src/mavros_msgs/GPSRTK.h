#ifndef _ROS_mavros_msgs_GPSRTK_h
#define _ROS_mavros_msgs_GPSRTK_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class GPSRTK : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _rtk_receiver_id_type;
      _rtk_receiver_id_type rtk_receiver_id;
      typedef int16_t _wn_type;
      _wn_type wn;
      typedef uint32_t _tow_type;
      _tow_type tow;
      typedef uint8_t _rtk_health_type;
      _rtk_health_type rtk_health;
      typedef uint8_t _rtk_rate_type;
      _rtk_rate_type rtk_rate;
      typedef uint8_t _nsats_type;
      _nsats_type nsats;
      typedef int32_t _baseline_a_type;
      _baseline_a_type baseline_a;
      typedef int32_t _baseline_b_type;
      _baseline_b_type baseline_b;
      typedef int32_t _baseline_c_type;
      _baseline_c_type baseline_c;
      typedef uint32_t _accuracy_type;
      _accuracy_type accuracy;
      typedef int32_t _iar_num_hypotheses_type;
      _iar_num_hypotheses_type iar_num_hypotheses;

    GPSRTK():
      header(),
      rtk_receiver_id(0),
      wn(0),
      tow(0),
      rtk_health(0),
      rtk_rate(0),
      nsats(0),
      baseline_a(0),
      baseline_b(0),
      baseline_c(0),
      accuracy(0),
      iar_num_hypotheses(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->rtk_receiver_id >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rtk_receiver_id);
      union {
        int16_t real;
        uint16_t base;
      } u_wn;
      u_wn.real = this->wn;
      *(outbuffer + offset + 0) = (u_wn.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wn.base >> (8 * 1)) & 0xFF;
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
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_a;
      u_baseline_a.real = this->baseline_a;
      *(outbuffer + offset + 0) = (u_baseline_a.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_a.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_a.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_a.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_a);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_b;
      u_baseline_b.real = this->baseline_b;
      *(outbuffer + offset + 0) = (u_baseline_b.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_b.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_b.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_b.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_b);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_c;
      u_baseline_c.real = this->baseline_c;
      *(outbuffer + offset + 0) = (u_baseline_c.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_baseline_c.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_baseline_c.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_baseline_c.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->baseline_c);
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
      this->rtk_receiver_id =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rtk_receiver_id);
      union {
        int16_t real;
        uint16_t base;
      } u_wn;
      u_wn.base = 0;
      u_wn.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wn.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->wn = u_wn.real;
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
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_a;
      u_baseline_a.base = 0;
      u_baseline_a.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_a.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_a.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_a.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_a = u_baseline_a.real;
      offset += sizeof(this->baseline_a);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_b;
      u_baseline_b.base = 0;
      u_baseline_b.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_b.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_b.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_b.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_b = u_baseline_b.real;
      offset += sizeof(this->baseline_b);
      union {
        int32_t real;
        uint32_t base;
      } u_baseline_c;
      u_baseline_c.base = 0;
      u_baseline_c.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_baseline_c.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_baseline_c.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_baseline_c.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->baseline_c = u_baseline_c.real;
      offset += sizeof(this->baseline_c);
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

    virtual const char * getType() override { return "mavros_msgs/GPSRTK"; };
    virtual const char * getMD5() override { return "ab2651768aa6464b60b7bc8ae031e017"; };

  };

}
#endif
