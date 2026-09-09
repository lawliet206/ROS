#ifndef _ROS_hector_mapping_HectorIterData_h
#define _ROS_hector_mapping_HectorIterData_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace hector_mapping
{

  class HectorIterData : public ros::Msg
  {
    public:
      float hessian[9];
      typedef float _conditionNum_type;
      _conditionNum_type conditionNum;
      typedef float _determinant_type;
      _determinant_type determinant;
      typedef float _conditionNum2d_type;
      _conditionNum2d_type conditionNum2d;
      typedef float _determinant2d_type;
      _determinant2d_type determinant2d;

    HectorIterData():
      hessian(),
      conditionNum(0),
      determinant(0),
      conditionNum2d(0),
      determinant2d(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 9; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->hessian[i]);
      }
      offset += serializeAvrFloat64(outbuffer + offset, this->conditionNum);
      offset += serializeAvrFloat64(outbuffer + offset, this->determinant);
      offset += serializeAvrFloat64(outbuffer + offset, this->conditionNum2d);
      offset += serializeAvrFloat64(outbuffer + offset, this->determinant2d);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      for( uint32_t i = 0; i < 9; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->hessian[i]));
      }
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->conditionNum));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->determinant));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->conditionNum2d));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->determinant2d));
     return offset;
    }

    virtual const char * getType() override { return "hector_mapping/HectorIterData"; };
    virtual const char * getMD5() override { return "ecedaa7e26b5fc817a1add44c17fec5f"; };

  };

}
#endif
