#ifndef _ROS_mbf_msgs_ExePathActionGoal_h
#define _ROS_mbf_msgs_ExePathActionGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "mbf_msgs/ExePathGoal.h"

namespace mbf_msgs
{

  class ExePathActionGoal : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef actionlib_msgs::GoalID _goal_id_type;
      _goal_id_type goal_id;
      typedef mbf_msgs::ExePathGoal _goal_type;
      _goal_type goal;

    ExePathActionGoal():
      header(),
      goal_id(),
      goal()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->goal_id.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->goal_id.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/ExePathActionGoal"; };
    virtual const char * getMD5() override { return "2e906bfb25f223a7c3bdf127e99a7e16"; };

  };

}
#endif
