#ifndef _ROS_mavros_msgs_Trajectory_h
#define _ROS_mavros_msgs_Trajectory_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/PositionTarget.h"

namespace mavros_msgs
{

  class Trajectory : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _type_type;
      _type_type type;
      typedef mavros_msgs::PositionTarget _point_1_type;
      _point_1_type point_1;
      typedef mavros_msgs::PositionTarget _point_2_type;
      _point_2_type point_2;
      typedef mavros_msgs::PositionTarget _point_3_type;
      _point_3_type point_3;
      typedef mavros_msgs::PositionTarget _point_4_type;
      _point_4_type point_4;
      typedef mavros_msgs::PositionTarget _point_5_type;
      _point_5_type point_5;
      uint8_t point_valid[5];
      uint16_t command[5];
      float time_horizon[5];
      enum { MAV_TRAJECTORY_REPRESENTATION_WAYPOINTS =  0 };
      enum { MAV_TRAJECTORY_REPRESENTATION_BEZIER =  1 };

    Trajectory():
      header(),
      type(0),
      point_1(),
      point_2(),
      point_3(),
      point_4(),
      point_5(),
      point_valid(),
      command(),
      time_horizon()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      offset += this->point_1.serialize(outbuffer + offset);
      offset += this->point_2.serialize(outbuffer + offset);
      offset += this->point_3.serialize(outbuffer + offset);
      offset += this->point_4.serialize(outbuffer + offset);
      offset += this->point_5.serialize(outbuffer + offset);
      for( uint32_t i = 0; i < 5; i++){
      *(outbuffer + offset + 0) = (this->point_valid[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->point_valid[i]);
      }
      for( uint32_t i = 0; i < 5; i++){
      *(outbuffer + offset + 0) = (this->command[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->command[i] >> (8 * 1)) & 0xFF;
      offset += sizeof(this->command[i]);
      }
      for( uint32_t i = 0; i < 5; i++){
      union {
        float real;
        uint32_t base;
      } u_time_horizoni;
      u_time_horizoni.real = this->time_horizon[i];
      *(outbuffer + offset + 0) = (u_time_horizoni.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_time_horizoni.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_time_horizoni.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_time_horizoni.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_horizon[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      offset += this->point_1.deserialize(inbuffer + offset);
      offset += this->point_2.deserialize(inbuffer + offset);
      offset += this->point_3.deserialize(inbuffer + offset);
      offset += this->point_4.deserialize(inbuffer + offset);
      offset += this->point_5.deserialize(inbuffer + offset);
      for( uint32_t i = 0; i < 5; i++){
      this->point_valid[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->point_valid[i]);
      }
      for( uint32_t i = 0; i < 5; i++){
      this->command[i] =  ((uint16_t) (*(inbuffer + offset)));
      this->command[i] |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->command[i]);
      }
      for( uint32_t i = 0; i < 5; i++){
      union {
        float real;
        uint32_t base;
      } u_time_horizoni;
      u_time_horizoni.base = 0;
      u_time_horizoni.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_time_horizoni.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_time_horizoni.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_time_horizoni.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->time_horizon[i] = u_time_horizoni.real;
      offset += sizeof(this->time_horizon[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Trajectory"; };
    virtual const char * getMD5() override { return "477b47a103394ad6be940e5705f338e8"; };

  };

}
#endif
