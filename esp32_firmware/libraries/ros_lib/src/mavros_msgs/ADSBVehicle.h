#ifndef _ROS_mavros_msgs_ADSBVehicle_h
#define _ROS_mavros_msgs_ADSBVehicle_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "ros/duration.h"

namespace mavros_msgs
{

  class ADSBVehicle : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint32_t _ICAO_address_type;
      _ICAO_address_type ICAO_address;
      typedef const char* _callsign_type;
      _callsign_type callsign;
      typedef float _latitude_type;
      _latitude_type latitude;
      typedef float _longitude_type;
      _longitude_type longitude;
      typedef float _altitude_type;
      _altitude_type altitude;
      typedef float _heading_type;
      _heading_type heading;
      typedef float _hor_velocity_type;
      _hor_velocity_type hor_velocity;
      typedef float _ver_velocity_type;
      _ver_velocity_type ver_velocity;
      typedef uint8_t _altitude_type_type;
      _altitude_type_type altitude_type;
      typedef uint8_t _emitter_type_type;
      _emitter_type_type emitter_type;
      typedef ros::Duration _tslc_type;
      _tslc_type tslc;
      typedef uint16_t _flags_type;
      _flags_type flags;
      typedef uint16_t _squawk_type;
      _squawk_type squawk;
      enum { ALT_PRESSURE_QNH =  0                };
      enum { ALT_GEOMETRIC =  1                   };
      enum { EMITTER_NO_INFO =  0 };
      enum { EMITTER_LIGHT =  1 };
      enum { EMITTER_SMALL =  2 };
      enum { EMITTER_LARGE =  3 };
      enum { EMITTER_HIGH_VORTEX_LARGE =  4 };
      enum { EMITTER_HEAVY =  5 };
      enum { EMITTER_HIGHLY_MANUV =  6 };
      enum { EMITTER_ROTOCRAFT =  7 };
      enum { EMITTER_UNASSIGNED =  8 };
      enum { EMITTER_GLIDER =  9 };
      enum { EMITTER_LIGHTER_AIR =  10 };
      enum { EMITTER_PARACHUTE =  11 };
      enum { EMITTER_ULTRA_LIGHT =  12 };
      enum { EMITTER_UNASSIGNED2 =  13 };
      enum { EMITTER_UAV =  14 };
      enum { EMITTER_SPACE =  15 };
      enum { EMITTER_UNASSGINED3 =  16 };
      enum { EMITTER_EMERGENCY_SURFACE =  17 };
      enum { EMITTER_SERVICE_SURFACE =  18 };
      enum { EMITTER_POINT_OBSTACLE =  19 };
      enum { FLAG_VALID_COORDS =  1 };
      enum { FLAG_VALID_ALTITUDE =  2 };
      enum { FLAG_VALID_HEADING =  4 };
      enum { FLAG_VALID_VELOCITY =  8 };
      enum { FLAG_VALID_CALLSIGN =  16 };
      enum { FLAG_VALID_SQUAWK =  32 };
      enum { FLAG_SIMULATED =  64 };
      enum { FLAG_VERTICAL_VELOCITY_VALID =  128 };
      enum { FLAG_BARO_VALID =  256 };
      enum { FLAG_SOURCE_UAT =  32768 };

