#ifndef _ROS_SERVICE_ToLL_h
#define _ROS_SERVICE_ToLL_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Point.h"
#include "geographic_msgs/GeoPoint.h"

namespace robot_localization
{

static const char TOLL[] = "robot_localization/ToLL";

  class ToLLRequest : public ros::Msg
  {
    public:
      typedef geometry_msgs::Point _map_point_type;
      _map_point_type map_point;

    ToLLRequest():
      map_point()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->map_point.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->map_point.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return TOLL; };
    virtual const char * getMD5() override { return "326fc0ec1385c52a253c06e024d9f49e"; };

  };

  class ToLLResponse : public ros::Msg
  {
    public:
      typedef geographic_msgs::GeoPoint _ll_point_type;
      _ll_point_type ll_point;

    ToLLResponse():
      ll_point()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->ll_point.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->ll_point.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return TOLL; };
    virtual const char * getMD5() override { return "191f0513cae6fcb95b63e2ec2e976938"; };

  };

  class ToLL {
    public:
    typedef ToLLRequest Request;
    typedef ToLLResponse Response;
  };

}
#endif
