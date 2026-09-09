#ifndef _ROS_mbf_msgs_MoveBaseActionFeedback_h
#define _ROS_mbf_msgs_MoveBaseActionFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "mbf_msgs/MoveBaseFeedback.h"

namespace mbf_msgs
{

  class MoveBaseActionFeedback : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef actionlib_msgs::GoalStatus _status_type;
      _status_type status;
      typedef mbf_msgs::MoveBaseFeedback _feedback_type;
      _feedback_type feedback;

    MoveBaseActionFeedback():
      header(),
      status(),
      feedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->status.serialize(outbuffer + offset);
      offset += this->feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->status.deserialize(inbuffer + offset);
      offset += this->feedback.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "mbf_msgs/MoveBaseActionFeedback"; };
    virtual const char * getMD5() override { return "41e5119fe263f5296a0eba2eff692cd2"; };

  };

}
#endif
