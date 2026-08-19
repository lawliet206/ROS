#ifndef _ROS_mbf_msgs_MoveBaseResult_h
#define _ROS_mbf_msgs_MoveBaseResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace mbf_msgs
{

  class MoveBaseResult : public ros::Msg
  {
    public:
      typedef uint32_t _outcome_type;
      _outcome_type outcome;
      typedef const char* _message_type;
      _message_type message;
      typedef float _dist_to_goal_type;
      _dist_to_goal_type dist_to_goal;
      typedef float _angle_to_goal_type;
      _angle_to_goal_type angle_to_goal;
      typedef geometry_msgs::PoseStamped _final_pose_type;
      _final_pose_type final_pose;
      enum { SUCCESS =  0 };
      enum { FAILURE =  10 };
      enum { CANCELED =  11 };
      enum { COLLISION =  12 };
      enum { OSCILLATION =  13 };
      enum { START_BLOCKED =  14 };
      enum { GOAL_BLOCKED =  15 };
      enum { TF_ERROR =  16 };
      enum { INTERNAL_ERROR =  17 };
      enum { PLAN_FAILURE =  50 };
      enum { CTRL_FAILURE =  100 };

    MoveBaseResult():
      outcome(0),
      message(""),
      dist_to_goal(0),
      angle_to_goal(0),
      final_pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->outcome >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->outcome >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->outcome >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->outcome >> (8 * 3)) & 0xFF;
      offset += sizeof(this->outcome);
      uint32_t length_message = strlen(this->message);
      varToArr(outbuffer + offset, length_message);
      offset += 4;
      memcpy(outbuffer + offset, this->message, length_message);
      offset += length_message;
      union {
        float real;
        uint32_t base;
      } u_dist_to_goal;
      u_dist_to_goal.real = this->dist_to_goal;
      *(outbuffer + offset + 0) = (u_dist_to_goal.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_dist_to_goal.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_dist_to_goal.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_dist_to_goal.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->dist_to_goal);
      union {
        float real;
        uint32_t base;
      } u_angle_to_goal;
      u_angle_to_goal.real = this->angle_to_goal;
      *(outbuffer + offset + 0) = (u_angle_to_goal.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_to_goal.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_to_goal.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_to_goal.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_to_goal);
      offset += this->final_pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->outcome =  ((uint32_t) (*(inbuffer + offset)));
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->outcome |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->outcome);
      uint32_t length_message;
      arrToVar(length_message, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_message; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_message-1]=0;
      this->message = (char *)(inbuffer + offset-1);
      offset += length_message;
      union {
        float real;
        uint32_t base;
      } u_dist_to_goal;
      u_dist_to_goal.base = 0;
      u_dist_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_dist_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_dist_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_dist_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->dist_to_goal = u_dist_to_goal.real;
      offset += sizeof(this->dist_to_goal);
      union {
        float real;
        uint32_t base;
      } u_angle_to_goal;
      u_angle_to_goal.base = 0;
      u_angle_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_to_goal.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_to_goal = u_angle_to_goal.real;
      offset += sizeof(this->angle_to_goal);
      offset += this->final_pose.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/MoveBaseResult"; };
    virtual const char * getMD5() override { return "c65d301ffa20590244253c6a99c37c5e"; };

  };

}
#endif
