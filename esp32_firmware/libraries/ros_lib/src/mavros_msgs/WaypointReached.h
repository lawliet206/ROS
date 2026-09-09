#ifndef _ROS_mavros_msgs_WaypointReached_h
#define _ROS_mavros_msgs_WaypointReached_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class WaypointReached : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint16_t _wp_seq_type;
      _wp_seq_type wp_seq;

    WaypointReached():
      header(),
      wp_seq(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->wp_seq >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wp_seq >> (8 * 1)) & 0xFF;
      offset += sizeof(this->wp_seq);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->wp_seq =  ((uint16_t) (*(inbuffer + offset)));
      this->wp_seq |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->wp_seq);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/WaypointReached"; };
    virtual const char * getMD5() override { return "1cf64d072d9f6aa0a6715922bdde6a35"; };

  };

}
#endif
