#ifndef _ROS_mavros_msgs_State_h
#define _ROS_mavros_msgs_State_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class State : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef bool _connected_type;
      _connected_type connected;
      typedef bool _armed_type;
      _armed_type armed;
      typedef bool _guided_type;
      _guided_type guided;
      typedef bool _manual_input_type;
      _manual_input_type manual_input;
      typedef const char* _mode_type;
      _mode_type mode;
      typedef uint8_t _system_status_type;
      _system_status_type system_status;
      enum { MODE_APM_PLANE_MANUAL =  MANUAL };
      enum { MODE_APM_PLANE_CIRCLE =  CIRCLE };
      enum { MODE_APM_PLANE_STABILIZE =  STABILIZE };
      enum { MODE_APM_PLANE_TRAINING =  TRAINING };
      enum { MODE_APM_PLANE_ACRO =  ACRO };
      enum { MODE_APM_PLANE_FBWA =  FBWA };
      enum { MODE_APM_PLANE_FBWB =  FBWB };
      enum { MODE_APM_PLANE_CRUISE =  CRUISE };
      enum { MODE_APM_PLANE_AUTOTUNE =  AUTOTUNE };
      enum { MODE_APM_PLANE_AUTO =  AUTO };
      enum { MODE_APM_PLANE_RTL =  RTL };
      enum { MODE_APM_PLANE_LOITER =  LOITER };
      enum { MODE_APM_PLANE_LAND =  LAND };
      enum { MODE_APM_PLANE_GUIDED =  GUIDED };
      enum { MODE_APM_PLANE_INITIALISING =  INITIALISING };
      enum { MODE_APM_PLANE_QSTABILIZE =  QSTABILIZE };
      enum { MODE_APM_PLANE_QHOVER =  QHOVER };
      enum { MODE_APM_PLANE_QLOITER =  QLOITER };
      enum { MODE_APM_PLANE_QLAND =  QLAND };
      enum { MODE_APM_PLANE_QRTL =  QRTL };
      enum { MODE_APM_COPTER_STABILIZE =  STABILIZE };
      enum { MODE_APM_COPTER_ACRO =  ACRO };
      enum { MODE_APM_COPTER_ALT_HOLD =  ALT_HOLD };
      enum { MODE_APM_COPTER_AUTO =  AUTO };
      enum { MODE_APM_COPTER_GUIDED =  GUIDED };
      enum { MODE_APM_COPTER_LOITER =  LOITER };
      enum { MODE_APM_COPTER_RTL =  RTL };
      enum { MODE_APM_COPTER_CIRCLE =  CIRCLE };
      enum { MODE_APM_COPTER_POSITION =  POSITION };
      enum { MODE_APM_COPTER_LAND =  LAND };
      enum { MODE_APM_COPTER_OF_LOITER =  OF_LOITER };
      enum { MODE_APM_COPTER_DRIFT =  DRIFT };
      enum { MODE_APM_COPTER_SPORT =  SPORT };
      enum { MODE_APM_COPTER_FLIP =  FLIP };
      enum { MODE_APM_COPTER_AUTOTUNE =  AUTOTUNE };
      enum { MODE_APM_COPTER_POSHOLD =  POSHOLD };
      enum { MODE_APM_COPTER_BRAKE =  BRAKE };
      enum { MODE_APM_COPTER_THROW =  THROW };
      enum { MODE_APM_COPTER_AVOID_ADSB =  AVOID_ADSB };
      enum { MODE_APM_COPTER_GUIDED_NOGPS =  GUIDED_NOGPS };
      enum { MODE_APM_ROVER_MANUAL =  MANUAL };
      enum { MODE_APM_ROVER_LEARNING =  LEARNING };
      enum { MODE_APM_ROVER_STEERING =  STEERING };
      enum { MODE_APM_ROVER_HOLD =  HOLD };
      enum { MODE_APM_ROVER_AUTO =  AUTO };
      enum { MODE_APM_ROVER_RTL =  RTL };
      enum { MODE_APM_ROVER_GUIDED =  GUIDED };
      enum { MODE_APM_ROVER_INITIALISING =  INITIALISING };
      enum { MODE_PX4_MANUAL =  MANUAL };
      enum { MODE_PX4_ACRO =  ACRO };
      enum { MODE_PX4_ALTITUDE =  ALTCTL };
      enum { MODE_PX4_POSITION =  POSCTL };
      enum { MODE_PX4_OFFBOARD =  OFFBOARD };
      enum { MODE_PX4_STABILIZED =  STABILIZED };
      enum { MODE_PX4_RATTITUDE =  RATTITUDE };
      enum { MODE_PX4_MISSION =  AUTO.MISSION };
      enum { MODE_PX4_LOITER =  AUTO.LOITER };
      enum { MODE_PX4_RTL =  AUTO.RTL };
      enum { MODE_PX4_LAND =  AUTO.LAND };
      enum { MODE_PX4_RTGS =  AUTO.RTGS };
      enum { MODE_PX4_READY =  AUTO.READY };
      enum { MODE_PX4_TAKEOFF =  AUTO.TAKEOFF };

    State():
      header(),
      connected(0),
      armed(0),
      guided(0),
      manual_input(0),
      mode(""),
      system_status(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_connected;
      u_connected.real = this->connected;
      *(outbuffer + offset + 0) = (u_connected.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->connected);
      union {
        bool real;
        uint8_t base;
      } u_armed;
      u_armed.real = this->armed;
      *(outbuffer + offset + 0) = (u_armed.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->armed);
      union {
        bool real;
        uint8_t base;
      } u_guided;
      u_guided.real = this->guided;
      *(outbuffer + offset + 0) = (u_guided.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->guided);
      union {
        bool real;
        uint8_t base;
      } u_manual_input;
      u_manual_input.real = this->manual_input;
      *(outbuffer + offset + 0) = (u_manual_input.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->manual_input);
      uint32_t length_mode = strlen(this->mode);
      varToArr(outbuffer + offset, length_mode);
      offset += 4;
      memcpy(outbuffer + offset, this->mode, length_mode);
      offset += length_mode;
      *(outbuffer + offset + 0) = (this->system_status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->system_status);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_connected;
      u_connected.base = 0;
      u_connected.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->connected = u_connected.real;
      offset += sizeof(this->connected);
      union {
        bool real;
        uint8_t base;
      } u_armed;
      u_armed.base = 0;
      u_armed.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->armed = u_armed.real;
      offset += sizeof(this->armed);
      union {
        bool real;
        uint8_t base;
      } u_guided;
      u_guided.base = 0;
      u_guided.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->guided = u_guided.real;
      offset += sizeof(this->guided);
      union {
        bool real;
        uint8_t base;
      } u_manual_input;
      u_manual_input.base = 0;
      u_manual_input.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->manual_input = u_manual_input.real;
      offset += sizeof(this->manual_input);
      uint32_t length_mode;
      arrToVar(length_mode, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_mode; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_mode-1]=0;
      this->mode = (char *)(inbuffer + offset-1);
      offset += length_mode;
      this->system_status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->system_status);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/State"; };
    virtual const char * getMD5() override { return "65cd0a9fff993b062b91e354554ec7e9"; };

  };

}
#endif
