#ifndef _ROS_SERVICE_VehicleInfoGet_h
#define _ROS_SERVICE_VehicleInfoGet_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "mavros_msgs/VehicleInfo.h"

namespace mavros_msgs
{

static const char VEHICLEINFOGET[] = "mavros_msgs/VehicleInfoGet";

  class VehicleInfoGetRequest : public ros::Msg
  {
    public:
      typedef uint8_t _sysid_type;
      _sysid_type sysid;
      typedef uint8_t _compid_type;
      _compid_type compid;
      typedef bool _get_all_type;
      _get_all_type get_all;
      enum { GET_MY_SYSID =  0 };
      enum { GET_MY_COMPID =  0 };

    VehicleInfoGetRequest():
      sysid(0),
      compid(0),
      get_all(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->sysid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sysid);
      *(outbuffer + offset + 0) = (this->compid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->compid);
      union {
        bool real;
        uint8_t base;
      } u_get_all;
      u_get_all.real = this->get_all;
      *(outbuffer + offset + 0) = (u_get_all.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->get_all);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->sysid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sysid);
      this->compid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->compid);
      union {
        bool real;
        uint8_t base;
      } u_get_all;
      u_get_all.base = 0;
      u_get_all.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->get_all = u_get_all.real;
      offset += sizeof(this->get_all);
     return offset;
    }

    virtual const char * getType() override { return VEHICLEINFOGET; };
    virtual const char * getMD5() override { return "c1911e97179d4b379a979e2ab8e01e5c"; };

  };

  class VehicleInfoGetResponse : public ros::Msg
  {
    public:
      typedef bool _success_type;
      _success_type success;
      uint32_t vehicles_length;
      typedef mavros_msgs::VehicleInfo _vehicles_type;
      _vehicles_type st_vehicles;
      _vehicles_type * vehicles;

    VehicleInfoGetResponse():
      success(0),
      vehicles_length(0), st_vehicles(), vehicles(nullptr)
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
      *(outbuffer + offset + 0) = (this->vehicles_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->vehicles_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->vehicles_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->vehicles_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vehicles_length);
      for( uint32_t i = 0; i < vehicles_length; i++){
      offset += this->vehicles[i].serialize(outbuffer + offset);
      }
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
      uint32_t vehicles_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      vehicles_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      vehicles_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      vehicles_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->vehicles_length);
      if(vehicles_lengthT > vehicles_length)
        this->vehicles = (mavros_msgs::VehicleInfo*)realloc(this->vehicles, vehicles_lengthT * sizeof(mavros_msgs::VehicleInfo));
      vehicles_length = vehicles_lengthT;
      for( uint32_t i = 0; i < vehicles_length; i++){
      offset += this->st_vehicles.deserialize(inbuffer + offset);
        memcpy( &(this->vehicles[i]), &(this->st_vehicles), sizeof(mavros_msgs::VehicleInfo));
      }
     return offset;
    }

    virtual const char * getType() override { return VEHICLEINFOGET; };
    virtual const char * getMD5() override { return "7b33b68f66a6b66456d3bed6fe1dfda0"; };

  };

  class VehicleInfoGet {
    public:
    typedef VehicleInfoGetRequest Request;
    typedef VehicleInfoGetResponse Response;
  };

}
#endif
