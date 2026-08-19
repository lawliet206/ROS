#ifndef _ROS_mavros_msgs_HomePosition_h
#define _ROS_mavros_msgs_HomePosition_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geographic_msgs/GeoPoint.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Vector3.h"

namespace mavros_msgs
{

  class HomePosition : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geographic_msgs::GeoPoint _geo_type;
      _geo_type geo;
      typedef geometry_msgs::Point _position_type;
      _position_type position;
      typedef geometry_msgs::Quaternion _orientation_type;
      _orientation_type orientation;
      typedef geometry_msgs::Vector3 _approach_type;
      _approach_type approach;

    HomePosition():
      header(),
      geo(),
      position(),
      orientation(),
      approach()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->geo.serialize(outbuffer + offset);
      offset += this->position.serialize(outbuffer + offset);
      offset += this->orientation.serialize(outbuffer + offset);
      offset += this->approach.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->geo.deserialize(inbuffer + offset);
      offset += this->position.deserialize(inbuffer + offset);
      offset += this->orientation.deserialize(inbuffer + offset);
      offset += this->approach.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/HomePosition"; };
    virtual const char * getMD5() override { return "c1167922de8c97acdb0ec714c1dba774"; };

  };

}
#endif
