#ifndef _ROS_mavros_msgs_ExtendedState_h
#define _ROS_mavros_msgs_ExtendedState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class ExtendedState : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _vtol_state_type;
      _vtol_state_type vtol_state;
      typedef uint8_t _landed_state_type;
      _landed_state_type landed_state;
      enum { VTOL_STATE_UNDEFINED =  0 };
      enum { VTOL_STATE_TRANSITION_TO_FW =  1 };
      enum { VTOL_STATE_TRANSITION_TO_MC =  2 };
      enum { VTOL_STATE_MC =  3 };
      enum { VTOL_STATE_FW =  4 };
      enum { LANDED_STATE_UNDEFINED =  0 };
      enum { LANDED_STATE_ON_GROUND =  1 };
      enum { LANDED_STATE_IN_AIR =  2 };
      enum { LANDED_STATE_TAKEOFF =  3 };
      enum { LANDED_STATE_LANDING =  4 };

    ExtendedState():
      header(),
      vtol_state(0),
      landed_state(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->vtol_state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->vtol_state);
      *(outbuffer + offset + 0) = (this->landed_state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->landed_state);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->vtol_state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->vtol_state);
      this->landed_state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->landed_state);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ExtendedState"; };
    virtual const char * getMD5() override { return "ae780b1800fe17b917369d21b90058bd"; };

  };

}
#endif
