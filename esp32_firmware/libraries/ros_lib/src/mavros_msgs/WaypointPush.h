#ifndef _ROS_SERVICE_WaypointPush_h
#define _ROS_SERVICE_WaypointPush_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mavros_msgs/Waypoint.h"

namespace mavros_msgs
{

static const char WAYPOINTPUSH[] = "mavros_msgs/WaypointPush";

  class WaypointPushRequest : public ros::Msg
  {
    public:
      typedef uint16_t _start_index_type;
      _start_index_type start_index;
      uint32_t waypoints_length;
      typedef mavros_msgs::Waypoint _waypoints_type;
      _waypoints_type st_waypoints;
      _waypoints_type * waypoints;

    WaypointPushRequest():
      start_index(0),
      waypoints_length(0), st_waypoints(), waypoints(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->start_index >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->start_index >> (8 * 1)) & 0xFF;
      offset += sizeof(this->start_index);
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
      this->start_index =  ((uint16_t) (*(inbuffer + offset)));
      this->start_index |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->start_index);
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

    virtual const char * getType() override { return WAYPOINTPUSH; };
    virtual const char * getMD5() override { return "b97216c84680b216429bf6c603ab2e03"; };

  };

  class WaypointPushResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef uint32_t _wp_transfered_type;
      _wp_transfered_type wp_transfered;

    WaypointPushResponse():
      success(0),
      wp_transfered(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      *(outbuffer + offset + 0) = (this->wp_transfered >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wp_transfered >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->wp_transfered >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->wp_transfered >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wp_transfered);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
      this->wp_transfered =  ((uint32_t) (*(inbuffer + offset)));
      this->wp_transfered |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->wp_transfered |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->wp_transfered |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->wp_transfered);
     return offset;
    }

    virtual const char * getType() override { return WAYPOINTPUSH; };
    virtual const char * getMD5() override { return "90e0074a42480231d34eed864d14365e"; };

  };

  class WaypointPush {
    public:
    typedef WaypointPushRequest Request;
    typedef WaypointPushResponse Response;
  };

}
#endif
