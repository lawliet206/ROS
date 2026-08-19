#ifndef _ROS_mavros_msgs_WaypointList_h
#define _ROS_mavros_msgs_WaypointList_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mavros_msgs/Waypoint.h"

namespace mavros_msgs
{

  class WaypointList : public ros::Msg
  {
    public:
      typedef uint16_t _current_seq_type;
      _current_seq_type current_seq;
      uint32_t waypoints_length;
      typedef mavros_msgs::Waypoint _waypoints_type;
      _waypoints_type st_waypoints;
      _waypoints_type * waypoints;

    WaypointList():
      current_seq(0),
      waypoints_length(0), st_waypoints(), waypoints(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->current_seq >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->current_seq >> (8 * 1)) & 0xFF;
      offset += sizeof(this->current_seq);
      *(outbuffer + offset + 0) = (this->waypoints_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->waypoints_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->waypoints_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->waypoints_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->waypoints_length);
      for( uint32_t i = 0; i < waypoints_length; i++){
      offset += this->waypoints[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->current_seq =  ((uint16_t) (*(inbuffer + offset)));
      this->current_seq |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->current_seq);
      uint32_t waypoints_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      waypoints_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      waypoints_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      waypoints_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->waypoints_length);
      if(waypoints_lengthT > waypoints_length)
        this->waypoints = (mavros_msgs::Waypoint*)realloc(this->waypoints, waypoints_lengthT * sizeof(mavros_msgs::Waypoint));
      waypoints_length = waypoints_lengthT;
      for( uint32_t i = 0; i < waypoints_length; i++){
      offset += this->st_waypoints.deserialize(inbuffer + offset);
        memcpy( &(this->waypoints[i]), &(this->st_waypoints), sizeof(mavros_msgs::Waypoint));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/WaypointList"; };
    virtual const char * getMD5() override { return "0605a2df153acd9a4a4823385ed81b57"; };

  };

}
#endif
