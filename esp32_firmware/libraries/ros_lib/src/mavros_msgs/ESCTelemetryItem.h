#ifndef _ROS_mavros_msgs_ESCTelemetryItem_h
#define _ROS_mavros_msgs_ESCTelemetryItem_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class ESCTelemetryItem : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef float _temperature_type;
      _temperature_type temperature;
      typedef float _voltage_type;
      _voltage_type voltage;
      typedef float _current_type;
      _current_type current;
      typedef float _totalcurrent_type;
      _totalcurrent_type totalcurrent;
      typedef int32_t _rpm_type;
      _rpm_type rpm;
      typedef uint16_t _count_type;
      _count_type count;

    ESCTelemetryItem():
      header(),
      temperature(0),
      voltage(0),
      current(0),
      totalcurrent(0),
      rpm(0),
      count(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_temperature;
      u_temperature.real = this->temperature;
      *(outbuffer + offset + 0) = (u_temperature.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_temperature.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_temperature.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_temperature.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->temperature);
      union {
        float real;
        uint32_t base;
      } u_voltage;
      u_voltage.real = this->voltage;
      *(outbuffer + offset + 0) = (u_voltage.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_voltage.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_voltage.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_voltage.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->voltage);
      union {
        float real;
        uint32_t base;
      } u_current;
      u_current.real = this->current;
      *(outbuffer + offset + 0) = (u_current.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_current.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_current.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_current.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->current);
      union {
        float real;
        uint32_t base;
      } u_totalcurrent;
      u_totalcurrent.real = this->totalcurrent;
      *(outbuffer + offset + 0) = (u_totalcurrent.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_totalcurrent.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_totalcurrent.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_totalcurrent.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->totalcurrent);
      union {
        int32_t real;
        uint32_t base;
      } u_rpm;
      u_rpm.real = this->rpm;
      *(outbuffer + offset + 0) = (u_rpm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rpm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rpm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rpm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rpm);
      *(outbuffer + offset + 0) = (this->count >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->count >> (8 * 1)) & 0xFF;
      offset += sizeof(this->count);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_temperature;
      u_temperature.base = 0;
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_temperature.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->temperature = u_temperature.real;
      offset += sizeof(this->temperature);
      union {
        float real;
        uint32_t base;
      } u_voltage;
      u_voltage.base = 0;
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_voltage.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->voltage = u_voltage.real;
      offset += sizeof(this->voltage);
      union {
        float real;
        uint32_t base;
      } u_current;
      u_current.base = 0;
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_current.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->current = u_current.real;
      offset += sizeof(this->current);
      union {
        float real;
        uint32_t base;
      } u_totalcurrent;
      u_totalcurrent.base = 0;
      u_totalcurrent.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_totalcurrent.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_totalcurrent.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_totalcurrent.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->totalcurrent = u_totalcurrent.real;
      offset += sizeof(this->totalcurrent);
      union {
        int32_t real;
        uint32_t base;
      } u_rpm;
      u_rpm.base = 0;
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rpm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rpm = u_rpm.real;
      offset += sizeof(this->rpm);
      this->count =  ((uint16_t) (*(inbuffer + offset)));
      this->count |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->count);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ESCTelemetryItem"; };
    virtual const char * getMD5() override { return "a135c5d0c71a3bade75476291a42a6df"; };

  };

}
#endif
