#ifndef _ROS_mavros_msgs_CameraImageCaptured_h
#define _ROS_mavros_msgs_CameraImageCaptured_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"
#include "geographic_msgs/GeoPoint.h"

namespace mavros_msgs
{

  class CameraImageCaptured : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef geometry_msgs::Quaternion _orientation_type;
      _orientation_type orientation;
      typedef geographic_msgs::GeoPoint _geo_type;
      _geo_type geo;
      typedef float _relative_alt_type;
      _relative_alt_type relative_alt;
      typedef int32_t _image_index_type;
      _image_index_type image_index;
      typedef int8_t _capture_result_type;
      _capture_result_type capture_result;
      typedef const char* _file_url_type;
      _file_url_type file_url;

    CameraImageCaptured():
      header(),
      orientation(),
      geo(),
      relative_alt(0),
      image_index(0),
      capture_result(0),
      file_url("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      offset += this->orientation.serialize(outbuffer + offset);
      offset += this->geo.serialize(outbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_relative_alt;
      u_relative_alt.real = this->relative_alt;
      *(outbuffer + offset + 0) = (u_relative_alt.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_relative_alt.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_relative_alt.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_relative_alt.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->relative_alt);
      union {
        int32_t real;
        uint32_t base;
      } u_image_index;
      u_image_index.real = this->image_index;
      *(outbuffer + offset + 0) = (u_image_index.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_image_index.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_image_index.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_image_index.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->image_index);
      union {
        int8_t real;
        uint8_t base;
      } u_capture_result;
      u_capture_result.real = this->capture_result;
      *(outbuffer + offset + 0) = (u_capture_result.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->capture_result);
      uint32_t length_file_url = strlen(this->file_url);
      varToArr(outbuffer + offset, length_file_url);
      offset += 4;
      memcpy(outbuffer + offset, this->file_url, length_file_url);
      offset += length_file_url;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      offset += this->orientation.deserialize(inbuffer + offset);
      offset += this->geo.deserialize(inbuffer + offset);
      union {
        float real;
        uint32_t base;
      } u_relative_alt;
      u_relative_alt.base = 0;
      u_relative_alt.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_relative_alt.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_relative_alt.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_relative_alt.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->relative_alt = u_relative_alt.real;
      offset += sizeof(this->relative_alt);
      union {
        int32_t real;
        uint32_t base;
      } u_image_index;
      u_image_index.base = 0;
      u_image_index.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_image_index.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_image_index.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_image_index.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->image_index = u_image_index.real;
      offset += sizeof(this->image_index);
      union {
        int8_t real;
        uint8_t base;
      } u_capture_result;
      u_capture_result.base = 0;
      u_capture_result.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->capture_result = u_capture_result.real;
      offset += sizeof(this->capture_result);
      uint32_t length_file_url;
      arrToVar(length_file_url, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_file_url; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_file_url-1]=0;
      this->file_url = (char *)(inbuffer + offset-1);
      offset += length_file_url;
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/CameraImageCaptured"; };
    virtual const char * getMD5() override { return "9559d135fc7e5e91d3f1b819ebcd7556"; };

  };

}
#endif
