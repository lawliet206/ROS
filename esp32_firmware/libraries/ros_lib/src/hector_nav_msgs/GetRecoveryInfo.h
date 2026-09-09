#ifndef _ROS_SERVICE_GetRecoveryInfo_h
#define _ROS_SERVICE_GetRecoveryInfo_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"
#include "geometry_msgs/PoseStamped.h"
#include "nav_msgs/Path.h"

namespace hector_nav_msgs
{

static const char GETRECOVERYINFO[] = "hector_nav_msgs/GetRecoveryInfo";

  class GetRecoveryInfoRequest : public ros::Msg
  {
    public:
      typedef ros::Time _request_time_type;
      _request_time_type request_time;
      typedef float _request_radius_type;
      _request_radius_type request_radius;

    GetRecoveryInfoRequest():
      request_time(),
      request_radius(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->request_time.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->request_time.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->request_time.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->request_time.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->request_time.sec);
      *(outbuffer + offset + 0) = (this->request_time.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->request_time.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->request_time.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->request_time.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->request_time.nsec);
      offset += serializeAvrFloat64(outbuffer + offset, this->request_radius);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->request_time.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->request_time.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->request_time.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->request_time.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->request_time.sec);
      this->request_time.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->request_time.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->request_time.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->request_time.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->request_time.nsec);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->request_radius));
     return offset;
    }

    virtual const char * getType() override { return GETRECOVERYINFO; };
    virtual const char * getMD5() override { return "3916a0c55958d5dd43204cd2fe5608f6"; };

  };

  class GetRecoveryInfoResponse : public ros::Msg
  {
    public:
      typedef nav_msgs::Path _trajectory_radius_entry_pose_to_req_pose_type;
      _trajectory_radius_entry_pose_to_req_pose_type trajectory_radius_entry_pose_to_req_pose;
      typedef geometry_msgs::PoseStamped _radius_entry_pose_type;
      _radius_entry_pose_type radius_entry_pose;
      typedef geometry_msgs::PoseStamped _req_pose_type;
      _req_pose_type req_pose;

    GetRecoveryInfoResponse():
      trajectory_radius_entry_pose_to_req_pose(),
      radius_entry_pose(),
      req_pose()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->trajectory_radius_entry_pose_to_req_pose.serialize(outbuffer + offset);
      offset += this->radius_entry_pose.serialize(outbuffer + offset);
      offset += this->req_pose.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->trajectory_radius_entry_pose_to_req_pose.deserialize(inbuffer + offset);
      offset += this->radius_entry_pose.deserialize(inbuffer + offset);
      offset += this->req_pose.deserialize(inbuffer + offset);
     return offset;
    }

    virtual const char * getType() override { return GETRECOVERYINFO; };
    virtual const char * getMD5() override { return "a93581be8e34e3c09aeafc6b9b990ad5"; };

  };

  class GetRecoveryInfo {
    public:
    typedef GetRecoveryInfoRequest Request;
    typedef GetRecoveryInfoResponse Response;
  };

}
#endif
