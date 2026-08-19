#ifndef _ROS_SERVICE_CheckPose_h
#define _ROS_SERVICE_CheckPose_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace mbf_msgs
{

static const char CHECKPOSE[] = "mbf_msgs/CheckPose";

  class CheckPoseRequest : public ros::Msg
  {
    public:
      typedef geometry_msgs::PoseStamped _pose_type;
      _pose_type pose;
      typedef float _safety_dist_type;
      _safety_dist_type safety_dist;
      typedef float _lethal_cost_mult_type;
      _lethal_cost_mult_type lethal_cost_mult;
      typedef float _inscrib_cost_mult_type;
      _inscrib_cost_mult_type inscrib_cost_mult;
      typedef float _unknown_cost_mult_type;
      _unknown_cost_mult_type unknown_cost_mult;
      typedef uint8_t _costmap_type;
      _costmap_type costmap;
      typedef bool _current_pose_type;
      _current_pose_type current_pose;
      enum { LOCAL_COSTMAP =  1 };
      enum { GLOBAL_COSTMAP =  2 };

    CheckPoseRequest():
      pose(),
      safety_dist(0),
      lethal_cost_mult(0),
      inscrib_cost_mult(0),
      unknown_cost_mult(0),
      costmap(0),
      current_pose(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->pose.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_safety_dist;
      u_safety_dist.real = this->safety_dist;
      *(outbuffer + offset + 0) = (u_safety_dist.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_safety_dist.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_safety_dist.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_safety_dist.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->safety_dist);
      union {
        float real;
        uint32_t base;
      } u_lethal_cost_mult;
      u_lethal_cost_mult.real = this->lethal_cost_mult;
      *(outbuffer + offset + 0) = (u_lethal_cost_mult.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_lethal_cost_mult.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_lethal_cost_mult.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_lethal_cost_mult.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->lethal_cost_mult);
      union {
        float real;
        uint32_t base;
      } u_inscrib_cost_mult;
      u_inscrib_cost_mult.real = this->inscrib_cost_mult;
      *(outbuffer + offset + 0) = (u_inscrib_cost_mult.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_inscrib_cost_mult.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_inscrib_cost_mult.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_inscrib_cost_mult.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->inscrib_cost_mult);
      union {
        float real;
        uint32_t base;
      } u_unknown_cost_mult;
      u_unknown_cost_mult.real = this->unknown_cost_mult;
      *(outbuffer + offset + 0) = (u_unknown_cost_mult.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_unknown_cost_mult.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_unknown_cost_mult.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_unknown_cost_mult.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->unknown_cost_mult);
      *(outbuffer + offset + 0) = (this->costmap >> (8 * 0)) & 0xFF;
      offset += sizeof(this->costmap);
      union {
        bool real;
        uint8_t base;
      } u_current_pose;
      u_current_pose.real = this->current_pose;
      *(outbuffer + offset + 0) = (u_current_pose.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->current_pose);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->pose.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_safety_dist;
      u_safety_dist.base = 0;
      u_safety_dist.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_safety_dist.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_safety_dist.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_safety_dist.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->safety_dist = u_safety_dist.real;
      offset += sizeof(this->safety_dist);
      union {
        float real;
        uint32_t base;
      } u_lethal_cost_mult;
      u_lethal_cost_mult.base = 0;
      u_lethal_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_lethal_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_lethal_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_lethal_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->lethal_cost_mult = u_lethal_cost_mult.real;
      offset += sizeof(this->lethal_cost_mult);
      union {
        float real;
        uint32_t base;
      } u_inscrib_cost_mult;
      u_inscrib_cost_mult.base = 0;
      u_inscrib_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_inscrib_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_inscrib_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_inscrib_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->inscrib_cost_mult = u_inscrib_cost_mult.real;
      offset += sizeof(this->inscrib_cost_mult);
      union {
        float real;
        uint32_t base;
      } u_unknown_cost_mult;
      u_unknown_cost_mult.base = 0;
      u_unknown_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_unknown_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_unknown_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_unknown_cost_mult.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->unknown_cost_mult = u_unknown_cost_mult.real;
      offset += sizeof(this->unknown_cost_mult);
      this->costmap =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->costmap);
      union {
        bool real;
        uint8_t base;
      } u_current_pose;
      u_current_pose.base = 0;
      u_current_pose.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->current_pose = u_current_pose.real;
      offset += sizeof(this->current_pose);
     return offset;
    }

    virtual const char * getType() override { return CHECKPOSE; };
    virtual const char * getMD5() override { return "265e0591fcb39074b9d918fb13f423f4"; };

  };

  class CheckPoseResponse : public ros::Msg
  {
    public:
      typedef uint8_t _state_type;
      _state_type state;
      typedef uint32_t _cost_type;
      _cost_type cost;
      enum { FREE =   0     };
      enum { INSCRIBED =   1     };
      enum { LETHAL =   2     };
      enum { UNKNOWN =   3     };
      enum { OUTSIDE =   4     };

    CheckPoseResponse():
      state(0),
      cost(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->state);
      *(outbuffer + offset + 0) = (this->cost >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cost >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->cost >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->cost >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cost);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
      this->cost =  ((uint32_t) (*(inbuffer + offset)));
      this->cost |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->cost);
     return offset;
    }

    virtual const char * getType() override { return CHECKPOSE; };
    virtual const char * getMD5() override { return "d74139e1f7169aa4fb64b44c3a698692"; };

  };

  class CheckPose {
    public:
    typedef CheckPoseRequest Request;
    typedef CheckPoseResponse Response;
  };

}
#endif
