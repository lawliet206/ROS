#ifndef _ROS_mbf_msgs_GetPathGoal_h
#define _ROS_mbf_msgs_GetPathGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace mbf_msgs
{

  class GetPathGoal : public ros::Msg
  {
    public:
      typedef bool _use_start_pose_type;
      _use_start_pose_type use_start_pose;
      typedef geometry_msgs::PoseStamped _start_pose_type;
      _start_pose_type start_pose;
      typedef geometry_msgs::PoseStamped _target_pose_type;
      _target_pose_type target_pose;
      typedef float _tolerance_type;
      _tolerance_type tolerance;
      typedef const char* _planner_type;
      _planner_type planner;
      typedef uint8_t _concurrency_slot_type;
      _concurrency_slot_type concurrency_slot;

    GetPathGoal():
      use_start_pose(0),
      start_pose(),
      target_pose(),
      tolerance(0),
      planner(""),
      concurrency_slot(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_use_start_pose;
      u_use_start_pose.real = this->use_start_pose;
      *(outbuffer + offset + 0) = (u_use_start_pose.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->use_start_pose);
      offset += this->start_pose.serialize(outbuffer + offset);
      offset += this->target_pose.serialize(outbuffer + offset);
      offset += serializeAvrFloat64(outbuffer + offset, this->tolerance);
      uint32_t length_planner = strlen(this->planner);
      varToArr(outbuffer + offset, length_planner);
      offset += 4;
      memcpy(outbuffer + offset, this->planner, length_planner);
      offset += length_planner;
      *(outbuffer + offset + 0) = (this->concurrency_slot >> (8 * 0)) & 0xFF;
      offset += sizeof(this->concurrency_slot);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_use_start_pose;
      u_use_start_pose.base = 0;
      u_use_start_pose.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->use_start_pose = u_use_start_pose.real;
      offset += sizeof(this->use_start_pose);
      offset += this->start_pose.deserialize(inbuffer + offset);
      offset += this->target_pose.deserialize(inbuffer + offset);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->tolerance));
      uint32_t length_planner;
      arrToVar(length_planner, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_planner; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_planner-1]=0;
      this->planner = (char *)(inbuffer + offset-1);
      offset += length_planner;
      this->concurrency_slot =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->concurrency_slot);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/GetPathGoal"; };
    virtual const char * getMD5() override { return "301d9f5ec2f8f08d1d4e16663a6d2c5a"; };

  };

}
#endif
