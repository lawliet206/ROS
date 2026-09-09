#ifndef _ROS_teb_local_planner_FeedbackMsg_h
#define _ROS_teb_local_planner_FeedbackMsg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "teb_local_planner/TrajectoryMsg.h"
#include "costmap_converter/ObstacleArrayMsg.h"

namespace teb_local_planner
{

  class FeedbackMsg : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t trajectories_length;
      typedef teb_local_planner::TrajectoryMsg _trajectories_type;
      _trajectories_type st_trajectories;
      _trajectories_type * trajectories;
      typedef uint16_t _selected_trajectory_idx_type;
      _selected_trajectory_idx_type selected_trajectory_idx;
      typedef costmap_converter::ObstacleArrayMsg _obstacles_msg_type;
      _obstacles_msg_type obstacles_msg;

    FeedbackMsg():
      header(),
      trajectories_length(0), st_trajectories(), trajectories(nullptr),
      selected_trajectory_idx(0),
      obstacles_msg()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->trajectories_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->trajectories_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->trajectories_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->trajectories_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->trajectories_length);
      for( uint32_t i = 0; i < trajectories_length; i++){
      offset += this->trajectories[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->selected_trajectory_idx >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->selected_trajectory_idx >> (8 * 1)) & 0xFF;
      offset += sizeof(this->selected_trajectory_idx);
      offset += this->obstacles_msg.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t trajectories_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      trajectories_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      trajectories_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      trajectories_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->trajectories_length);
      if(trajectories_lengthT > trajectories_length)
        this->trajectories = (teb_local_planner::TrajectoryMsg*)realloc(this->trajectories, trajectories_lengthT * sizeof(teb_local_planner::TrajectoryMsg));
      trajectories_length = trajectories_lengthT;
      for( uint32_t i = 0; i < trajectories_length; i++){
      offset += this->st_trajectories.deserialize(inbuffer + offset);
        memcpy( &(this->trajectories[i]), &(this->st_trajectories), sizeof(teb_local_planner::TrajectoryMsg));
      }
      this->selected_trajectory_idx =  ((uint16_t) (*(inbuffer + offset)));
      this->selected_trajectory_idx |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->selected_trajectory_idx);
      offset += this->obstacles_msg.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "teb_local_planner/FeedbackMsg"; };
    virtual const char * getMD5() override { return "e8086148d3a39de24ca2cc423f1e94e6"; };

  };

}
#endif
