#ifndef _ROS_SERVICE_FileTruncate_h
#define _ROS_SERVICE_FileTruncate_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char FILETRUNCATE[] = "mavros_msgs/FileTruncate";

  class FileTruncateRequest : public ros::Msg
  {
    public:
      typedef const char* _file_path_type;
      _file_path_type file_path;
      typedef uint64_t _length_type;
      _length_type length;

    FileTruncateRequest():
      file_path(""),
      length(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_file_path = strlen(this->file_path);
      varToArr(outbuffer + offset, length_file_path);
      offset += 4;
      memcpy(outbuffer + offset, this->file_path, length_file_path);
      offset += length_file_path;
      *(outbuffer + offset + 0) = (this->length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->length >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->length >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->length >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->length >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->length >> (8 * 7)) & 0xFF;
      offset += sizeof(this->length);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_file_path;
      arrToVar(length_file_path, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_file_path; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_file_path-1]=0;
      this->file_path = (char *)(inbuffer + offset-1);
      offset += length_file_path;
      this->length =  ((uint64_t) (*(inbuffer + offset)));
      this->length |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->length |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->length |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->length |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->length |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->length |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->length |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->length);
     return offset;
    }

    virtual const char * getType() override { return FILETRUNCATE; };
    virtual const char * getMD5() override { return "8153dbfb1601dd12c2e69aba3171d186"; };

  };

  class FileTruncateResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      typedef int32_t _r_errno_type;
      _r_errno_type r_errno;

    FileTruncateResponse():
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

    virtual const char * getType() override { return FILETRUNCATE; };
    virtual const char * getMD5() override { return "85394f2e941a8937ac567a617f06157f"; };

  };

  class FileTruncate {
    public:
    typedef FileTruncateRequest Request;
    typedef FileTruncateResponse Response;
  };

}
#endif
