#ifndef _ROS_mbf_msgs_MoveBaseGoal_h
#define _ROS_mbf_msgs_MoveBaseGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace mbf_msgs
{

  class MoveBaseGoal : public ros::Msg
  {
    public:
      typedef geometry_msgs::PoseStamped _target_pose_type;
      _target_pose_type target_pose;
      typedef const char* _controller_type;
      _controller_type controller;
      typedef const char* _planner_type;
      _planner_type planner;
      uint32_t recovery_behaviors_length;
      typedef char* _recovery_behaviors_type;
      _recovery_behaviors_type st_recovery_behaviors;
      _recovery_behaviors_type * recovery_behaviors;

    MoveBaseGoal():
      target_pose(),
      controller(""),
      planner(""),
      recovery_behaviors_length(0), st_recovery_behaviors(), recovery_behaviors(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->target_pose.serialize(outbuffer + offset);
      uint32_t length_controller = strlen(this->controller);
      varToArr(outbuffer + offset, length_controller);
      offset += 4;
      memcpy(outbuffer + offset, this->controller, length_controller);
      offset += length_controller;
      uint32_t length_planner = strlen(this->planner);
      varToArr(outbuffer + offset, length_planner);
      offset += 4;
      memcpy(outbuffer + offset, this->planner, length_planner);
      offset += length_planner;
      *(outbuffer + offset + 0) = (this->recovery_behaviors_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->recovery_behaviors_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->recovery_behaviors_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->recovery_behaviors_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->recovery_behaviors_length);
      for( uint32_t i = 0; i < recovery_behaviors_length; i++){
      uint32_t length_recovery_behaviorsi = strlen(this->recovery_behaviors[i]);
      varToArr(outbuffer + offset, length_recovery_behaviorsi);
      offset += 4;
      memcpy(outbuffer + offset, this->recovery_behaviors[i], length_recovery_behaviorsi);
      offset += length_recovery_behaviorsi;
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->target_pose.deserialize(inbuffer + offset);
      uint32_t length_controller;
      arrToVar(length_controller, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_controller; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_controller-1]=0;
      this->controller = (char *)(inbuffer + offset-1);
      offset += length_controller;
      uint32_t length_planner;
      arrToVar(length_planner, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_planner; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_planner-1]=0;
      this->planner = (char *)(inbuffer + offset-1);
      offset += length_planner;
      uint32_t recovery_behaviors_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      recovery_behaviors_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      recovery_behaviors_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      recovery_behaviors_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->recovery_behaviors_length);
      if(recovery_behaviors_lengthT > recovery_behaviors_length)
        this->recovery_behaviors = (char**)realloc(this->recovery_behaviors, recovery_behaviors_lengthT * sizeof(char*));
      recovery_behaviors_length = recovery_behaviors_lengthT;
      for( uint32_t i = 0; i < recovery_behaviors_length; i++){
      uint32_t length_st_recovery_behaviors;
      arrToVar(length_st_recovery_behaviors, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_recovery_behaviors; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_recovery_behaviors-1]=0;
      this->st_recovery_behaviors = (char *)(inbuffer + offset-1);
      offset += length_st_recovery_behaviors;
        memcpy( &(this->recovery_behaviors[i]), &(this->st_recovery_behaviors), sizeof(char*));
      }
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/MoveBaseGoal"; };
    virtual const char * getMD5() override { return "722601faf59588c53b718bb090b96808"; };

  };

}
#endif
