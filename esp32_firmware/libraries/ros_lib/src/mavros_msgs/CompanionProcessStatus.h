#ifndef _ROS_mavros_msgs_CompanionProcessStatus_h
#define _ROS_mavros_msgs_CompanionProcessStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class CompanionProcessStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _state_type;
      _state_type state;
      typedef uint8_t _component_type;
      _component_type component;
      enum { MAV_STATE_UNINIT =  0 };
      enum { MAV_STATE_BOOT =  1 };
      enum { MAV_STATE_CALIBRATING =  2 };
      enum { MAV_STATE_STANDBY =  3 };
      enum { MAV_STATE_ACTIVE =  4 };
      enum { MAV_STATE_CRITICAL =  5 };
      enum { MAV_STATE_EMERGENCY =  6 };
      enum { MAV_STATE_POWEROFF =  7 };
      enum { MAV_STATE_FLIGHT_TERMINATION =  8 };
      enum { MAV_COMP_ID_OBSTACLE_AVOIDANCE =  196 };
      enum { MAV_COMP_ID_VISUAL_INERTIAL_ODOMETRY =  197 };

    CompanionProcessStatus():
      header(),
      state(0),
      component(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->state);
      *(outbuffer + offset + 0) = (this->component >> (8 * 0)) & 0xFF;
      offset += sizeof(this->component);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
      this->component =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->component);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/CompanionProcessStatus"; };
    virtual const char * getMD5() override { return "b6dd787fcd873e87778987b1845f4cb5"; };

  };

}
#endif
