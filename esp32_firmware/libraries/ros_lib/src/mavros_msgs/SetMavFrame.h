#ifndef _ROS_SERVICE_SetMavFrame_h
#define _ROS_SERVICE_SetMavFrame_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char SETMAVFRAME[] = "mavros_msgs/SetMavFrame";

  class SetMavFrameRequest : public ros::Msg
  {
    public:
      typedef uint8_t _mav_frame_type;
      _mav_frame_type mav_frame;
      enum { FRAME_GLOBAL =  0                    };
      enum { FRAME_LOCAL_NED =  1                 };
      enum { FRAME_MISSION =  2                   };
      enum { FRAME_GLOBAL_RELATIVE_ALT =  3       };
      enum { FRAME_LOCAL_ENU =  4                 };
      enum { FRAME_GLOBAL_INT =  5                };
      enum { FRAME_GLOBAL_RELATIVE_ALT_INT =  6   };
      enum { FRAME_LOCAL_OFFSET_NED =  7          };
      enum { FRAME_BODY_NED =  8                  };
      enum { FRAME_BODY_OFFSET_NED =  9           };
      enum { FRAME_GLOBAL_TERRAIN_ALT =  10       };
      enum { FRAME_GLOBAL_TERRAIN_ALT_INT =  11   };
      enum { FRAME_BODY_FRD =  12                 };
      enum { FRAME_RESERVED_13 =  13              };
      enum { FRAME_RESERVED_14 =  14              };
      enum { FRAME_RESERVED_15 =  15              };
      enum { FRAME_RESERVED_16 =  16              };
      enum { FRAME_RESERVED_17 =  17              };
      enum { FRAME_RESERVED_18 =  18              };
      enum { FRAME_RESERVED_19 =  19              };
      enum { FRAME_LOCAL_FRD =  20                };
      enum { FRAME_LOCAL_FLU =  21                };

    SetMavFrameRequest():
      mav_frame(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->mav_frame >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mav_frame);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->mav_frame =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mav_frame);
     return offset;
    }

    virtual const char * getType() override { return SETMAVFRAME; };
    virtual const char * getMD5() override { return "10d9e03dcd8d648e58b34bad2a28091f"; };

  };

  class SetMavFrameResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    SetMavFrameResponse():
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    virtual const char * getType() override { return SETMAVFRAME; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class SetMavFrame {
    public:
    typedef SetMavFrameRequest Request;
    typedef SetMavFrameResponse Response;
  };

}
#endif
