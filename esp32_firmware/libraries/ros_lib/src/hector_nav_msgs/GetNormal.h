#ifndef _ROS_SERVICE_GetNormal_h
#define _ROS_SERVICE_GetNormal_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Vector3.h"

namespace hector_nav_msgs
{

static const char GETNORMAL[] = "hector_nav_msgs/GetNormal";

  class GetNormalRequest : public ros::Msg
  {
    public:
      typedef geometry_msgs::PointStamped _point_type;
      _point_type point;

    GetNormalRequest():
      point()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->point.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->point.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return GETNORMAL; };
    virtual const char * getMD5() override { return "47dfdbd810b48d0a47b7ad67e4191bcc"; };

  };

  class GetNormalResponse : public ros::Msg
  {
    public:
      typedef geometry_msgs::Vector3 _normal_type;
      _normal_type normal;

    GetNormalResponse():
      normal()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->normal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->normal.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return GETNORMAL; };
    virtual const char * getMD5() override { return "9a5880458dbcd28bf7ed1889c8ac7f8e"; };

  };

  class GetNormal {
    public:
    typedef GetNormalRequest Request;
    typedef GetNormalResponse Response;
  };

}
#endif
