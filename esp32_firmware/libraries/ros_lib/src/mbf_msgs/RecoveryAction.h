#ifndef _ROS_mbf_msgs_RecoveryAction_h
#define _ROS_mbf_msgs_RecoveryAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mbf_msgs/RecoveryActionGoal.h"
#include "mbf_msgs/RecoveryActionResult.h"
#include "mbf_msgs/RecoveryActionFeedback.h"

namespace mbf_msgs
{

  class RecoveryAction : public ros::Msg
  {
    public:
      typedef mbf_msgs::RecoveryActionGoal _action_goal_type;
      _action_goal_type action_goal;
      typedef mbf_msgs::RecoveryActionResult _action_result_type;
      _action_result_type action_result;
      typedef mbf_msgs::RecoveryActionFeedback _action_feedback_type;
      _action_feedback_type action_feedback;

    RecoveryAction():
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

    virtual const char * getType() override { return "mbf_msgs/RecoveryAction"; };
    virtual const char * getMD5() override { return "b35ffa9c1783e3d612414d87fbc80598"; };

  };

}
#endif
