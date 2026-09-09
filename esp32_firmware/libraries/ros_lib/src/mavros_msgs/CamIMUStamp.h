#ifndef _ROS_mavros_msgs_CamIMUStamp_h
#define _ROS_mavros_msgs_CamIMUStamp_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace mavros_msgs
{

  class CamIMUStamp : public ros::Msg
  {
    public:
      typedef ros::Time _frame_stamp_type;
      _frame_stamp_type frame_stamp;
      typedef int32_t _frame_seq_id_type;
      _frame_seq_id_type frame_seq_id;

    CamIMUStamp():
      frame_stamp(),
      frame_seq_id(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->frame_stamp.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->frame_stamp.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->frame_stamp.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->frame_stamp.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_stamp.sec);
      *(outbuffer + offset + 0) = (this->frame_stamp.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->frame_stamp.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->frame_stamp.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->frame_stamp.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_stamp.nsec);
      union {
        int32_t real;
        uint32_t base;
      } u_frame_seq_id;
      u_frame_seq_id.real = this->frame_seq_id;
      *(outbuffer + offset + 0) = (u_frame_seq_id.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_frame_seq_id.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_frame_seq_id.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_frame_seq_id.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->frame_seq_id);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->frame_stamp.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->frame_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->frame_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->frame_stamp.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->frame_stamp.sec);
      this->frame_stamp.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->frame_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->frame_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->frame_stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->frame_stamp.nsec);
      union {
        int32_t real;
        uint32_t base;
      } u_frame_seq_id;
      u_frame_seq_id.base = 0;
      u_frame_seq_id.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_frame_seq_id.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_frame_seq_id.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_frame_seq_id.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->frame_seq_id = u_frame_seq_id.real;
      offset += sizeof(this->frame_seq_id);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/CamIMUStamp"; };
    virtual const char * getMD5() override { return "ac22af9031671dd528a56f12d0986660"; };

  };

}
#endif