    ADSBVehicle():
      header(),
      ICAO_address(0),
      callsign(""),
      latitude(0),
      longitude(0),
      altitude(0),
      heading(0),
      hor_velocity(0),
      ver_velocity(0),
      altitude_type(0),
      emitter_type(0),
      tslc(),
      flags(0),
      squawk(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->ICAO_address >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ICAO_address >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ICAO_address >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ICAO_address >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ICAO_address);
      uint32_t length_callsign = strlen(this->callsign);
      varToArr(outbuffer + offset, length_callsign);
      offset += 4;
      memcpy(outbuffer + offset, this->callsign, length_callsign);
      offset += length_callsign;
      offset += serializeAvrFloat64(outbuffer + offset, this->latitude);
      offset += serializeAvrFloat64(outbuffer + offset, this->longitude);
      union {
        float real;
        uint32_t base;
      } u_altitude;
      u_altitude.real = this->altitude;
      *(outbuffer + offset + 0) = (u_altitude.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_altitude.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_altitude.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_altitude.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->altitude);
      union {
        float real;
        uint32_t base;
      } u_heading;
      u_heading.real = this->heading;
      *(outbuffer + offset + 0) = (u_heading.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_heading.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_heading.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_heading.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->heading);
      union {
        float real;
        uint32_t base;
      } u_hor_velocity;
      u_hor_velocity.real = this->hor_velocity;
      *(outbuffer + offset + 0) = (u_hor_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_hor_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_hor_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_hor_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->hor_velocity);
      union {
        float real;
        uint32_t base;
      } u_ver_velocity;
      u_ver_velocity.real = this->ver_velocity;
      *(outbuffer + offset + 0) = (u_ver_velocity.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_ver_velocity.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_ver_velocity.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_ver_velocity.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ver_velocity);
      *(outbuffer + offset + 0) = (this->altitude_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->altitude_type);
      *(outbuffer + offset + 0) = (this->emitter_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->emitter_type);
      *(outbuffer + offset + 0) = (this->tslc.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->tslc.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->tslc.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->tslc.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->tslc.sec);
      *(outbuffer + offset + 0) = (this->tslc.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->tslc.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->tslc.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->tslc.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->tslc.nsec);
      *(outbuffer + offset + 0) = (this->flags >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->flags >> (8 * 1)) & 0xFF;
      offset += sizeof(this->flags);
      *(outbuffer + offset + 0) = (this->squawk >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->squawk >> (8 * 1)) & 0xFF;
      offset += sizeof(this->squawk);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->ICAO_address =  ((uint32_t) (*(inbuffer + offset)));
      this->ICAO_address |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ICAO_address |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ICAO_address |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ICAO_address);
      uint32_t length_callsign;
      arrToVar(length_callsign, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_callsign; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_callsign-1]=0;
      this->callsign = (char *)(inbuffer + offset-1);
      offset += length_callsign;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->latitude));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->longitude));
      union {
        float real;
        uint32_t base;
      } u_altitude;
      u_altitude.base = 0;
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_altitude.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->altitude = u_altitude.real;
      offset += sizeof(this->altitude);
      union {
        float real;
        uint32_t base;
      } u_heading;
      u_heading.base = 0;
      u_heading.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_heading.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_heading.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_heading.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->heading = u_heading.real;
      offset += sizeof(this->heading);
      union {
        float real;
        uint32_t base;
      } u_hor_velocity;
      u_hor_velocity.base = 0;
      u_hor_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_hor_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_hor_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_hor_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->hor_velocity = u_hor_velocity.real;
      offset += sizeof(this->hor_velocity);
      union {
        float real;
        uint32_t base;
      } u_ver_velocity;
      u_ver_velocity.base = 0;
      u_ver_velocity.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_ver_velocity.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_ver_velocity.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_ver_velocity.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->ver_velocity = u_ver_velocity.real;
      offset += sizeof(this->ver_velocity);
      this->altitude_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->altitude_type);
      this->emitter_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->emitter_type);
      this->tslc.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->tslc.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->tslc.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->tslc.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->tslc.sec);
      this->tslc.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->tslc.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->tslc.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->tslc.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->tslc.nsec);
      this->flags =  ((uint16_t) (*(inbuffer + offset)));
      this->flags |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->flags);
      this->squawk =  ((uint16_t) (*(inbuffer + offset)));
      this->squawk |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->squawk);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/ADSBVehicle"; };
    virtual const char * getMD5() override { return "d532685113a66fcc6ba0e6363ace0244"; };

  };

}
#endif
