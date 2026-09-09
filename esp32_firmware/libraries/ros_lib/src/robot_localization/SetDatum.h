#ifndef _ROS_SERVICE_SetDatum_h
#define _ROS_SERVICE_SetDatum_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geographic_msgs/GeoPose.h"

namespace robot_localization
{

static const char SETDATUM[] = "robot_localization/SetDatum";

  class SetDatumRequest : public ros::Msg
  {
    public:
      typedef geographic_msgs::GeoPose _geo_pose_type;
      _geo_pose_type geo_pose;

    SetDatumRequest():
      geo_pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->geo_pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->geo_pose.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return SETDATUM; };
    virtual const char * getMD5() override { return "fe903ca95d0210defda73a1629604439"; };

  };

  class SetDatumResponse : public ros::Msg
  {
    public:

    SetDatumResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return SETDATUM; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetDatum {
    public:
    typedef SetDatumRequest Request;
    typedef SetDatumResponse Response;
  };

}
#endif
