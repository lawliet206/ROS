#ifndef _ROS_mbf_msgs_ExePathAction_h
#define _ROS_mbf_msgs_ExePathAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mbf_msgs/ExePathActionGoal.h"
#include "mbf_msgs/ExePathActionResult.h"
#include "mbf_msgs/ExePathActionFeedback.h"

namespace mbf_msgs
{

  class ExePathAction : public ros::Msg
  {
    public:
      typedef mbf_msgs::ExePathActionGoal _action_goal_type;
      _action_goal_type action_goal;
      typedef mbf_msgs::ExePathActionResult _action_result_type;
      _action_result_type action_result;
      typedef mbf_msgs::ExePathActionFeedback _action_feedback_type;
      _action_feedback_type action_feedback;

    ExePathAction():
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

    virtual const char * getType() override { return "mbf_msgs/ExePathAction"; };
    virtual const char * getMD5() override { return "1eb3204035d1ceb85b999c9c0a477f7b"; };

  };

}
#endif
