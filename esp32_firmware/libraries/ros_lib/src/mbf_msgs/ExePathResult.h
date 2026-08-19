#ifndef _ROS_mbf_msgs_ExePathResult_h
#define _ROS_mbf_msgs_ExePathResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PoseStamped.h"

namespace mbf_msgs
{

  class ExePathResult : public ros::Msg
  {
    public:
      typedef uint32_t _outcome_type;
      _outcome_type outcome;
      typedef const char* _message_type;
      _message_type message;
      typedef geometry_msgs::PoseStamped _final_pose_type;
      _final_pose_type final_pose;
      typedef float _dist_to_goal_type;
      _dist_to_goal_type dist_to_goal;
      typedef float _angle_to_goal_type;
      _angle_to_goal_type angle_to_goal;
      enum { SUCCESS =  0 };
      enum { FAILURE =  100   };
      enum { CANCELED =  101 };
      enum { NO_VALID_CMD =  102 };
      enum { PAT_EXCEEDED =  103 };
      enum { COLLISION =  104 };
      enum { OSCILLATION =  105 };
      enum { ROBOT_STUCK =  106 };
      enum { MISSED_GOAL =  107 };
      enum { MISSED_PATH =  108 };
      enum { BLOCKED_PATH =  109 };
      enum { INVALID_PATH =  110 };
      enum { TF_ERROR =  111 };
      enum { NOT_INITIALIZED =  112 };
      enum { INVALID_PLUGIN =  113 };
      enum { INTERNAL_ERROR =  114 };
      enum { OUT_OF_MAP =  115   };
      enum { MAP_ERROR =  116   };
      enum { STOPPED =  117   };

    ExePathResult():
      outcome(0),
      message(""),
      final_pose(),
      dist_to_goal(0),
      angle_to_goal(0)
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
      offset += this->final_pose.serialize(outbuffer + offset);
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
      offset += this->final_pose.deserialize(inbuffer + offset);
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
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/ExePathResult"; };
    virtual const char * getMD5() override { return "b22f308686bb4e3a7364ea944ef68fd0"; };

  };

}
#endif
