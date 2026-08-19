#ifndef _ROS_SERVICE_FileList_h
#define _ROS_SERVICE_FileList_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mavros_msgs/FileEntry.h"

namespace mavros_msgs
{

static const char FILELIST[] = "mavros_msgs/FileList";

  class FileListRequest : public ros::Msg
  {
    public:
      typedef const char* _dir_path_type;
      _dir_path_type dir_path;

    FileListRequest():
      dir_path("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_dir_path = strlen(this->dir_path);
      varToArr(outbuffer + offset, length_dir_path);
      offset += 4;
      memcpy(outbuffer + offset, this->dir_path, length_dir_path);
      offset += length_dir_path;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_dir_path;
      arrToVar(length_dir_path, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_dir_path; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_dir_path-1]=0;
      this->dir_path = (char *)(inbuffer + offset-1);
      offset += length_dir_path;
     return offset;
    }

    virtual const char * getType() override { return FILELIST; };
    virtual const char * getMD5() override { return "401d5cf5f836aaa9ebdc0897f75da874"; };

  };

  class FileListResponse : public ros::Msg
  {
    public:
      uint32_t list_length;
      typedef mavros_msgs::FileEntry _list_type;
      _list_type st_list;
      _list_type * list;
      typedef bool _success_type;
      _success_type success;
      typedef int32_t _r_errno_type;
      _r_errno_type r_errno;

    FileListResponse():
      list_length(0), st_list(), list(nullptr),
      success(0),
      r_errno(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->list_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->list_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->list_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->list_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->list_length);
      for( uint32_t i = 0; i < list_length; i++){
      offset += this->list[i].serialize(outbuffer + offset);
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
      uint32_t list_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      list_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      list_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      list_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->list_length);
      if(list_lengthT > list_length)
        this->list = (mavros_msgs::FileEntry*)realloc(this->list, list_lengthT * sizeof(mavros_msgs::FileEntry));
      list_length = list_lengthT;
      for( uint32_t i = 0; i < list_length; i++){
      offset += this->st_list.deserialize(inbuffer + offset);
        memcpy( &(this->list[i]), &(this->st_list), sizeof(mavros_msgs::FileEntry));
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

    virtual const char * getType() override { return FILELIST; };
    virtual const char * getMD5() override { return "3cf4be333d40be8a08832e3b61ed4336"; };

  };

  class FileList {
    public:
    typedef FileListRequest Request;
    typedef FileListResponse Response;
  };

}
#endif
