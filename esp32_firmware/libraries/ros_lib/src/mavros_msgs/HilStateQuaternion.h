#ifndef _ROS_mavros_msgs_HilStateQuaternion_h
#define _ROS_mavros_msgs_HilStateQuaternion_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Vector3.h"
#include "geographic_msgs/GeoPoint.h"

namespace mavros_msgs
{

  class HilStateQuaternion : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geometry_msgs::Quaternion _orientation_type;
      _orientation_type orientation;
      typedef geometry_msgs::Vector3 _angular_velocity_type;
      _angular_velocity_type angular_velocity;
      typedef geometry_msgs::Vector3 _linear_acceleration_type;
      _linear_acceleration_type linear_acceleration;
      typedef geometry_msgs::Vector3 _linear_velocity_type;
      _linear_velocity_type linear_velocity;
      typedef geographic_msgs::GeoPoint _geo_type;
      _geo_type geo;
      typedef float _ind_airspeed_type;
      _ind_airspeed_type ind_airspeed;
      typedef float _true_airspeed_type;
      _true_airspeed_type true_airspeed;

    HilStateQuaternion():
      header(),
      orientation(),
      angular_velocity(),
      linear_acceleration(),
      linear_velocity(),
      geo(),
      ind_airspeed(0),
      true_airspeed(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->orientation.serialize(outbuffer + offset);
      offset += this->angular_velocity.serialize(outbuffer + offset);
      offset += this->linear_acceleration.serialize(outbuffer + offset);
      offset += this->linear_velocity.serialize(outbuffer + offset);
      offset += this->geo.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_ind_airspeed;
      u_ind_airspeed.real = this->ind_airspeed;
      *(outbuffer + offset + 0) = (u_ind_airspeed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ind_airspeed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ind_airspeed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ind_airspeed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ind_airspeed);
      union {
        float real;
        uint32_t base;
      } u_true_airspeed;
      u_true_airspeed.real = this->true_airspeed;
      *(outbuffer + offset + 0) = (u_true_airspeed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_true_airspeed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_true_airspeed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_true_airspeed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->true_airspeed);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->orientation.deserialize(inbuffer + offset);
      offset += this->angular_velocity.deserialize(inbuffer + offset);
      offset += this->linear_acceleration.deserialize(inbuffer + offset);
      offset += this->linear_velocity.deserialize(inbuffer + offset);
      offset += this->geo.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_ind_airspeed;
      u_ind_airspeed.base = 0;
      u_ind_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ind_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ind_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ind_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ind_airspeed = u_ind_airspeed.real;
      offset += sizeof(this->ind_airspeed);
      union {
        float real;
        uint32_t base;
      } u_true_airspeed;
      u_true_airspeed.base = 0;
      u_true_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_true_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_true_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_true_airspeed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->true_airspeed = u_true_airspeed.real;
      offset += sizeof(this->true_airspeed);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HilStateQuaternion"; };
    virtual const char * getMD5() override { return "c858c0f05d4ab30256be7c53edee8e3c"; };

  };

}
#endif
