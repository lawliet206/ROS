#ifndef _ROS_mavros_msgs_HilSensor_h
#define _ROS_mavros_msgs_HilSensor_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Vector3.h"

namespace mavros_msgs
{

  class HilSensor : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geometry_msgs::Vector3 _acc_type;
      _acc_type acc;
      typedef geometry_msgs::Vector3 _gyro_type;
      _gyro_type gyro;
      typedef geometry_msgs::Vector3 _mag_type;
      _mag_type mag;
      typedef float _abs_pressure_type;
      _abs_pressure_type abs_pressure;
      typedef float _diff_pressure_type;
      _diff_pressure_type diff_pressure;
      typedef float _pressure_alt_type;
      _pressure_alt_type pressure_alt;
      typedef float _temperature_type;
      _temperature_type temperature;
      typedef uint32_t _fields_updated_type;
      _fields_updated_type fields_updated;

    HilSensor():
      header(),
      acc(),
      gyro(),
      mag(),
      abs_pressure(0),
      diff_pressure(0),
      pressure_alt(0),
      temperature(0),
      fields_updated(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->acc.serialize(outbuffer + offset);
      offset += this->gyro.serialize(outbuffer + offset);
      offset += this->mag.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_abs_pressure;
      u_abs_pressure.real = this->abs_pressure;
      *(outbuffer + offset + 0) = (u_abs_pressure.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_abs_pressure.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_abs_pressure.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_abs_pressure.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->abs_pressure);
      union {
        float real;
        uint32_t base;
      } u_diff_pressure;
      u_diff_pressure.real = this->diff_pressure;
      *(outbuffer + offset + 0) = (u_diff_pressure.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_diff_pressure.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_diff_pressure.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_diff_pressure.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->diff_pressure);
      union {
        float real;
        uint32_t base;
      } u_pressure_alt;
      u_pressure_alt.real = this->pressure_alt;
      *(outbuffer + offset + 0) = (u_pressure_alt.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_pressure_alt.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_pressure_alt.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_pressure_alt.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->pressure_alt);
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
      *(outbuffer + offset + 0) = (this->fields_updated >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->fields_updated >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->fields_updated >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->fields_updated >> (8 * 3)) & 0xFF;
      offset += sizeof(this->fields_updated);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->acc.deserialize(inbuffer + offset);
      offset += this->gyro.deserialize(inbuffer + offset);
      offset += this->mag.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_abs_pressure;
      u_abs_pressure.base = 0;
      u_abs_pressure.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_abs_pressure.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_abs_pressure.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_abs_pressure.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->abs_pressure = u_abs_pressure.real;
      offset += sizeof(this->abs_pressure);
      union {
        float real;
        uint32_t base;
      } u_diff_pressure;
      u_diff_pressure.base = 0;
      u_diff_pressure.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_diff_pressure.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_diff_pressure.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_diff_pressure.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->diff_pressure = u_diff_pressure.real;
      offset += sizeof(this->diff_pressure);
      union {
        float real;
        uint32_t base;
      } u_pressure_alt;
      u_pressure_alt.base = 0;
      u_pressure_alt.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_pressure_alt.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_pressure_alt.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_pressure_alt.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->pressure_alt = u_pressure_alt.real;
      offset += sizeof(this->pressure_alt);
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
      this->fields_updated =  ((uint32_t) (*(inbuffer + offset)));
      this->fields_updated |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->fields_updated |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->fields_updated |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->fields_updated);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HilSensor"; };
    virtual const char * getMD5() override { return "2a892891e5c40d6dd1066bf1f394b5dc"; };

  };

}
#endif
