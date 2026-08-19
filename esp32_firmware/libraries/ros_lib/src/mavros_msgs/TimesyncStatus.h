#ifndef _ROS_mavros_msgs_TimesyncStatus_h
#define _ROS_mavros_msgs_TimesyncStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class TimesyncStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint64_t _remote_timestamp_ns_type;
      _remote_timestamp_ns_type remote_timestamp_ns;
      typedef int64_t _observed_offset_ns_type;
      _observed_offset_ns_type observed_offset_ns;
      typedef int64_t _estimated_offset_ns_type;
      _estimated_offset_ns_type estimated_offset_ns;
      typedef float _round_trip_time_ms_type;
      _round_trip_time_ms_type round_trip_time_ms;

    TimesyncStatus():
      header(),
      remote_timestamp_ns(0),
      observed_offset_ns(0),
      estimated_offset_ns(0),
      round_trip_time_ms(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->remote_timestamp_ns >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->remote_timestamp_ns >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->remote_timestamp_ns >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->remote_timestamp_ns >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->remote_timestamp_ns >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->remote_timestamp_ns >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->remote_timestamp_ns >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->remote_timestamp_ns >> (8 * 7)) & 0xFF;
      offset += sizeof(this->remote_timestamp_ns);
      union {
        int64_t real;
        uint64_t base;
      } u_observed_offset_ns;
      u_observed_offset_ns.real = this->observed_offset_ns;
      *(outbuffer + offset + 0) = (u_observed_offset_ns.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_observed_offset_ns.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_observed_offset_ns.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_observed_offset_ns.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_observed_offset_ns.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_observed_offset_ns.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_observed_offset_ns.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_observed_offset_ns.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->observed_offset_ns);
      union {
        int64_t real;
        uint64_t base;
      } u_estimated_offset_ns;
      u_estimated_offset_ns.real = this->estimated_offset_ns;
      *(outbuffer + offset + 0) = (u_estimated_offset_ns.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_estimated_offset_ns.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_estimated_offset_ns.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_estimated_offset_ns.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_estimated_offset_ns.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_estimated_offset_ns.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_estimated_offset_ns.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_estimated_offset_ns.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->estimated_offset_ns);
      union {
        float real;
        uint32_t base;
      } u_round_trip_time_ms;
      u_round_trip_time_ms.real = this->round_trip_time_ms;
      *(outbuffer + offset + 0) = (u_round_trip_time_ms.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_round_trip_time_ms.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_round_trip_time_ms.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_round_trip_time_ms.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->round_trip_time_ms);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->remote_timestamp_ns =  ((uint64_t) (*(inbuffer + offset)));
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->remote_timestamp_ns |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->remote_timestamp_ns);
      union {
        int64_t real;
        uint64_t base;
      } u_observed_offset_ns;
      u_observed_offset_ns.base = 0;
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_observed_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->observed_offset_ns = u_observed_offset_ns.real;
      offset += sizeof(this->observed_offset_ns);
      union {
        int64_t real;
        uint64_t base;
      } u_estimated_offset_ns;
      u_estimated_offset_ns.base = 0;
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_estimated_offset_ns.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->estimated_offset_ns = u_estimated_offset_ns.real;
      offset += sizeof(this->estimated_offset_ns);
      union {
        float real;
        uint32_t base;
      } u_round_trip_time_ms;
      u_round_trip_time_ms.base = 0;
      u_round_trip_time_ms.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_round_trip_time_ms.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_round_trip_time_ms.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_round_trip_time_ms.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->round_trip_time_ms = u_round_trip_time_ms.real;
      offset += sizeof(this->round_trip_time_ms);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/TimesyncStatus"; };
    virtual const char * getMD5() override { return "021ec8044e747bea518b441f374ba64b"; };

  };

}
#endif
