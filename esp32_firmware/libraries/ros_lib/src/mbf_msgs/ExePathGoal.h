#ifndef _ROS_mbf_msgs_ExePathGoal_h
#define _ROS_mbf_msgs_ExePathGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nav_msgs/Path.h"

namespace mbf_msgs
{

  class ExePathGoal : public ros::Msg
  {
    public:
      typedef nav_msgs::Path _path_type;
      _path_type path;
      typedef const char* _controller_type;
      _controller_type controller;
      typedef uint8_t _concurrency_slot_type;
      _concurrency_slot_type concurrency_slot;
      typedef bool _tolerance_from_action_type;
      _tolerance_from_action_type tolerance_from_action;
      typedef float _dist_tolerance_type;
      _dist_tolerance_type dist_tolerance;
      typedef float _angle_tolerance_type;
      _angle_tolerance_type angle_tolerance;

    ExePathGoal():
      path(),
      controller(""),
      concurrency_slot(0),
      tolerance_from_action(0),
      dist_tolerance(0),
      angle_tolerance(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->path.serialize(outbuffer + offset);
      uint32_t length_controller = strlen(this->controller);
      varToArr(outbuffer + offset, length_controller);
      offset += 4;
      memcpy(outbuffer + offset, this->controller, length_controller);
      offset += length_controller;
      *(outbuffer + offset + 0) = (this->concurrency_slot >> (8 * 0)) & 0xFF;
      offset += sizeof(this->concurrency_slot);
      union {
        bool real;
        uint8_t base;
      } u_tolerance_from_action;
      u_tolerance_from_action.real = this->tolerance_from_action;
      *(outbuffer + offset + 0) = (u_tolerance_from_action.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->tolerance_from_action);
      union {
        float real;
        uint32_t base;
      } u_dist_tolerance;
      u_dist_tolerance.real = this->dist_tolerance;
      *(outbuffer + offset + 0) = (u_dist_tolerance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_dist_tolerance.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_dist_tolerance.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_dist_tolerance.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->dist_tolerance);
      union {
        float real;
        uint32_t base;
      } u_angle_tolerance;
      u_angle_tolerance.real = this->angle_tolerance;
      *(outbuffer + offset + 0) = (u_angle_tolerance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_tolerance.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_tolerance.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_tolerance.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_tolerance);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->path.deserialize(inbuffer + offset);
      uint32_t length_controller;
      arrToVar(length_controller, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_controller; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_controller-1]=0;
      this->controller = (char *)(inbuffer + offset-1);
      offset += length_controller;
      this->concurrency_slot =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->concurrency_slot);
      union {
        bool real;
        uint8_t base;
      } u_tolerance_from_action;
      u_tolerance_from_action.base = 0;
      u_tolerance_from_action.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->tolerance_from_action = u_tolerance_from_action.real;
      offset += sizeof(this->tolerance_from_action);
      union {
        float real;
        uint32_t base;
      } u_dist_tolerance;
      u_dist_tolerance.base = 0;
      u_dist_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_dist_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_dist_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_dist_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->dist_tolerance = u_dist_tolerance.real;
      offset += sizeof(this->dist_tolerance);
      union {
        float real;
        uint32_t base;
      } u_angle_tolerance;
      u_angle_tolerance.base = 0;
      u_angle_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_tolerance.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_tolerance = u_angle_tolerance.real;
      offset += sizeof(this->angle_tolerance);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/ExePathGoal"; };
    virtual const char * getMD5() override { return "997d05ac3260fea4e2e2586ca47d2578"; };

  };

}
#endif
