#ifndef _ROS_mavros_msgs_DebugValue_h
#define _ROS_mavros_msgs_DebugValue_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class DebugValue : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef int32_t _index_type;
      _index_type index;
      typedef int32_t _array_id_type;
      _array_id_type array_id;
      typedef const char* _name_type;
      _name_type name;
      typedef float _value_float_type;
      _value_float_type value_float;
      typedef int32_t _value_int_type;
      _value_int_type value_int;
      uint32_t data_length;
      typedef float _data_type;
      _data_type st_data;
      _data_type * data;
      typedef uint8_t _type_type;
      _type_type type;
      enum { TYPE_DEBUG =  0 };
      enum { TYPE_DEBUG_VECT =  1 };
      enum { TYPE_DEBUG_FLOAT_ARRAY =  2 };
      enum { TYPE_NAMED_VALUE_FLOAT =  3 };
      enum { TYPE_NAMED_VALUE_INT =  4 };

    DebugValue():
      header(),
      index(0),
      array_id(0),
      name(""),
      value_float(0),
      value_int(0),
      data_length(0), st_data(), data(nullptr),
      type(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_index;
      u_index.real = this->index;
      *(outbuffer + offset + 0) = (u_index.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_index.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_index.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_index.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->index);
      union {
        int32_t real;
        uint32_t base;
      } u_array_id;
      u_array_id.real = this->array_id;
      *(outbuffer + offset + 0) = (u_array_id.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_array_id.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_array_id.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_array_id.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->array_id);
      uint32_t length_name = strlen(this->name);
      varToArr(outbuffer + offset, length_name);
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
      union {
        float real;
        uint32_t base;
      } u_value_float;
      u_value_float.real = this->value_float;
      *(outbuffer + offset + 0) = (u_value_float.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value_float.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value_float.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value_float.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->value_float);
      union {
        int32_t real;
        uint32_t base;
      } u_value_int;
      u_value_int.real = this->value_int;
      *(outbuffer + offset + 0) = (u_value_int.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_value_int.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_value_int.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_value_int.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->value_int);
      *(outbuffer + offset + 0) = (this->data_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->data_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->data_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->data_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data_length);
      for( uint32_t i = 0; i < data_length; i++){
      union {
        float real;
        uint32_t base;
      } u_datai;
      u_datai.real = this->data[i];
      *(outbuffer + offset + 0) = (u_datai.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_datai.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_datai.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_datai.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data[i]);
      }
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_index;
      u_index.base = 0;
      u_index.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_index.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_index.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_index.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->index = u_index.real;
      offset += sizeof(this->index);
      union {
        int32_t real;
        uint32_t base;
      } u_array_id;
      u_array_id.base = 0;
      u_array_id.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_array_id.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_array_id.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_array_id.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->array_id = u_array_id.real;
      offset += sizeof(this->array_id);
      uint32_t length_name;
      arrToVar(length_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
      union {
        float real;
        uint32_t base;
      } u_value_float;
      u_value_float.base = 0;
      u_value_float.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value_float.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value_float.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value_float.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->value_float = u_value_float.real;
      offset += sizeof(this->value_float);
      union {
        int32_t real;
        uint32_t base;
      } u_value_int;
      u_value_int.base = 0;
      u_value_int.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_value_int.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_value_int.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_value_int.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->value_int = u_value_int.real;
      offset += sizeof(this->value_int);
      uint32_t data_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->data_length);
      if(data_lengthT > data_length)
        this->data = (float*)realloc(this->data, data_lengthT * sizeof(float));
      data_length = data_lengthT;
      for( uint32_t i = 0; i < data_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_data;
      u_st_data.base = 0;
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_data = u_st_data.real;
      offset += sizeof(this->st_data);
        memcpy( &(this->data[i]), &(this->st_data), sizeof(float));
      }
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/DebugValue"; };
    virtual const char * getMD5() override { return "a385507a0141af80b5f68662747f1e68"; };

  };

}
#endif
