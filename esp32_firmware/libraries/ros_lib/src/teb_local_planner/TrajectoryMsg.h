#ifndef _ROS_teb_local_planner_TrajectoryMsg_h
#define _ROS_teb_local_planner_TrajectoryMsg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "teb_local_planner/TrajectoryPointMsg.h"

namespace teb_local_planner
{

  class TrajectoryMsg : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t trajectory_length;
      typedef teb_local_planner::TrajectoryPointMsg _trajectory_type;
      _trajectory_type st_trajectory;
      _trajectory_type * trajectory;

    TrajectoryMsg():
      header(),
      trajectory_length(0), st_trajectory(), trajectory(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->trajectory_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->trajectory_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->trajectory_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->trajectory_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->trajectory_length);
      for( uint32_t i = 0; i < trajectory_length; i++){
      offset += this->trajectory[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t trajectory_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      trajectory_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      trajectory_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      trajectory_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->trajectory_length);
      if(trajectory_lengthT > trajectory_length)
        this->trajectory = (teb_local_planner::TrajectoryPointMsg*)realloc(this->trajectory, trajectory_lengthT * sizeof(teb_local_planner::TrajectoryPointMsg));
      trajectory_length = trajectory_lengthT;
      for( uint32_t i = 0; i < trajectory_length; i++){
      offset += this->st_trajectory.deserialize(inbuffer + offset);
        memcpy( &(this->trajectory[i]), &(this->st_trajectory), sizeof(teb_local_planner::TrajectoryPointMsg));
      }
     return offset;
    }

    virtual const char * getType() override { return "teb_local_planner/TrajectoryMsg"; };
    virtual const char * getMD5() override { return "9dfdc1e62b3eb03a32af2423c5b7a0dd"; };

  };

}
#endif
