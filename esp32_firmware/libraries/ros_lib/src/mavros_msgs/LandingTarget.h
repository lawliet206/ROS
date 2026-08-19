#ifndef _ROS_mavros_msgs_LandingTarget_h
#define _ROS_mavros_msgs_LandingTarget_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Pose.h"

namespace mavros_msgs
{

  class LandingTarget : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _target_num_type;
      _target_num_type target_num;
      typedef uint8_t _frame_type;
      _frame_type frame;
      float angle[2];
      typedef float _distance_type;
      _distance_type distance;
      float size[2];
      typedef geometry_msgs::Pose _pose_type;
      _pose_type pose;
      typedef uint8_t _type_type;
      _type_type type;
      enum { GLOBAL =  0                    };
      enum { LOCAL_NED =  2                 };
      enum { MISSION =  3                   };
      enum { GLOBAL_RELATIVE_ALT =  4       };
      enum { LOCAL_ENU =  5                 };
      enum { GLOBAL_INT =  6                };
      enum { GLOBAL_RELATIVE_ALT_INT =  7   };
      enum { LOCAL_OFFSET_NED =  8          };
      enum { BODY_NED =  9                  };
      enum { BODY_OFFSET_NED =  10          };
      enum { GLOBAL_TERRAIN_ALT =  11       };
      enum { GLOBAL_TERRAIN_ALT_INT =  12   };
      enum { LIGHT_BEACON =  0              };
      enum { RADIO_BEACON =  1              };
      enum { VISION_FIDUCIAL =  2           };
      enum { VISION_OTHER =  3              };

    LandingTarget():
      header(),
      target_num(0),
      frame(0),
      angle(),
      distance(0),
      size(),
      pose(),
      type(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->target_num >> (8 * 0)) & 0xFF;
      offset += sizeof(this->target_num);
      *(outbuffer + offset + 0) = (this->frame >> (8 * 0)) & 0xFF;
      offset += sizeof(this->frame);
      for( uint32_t i = 0; i < 2; i++){
      union {
        float real;
        uint32_t base;
      } u_anglei;
      u_anglei.real = this->angle[i];
      *(outbuffer + offset + 0) = (u_anglei.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_anglei.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_anglei.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_anglei.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle[i]);
      }
      union {
        float real;
        uint32_t base;
      } u_distance;
      u_distance.real = this->distance;
      *(outbuffer + offset + 0) = (u_distance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_distance.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_distance.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_distance.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->distance);
      for( uint32_t i = 0; i < 2; i++){
      union {
        float real;
        uint32_t base;
      } u_sizei;
      u_sizei.real = this->size[i];
      *(outbuffer + offset + 0) = (u_sizei.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_sizei.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_sizei.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_sizei.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->size[i]);
      }
      offset += this->pose.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->target_num =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->target_num);
      this->frame =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->frame);
      for( uint32_t i = 0; i < 2; i++){
      union {
        float real;
        uint32_t base;
      } u_anglei;
      u_anglei.base = 0;
      u_anglei.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_anglei.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_anglei.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_anglei.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle[i] = u_anglei.real;
      offset += sizeof(this->angle[i]);
      }
      union {
        float real;
        uint32_t base;
      } u_distance;
      u_distance.base = 0;
      u_distance.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_distance.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_distance.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_distance.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->distance = u_distance.real;
      offset += sizeof(this->distance);
      for( uint32_t i = 0; i < 2; i++){
      union {
        float real;
        uint32_t base;
      } u_sizei;
      u_sizei.base = 0;
      u_sizei.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_sizei.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_sizei.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_sizei.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->size[i] = u_sizei.real;
      offset += sizeof(this->size[i]);
      }
      offset += this->pose.deserialize(inbuffer + offset);
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/LandingTarget"; };
    virtual const char * getMD5() override { return "76548ee08437914830bb7319d04d5490"; };

  };

}
#endif
