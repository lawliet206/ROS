#ifndef _ROS_mavros_msgs_LogEntry_h
#define _ROS_mavros_msgs_LogEntry_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ros/time.h"

namespace mavros_msgs
{

  class LogEntry : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint16_t _id_type;
      _id_type id;
      typedef uint16_t _num_logs_type;
      _num_logs_type num_logs;
      typedef uint16_t _last_log_num_type;
      _last_log_num_type last_log_num;
      typedef ros::Time _time_utc_type;
      _time_utc_type time_utc;
      typedef uint32_t _size_type;
      _size_type size;

    LogEntry():
      header(),
      id(0),
      num_logs(0),
      last_log_num(0),
      time_utc(),
      size(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->id >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->id >> (8 * 1)) & 0xFF;
      offset += sizeof(this->id);
      *(outbuffer + offset + 0) = (this->num_logs >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->num_logs >> (8 * 1)) & 0xFF;
      offset += sizeof(this->num_logs);
      *(outbuffer + offset + 0) = (this->last_log_num >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->last_log_num >> (8 * 1)) & 0xFF;
      offset += sizeof(this->last_log_num);
      *(outbuffer + offset + 0) = (this->time_utc.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_utc.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_utc.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_utc.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_utc.sec);
      *(outbuffer + offset + 0) = (this->time_utc.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_utc.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_utc.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_utc.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_utc.nsec);
      *(outbuffer + offset + 0) = (this->size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->size >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->size >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->size >> (8 * 3)) & 0xFF;
      offset += sizeof(this->size);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->id =  ((uint16_t) (*(inbuffer + offset)));
      this->id |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->id);
      this->num_logs =  ((uint16_t) (*(inbuffer + offset)));
      this->num_logs |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->num_logs);
      this->last_log_num =  ((uint16_t) (*(inbuffer + offset)));
      this->last_log_num |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->last_log_num);
      this->time_utc.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_utc.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_utc.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_utc.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_utc.sec);
      this->time_utc.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_utc.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_utc.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_utc.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_utc.nsec);
      this->size =  ((uint32_t) (*(inbuffer + offset)));
      this->size |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->size |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->size |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->size);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/LogEntry"; };
    virtual const char * getMD5() override { return "a1428fc1ec4b2bfc8ab0c0ead7cce571"; };

  };

}
#endif
