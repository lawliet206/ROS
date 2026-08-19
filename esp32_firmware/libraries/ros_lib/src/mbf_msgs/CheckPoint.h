#ifndef _ROS_SERVICE_CheckPoint_h
#define _ROS_SERVICE_CheckPoint_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/PointStamped.h"

namespace mbf_msgs
{

static const char CHECKPOINT[] = "mbf_msgs/CheckPoint";

  class CheckPointRequest : public ros::Msg
  {
    public:
      typedef geometry_msgs::PointStamped _point_type;
      _point_type point;
      typedef uint8_t _costmap_type;
      _costmap_type costmap;
      enum { LOCAL_COSTMAP =  1 };
      enum { GLOBAL_COSTMAP =  2 };

    CheckPointRequest():
      point(),
      costmap(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->point.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->costmap >> (8 * 0)) & 0xFF;
      offset += sizeof(this->costmap);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->point.deserialize(inbuffer + offset);
      this->costmap =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->costmap);
     return offset;
    }

    virtual const char * getType() override { return CHECKPOINT; };
    virtual const char * getMD5() override { return "36e9c2f425eee0a2ebd8c4b0aae9f573"; };

  };

  class CheckPointResponse : public ros::Msg
  {
    public:
      typedef uint8_t _state_type;
      _state_type state;
      typedef uint32_t _cost_type;
      _cost_type cost;
      enum { FREE =   0     };
      enum { INSCRIBED =   1     };
      enum { LETHAL =   2     };
      enum { UNKNOWN =   3     };
      enum { OUTSIDE =   4     };

    CheckPointResponse():
      state(0),
      cost(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->state >> (8 * 0)) & 0xFF;
      offset += sizeof(this->state);
      *(outbuffer + offset + 0) = (this->cost >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->cost >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->cost >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->cost >> (8 * 3)) & 0xFF;
      offset += sizeof(this->cost);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->state =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->state);
      this->cost =  ((uint32_t) (*(inbuffer + offset)));
      this->cost |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->cost |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->cost);
     return offset;
    }

    virtual const char * getType() override { return CHECKPOINT; };
    virtual const char * getMD5() override { return "d74139e1f7169aa4fb64b44c3a698692"; };

  };

  class CheckPoint {
    public:
    typedef CheckPointRequest Request;
    typedef CheckPointResponse Response;
  };

}
#endif
