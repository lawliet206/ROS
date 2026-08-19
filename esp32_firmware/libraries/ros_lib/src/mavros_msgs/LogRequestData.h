#ifndef _ROS_SERVICE_LogRequestData_h
#define _ROS_SERVICE_LogRequestData_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char LOGREQUESTDATA[] = "mavros_msgs/LogRequestData";

  class LogRequestDataRequest : public ros::Msg
  {
    public:
      typedef uint16_t _id_type;
      _id_type id;
      typedef uint32_t _offset_type;
      _offset_type offset;
      typedef uint32_t _count_type;
      _count_type count;

    LogRequestDataRequest():
      id(0),
      offset(0),
      count(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->id >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->id >> (8 * 1)) & 0xFF;
      offset += sizeof(this->id);
      *(outbuffer + offset + 0) = (this->offset >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->offset >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->offset >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->offset >> (8 * 3)) & 0xFF;
      offset += sizeof(this->offset);
      *(outbuffer + offset + 0) = (this->count >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->count >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->count >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->count >> (8 * 3)) & 0xFF;
      offset += sizeof(this->count);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->id =  ((uint16_t) (*(inbuffer + offset)));
      this->id |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->id);
      this->offset =  ((uint32_t) (*(inbuffer + offset)));
      this->offset |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->offset |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->offset |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->offset);
      this->count =  ((uint32_t) (*(inbuffer + offset)));
      this->count |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->count |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->count |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->count);
     return offset;
    }

    virtual const char * getType() override { return LOGREQUESTDATA; };
    virtual const char * getMD5() override { return "9bd5fb12d79dcd29b3f845d7dd682415"; };

  };

  class LogRequestDataResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;

    LogRequestDataResponse():
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

    virtual const char * getType() override { return LOGREQUESTDATA; };
    virtual const char * getMD5() override { return "358e233cde0c8a8bcfea4ce193f8fc15"; };

  };

  class LogRequestData {
    public:
    typedef LogRequestDataRequest Request;
    typedef LogRequestDataResponse Response;
  };

}
#endif
