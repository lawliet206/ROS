#ifndef _ROS_SERVICE_FileRename_h
#define _ROS_SERVICE_FileRename_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char FILERENAME[] = "mavros_msgs/FileRename";

  class FileRenameRequest : public ros::Msg
  {
    public:
      typedef const char* _old_path_type;
      _old_path_type old_path;
      typedef const char* _new_path_type;
      _new_path_type new_path;

    FileRenameRequest():
      old_path(""),
      new_path("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_old_path = strlen(this->old_path);
      varToArr(outbuffer + offset, length_old_path);
      offset += 4;
      memcpy(outbuffer + offset, this->old_path, length_old_path);
      offset += length_old_path;
      uint32_t length_new_path = strlen(this->new_path);
      varToArr(outbuffer + offset, length_new_path);
      offset += 4;
      memcpy(outbuffer + offset, this->new_path, length_new_path);
      offset += length_new_path;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_old_path;
      arrToVar(length_old_path, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_old_path; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_old_path-1]=0;
      this->old_path = (char *)(inbuffer + offset-1);
      offset += length_old_path;
      uint32_t length_new_path;
      arrToVar(length_new_path, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_new_path; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_new_path-1]=0;
      this->new_path = (char *)(inbuffer + offset-1);
      offset += length_new_path;
     return offset;
    }

    virtual const char * getType() override { return FILERENAME; };
    virtual const char * getMD5() override { return "e4a29684c4f7a3290a1bec0a9de2ed01"; };

  };

  class FileRenameResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef int32_t _r_errno_type;
      _r_errno_type r_errno;

    FileRenameResponse():
      success(0),
      r_errno(0)
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
      union {
        int32_t real;
        uint32_t base;
      } u_r_errno;
      u_r_errno.real = this->r_errno;
      *(outbuffer + offset + 0) = (u_r_errno.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_r_errno.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_r_errno.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_r_errno.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->r_errno);
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
      union {
        int32_t real;
        uint32_t base;
      } u_r_errno;
      u_r_errno.base = 0;
      u_r_errno.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_r_errno.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_r_errno.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_r_errno.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->r_errno = u_r_errno.real;
      offset += sizeof(this->r_errno);
     return offset;
    }

    virtual const char * getType() override { return FILERENAME; };
    virtual const char * getMD5() override { return "85394f2e941a8937ac567a617f06157f"; };

  };

  class FileRename {
    public:
    typedef FileRenameRequest Request;
    typedef FileRenameResponse Response;
  };

}
#endif
