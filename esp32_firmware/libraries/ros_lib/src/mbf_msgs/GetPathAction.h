#ifndef _ROS_mbf_msgs_GetPathAction_h
#define _ROS_mbf_msgs_GetPathAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mbf_msgs/GetPathActionGoal.h"
#include "mbf_msgs/GetPathActionResult.h"
#include "mbf_msgs/GetPathActionFeedback.h"

namespace mbf_msgs
{

  class GetPathAction : public ros::Msg
  {
    public:
      typedef mbf_msgs::GetPathActionGoal _action_goal_type;
      _action_goal_type action_goal;
      typedef mbf_msgs::GetPathActionResult _action_result_type;
      _action_result_type action_result;
      typedef mbf_msgs::GetPathActionFeedback _action_feedback_type;
      _action_feedback_type action_feedback;

    GetPathAction():
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

    virtual const char * getType() override { return "mbf_msgs/GetPathAction"; };
    virtual const char * getMD5() override { return "f4d6567e6c5805b81da135676625d187"; };

  };

}
#endif
