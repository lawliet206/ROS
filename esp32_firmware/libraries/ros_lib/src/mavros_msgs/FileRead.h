#ifndef _ROS_SERVICE_FileRead_h
#define _ROS_SERVICE_FileRead_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

static const char FILEREAD[] = "mavros_msgs/FileRead";

  class FileReadRequest : public ros::Msg
  {
    public:
      typedef const char* _file_path_type;
      _file_path_type file_path;
      typedef uint64_t _offset_type;
      _offset_type offset;
      typedef uint64_t _size_type;
      _size_type size;

    FileReadRequest():
      file_path(""),
      offset(0),
      size(0)
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
      *(outbuffer + offset + 0) = (this->offset >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->offset >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->offset >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->offset >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->offset >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->offset >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->offset >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->offset >> (8 * 7)) & 0xFF;
      offset += sizeof(this->offset);
      *(outbuffer + offset + 0) = (this->size >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->size >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->size >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->size >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->size >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->size >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->size >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->size >> (8 * 7)) & 0xFF;
      offset += sizeof(this->size);
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
      this->offset =  ((uint64_t) (*(inbuffer + offset)));
      this->offset |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->offset |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->offset);
      this->size =  ((uint64_t) (*(inbuffer + offset)));
      this->size |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->size |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->size |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->size |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->size |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->size |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->size |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->size);
     return offset;
    }

    virtual const char * getType() override { return FILEREAD; };
    virtual const char * getMD5() override { return "c83da8c18af06c9d7d1b66667fa2bb6b"; };

  };

  class FileReadResponse : public ros::Msg
  {
    public:
      uint32_t data_length;
      typedef uint8_t _data_type;
      _data_type st_data;
      _data_type * data;
      typedef bool _success_type;
      _success_type success;
      typedef int32_t _r_errno_type;
      _r_errno_type r_errno;

    FileReadResponse():
      data_length(0), st_data(), data(nullptr),
      success(0),
      r_errno(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->data_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->data_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->data_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->data_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data_length);
      for( uint32_t i = 0; i < data_length; i++){
      *(outbuffer + offset + 0) = (this->data[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->data[i]);
      }
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
      uint32_t data_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->data_length);
      if(data_lengthT > data_length)
        this->data = (uint8_t*)realloc(this->data, data_lengthT * sizeof(uint8_t));
      data_length = data_lengthT;
      for( uint32_t i = 0; i < data_length; i++){
      this->st_data =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->st_data);
        memcpy( &(this->data[i]), &(this->st_data), sizeof(uint8_t));
      }
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

    virtual const char * getType() override { return FILEREAD; };
    virtual const char * getMD5() override { return "729aa1e22d45390356095d59a2993cb4"; };

  };

  class FileRead {
    public:
    typedef FileReadRequest Request;
    typedef FileReadResponse Response;
  };

}
#endif
