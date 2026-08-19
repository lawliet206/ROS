#ifndef _ROS_mbf_msgs_MoveBaseAction_h
#define _ROS_mbf_msgs_MoveBaseAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mbf_msgs/MoveBaseActionGoal.h"
#include "mbf_msgs/MoveBaseActionResult.h"
#include "mbf_msgs/MoveBaseActionFeedback.h"

namespace mbf_msgs
{

  class MoveBaseAction : public ros::Msg
  {
    public:
      typedef mbf_msgs::MoveBaseActionGoal _action_goal_type;
      _action_goal_type action_goal;
      typedef mbf_msgs::MoveBaseActionResult _action_result_type;
      _action_result_type action_result;
      typedef mbf_msgs::MoveBaseActionFeedback _action_feedback_type;
      _action_feedback_type action_feedback;

    MoveBaseAction():
      action_goal(),
      action_result(),
      action_feedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->action_goal.serialize(outbuffer + offset);
      offset += this->action_result.serialize(outbuffer + offset);
      offset += this->action_feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->action_goal.deserialize(inbuffer + offset);
      offset += this->action_result.deserialize(inbuffer + offset);
      offset += this->action_feedback.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/MoveBaseAction"; };
    virtual const char * getMD5() override { return "c98ceb3d38dd07397b09c7280aa0d553"; };

  };

}
#endif
