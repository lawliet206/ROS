#ifndef _ROS_SERVICE_CheckPath_h
#define _ROS_SERVICE_CheckPath_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nav_msgs/Path.h"

namespace mbf_msgs
{

static const char CHECKPATH[] = "mbf_msgs/CheckPath";

  class CheckPathRequest : public ros::Msg
  {
    public:
      typedef nav_msgs::Path _path_type;
      _path_type path;
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
      typedef uint8_t _return_on_type;
      _return_on_type return_on;
      typedef uint8_t _skip_poses_type;
      _skip_poses_type skip_poses;
      typedef bool _path_cells_only_type;
      _path_cells_only_type path_cells_only;
      enum { LOCAL_COSTMAP =  1 };
      enum { GLOBAL_COSTMAP =  2 };

    CheckPathRequest():
      path(),
      safety_dist(0),
      lethal_cost_mult(0),
      inscrib_cost_mult(0),
      unknown_cost_mult(0),
      costmap(0),
      return_on(0),
      skip_poses(0),
      path_cells_only(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->path.serialize(outbuffer + offset);
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
      *(outbuffer + offset + 0) = (this->return_on >> (8 * 0)) & 0xFF;
      offset += sizeof(this->return_on);
      *(outbuffer + offset + 0) = (this->skip_poses >> (8 * 0)) & 0xFF;
      offset += sizeof(this->skip_poses);
      union {
        bool real;
        uint8_t base;
      } u_path_cells_only;
      u_path_cells_only.real = this->path_cells_only;
      *(outbuffer + offset + 0) = (u_path_cells_only.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->path_cells_only);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->path.deserialize(inbuffer + offset);
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
      this->return_on =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->return_on);
      this->skip_poses =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->skip_poses);
      union {
        bool real;
        uint8_t base;
      } u_path_cells_only;
      u_path_cells_only.base = 0;
      u_path_cells_only.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->path_cells_only = u_path_cells_only.real;
      offset += sizeof(this->path_cells_only);
     return offset;
    }

    virtual const char * getType() override { return CHECKPATH; };
    virtual const char * getMD5() override { return "16f70020b17f5c034724ed8fb518b9f5"; };

  };

  class CheckPathResponse : public ros::Msg
  {
    public:
      typedef uint32_t _last_checked_type;
      _last_checked_type last_checked;
      typedef uint8_t _state_type;
      _state_type state;
      typedef uint32_t _cost_type;
      _cost_type cost;
      enum { FREE =   0     };
      enum { INSCRIBED =   1     };
      enum { LETHAL =   2     };
      enum { UNKNOWN =   3     };
      enum { OUTSIDE =   4     };

    CheckPathResponse():
      last_checked(0),
      state(0),
      cost(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->last_checked >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->last_checked >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->last_checked >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->last_checked >> (8 * 3)) & 0xFF;
      offset += sizeof(this->last_checked);
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
      this->last_checked =  ((uint32_t) (*(inbuffer + offset)));
      this->last_checked |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->last_checked |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->last_checked |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->last_checked);
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
      this->cost =  ((uint32_t) (*(inbuffer + offset)));
      this->cost |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->cost);
     return offset;
    }

    virtual const char * getType() override { return CHECKPATH; };
    virtual const char * getMD5() override { return "420eb6a13d128bba3770710452ea1c17"; };

  };

  class CheckPath {
    public:
    typedef CheckPathRequest Request;
    typedef CheckPathResponse Response;
  };

}
#endif
