#ifndef _ROS_mavros_msgs_EstimatorStatus_h
#define _ROS_mavros_msgs_EstimatorStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class EstimatorStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef bool _attitude_status_flag_type;
      _attitude_status_flag_type attitude_status_flag;
      typedef bool _velocity_horiz_status_flag_type;
      _velocity_horiz_status_flag_type velocity_horiz_status_flag;
      typedef bool _velocity_vert_status_flag_type;
      _velocity_vert_status_flag_type velocity_vert_status_flag;
      typedef bool _pos_horiz_rel_status_flag_type;
      _pos_horiz_rel_status_flag_type pos_horiz_rel_status_flag;
      typedef bool _pos_horiz_abs_status_flag_type;
      _pos_horiz_abs_status_flag_type pos_horiz_abs_status_flag;
      typedef bool _pos_vert_abs_status_flag_type;
      _pos_vert_abs_status_flag_type pos_vert_abs_status_flag;
      typedef bool _pos_vert_agl_status_flag_type;
      _pos_vert_agl_status_flag_type pos_vert_agl_status_flag;
      typedef bool _const_pos_mode_status_flag_type;
      _const_pos_mode_status_flag_type const_pos_mode_status_flag;
      typedef bool _pred_pos_horiz_rel_status_flag_type;
      _pred_pos_horiz_rel_status_flag_type pred_pos_horiz_rel_status_flag;
      typedef bool _pred_pos_horiz_abs_status_flag_type;
      _pred_pos_horiz_abs_status_flag_type pred_pos_horiz_abs_status_flag;
      typedef bool _gps_glitch_status_flag_type;
      _gps_glitch_status_flag_type gps_glitch_status_flag;
      typedef bool _accel_error_status_flag_type;
      _accel_error_status_flag_type accel_error_status_flag;

    EstimatorStatus():
      header(),
      attitude_status_flag(0),
      velocity_horiz_status_flag(0),
      velocity_vert_status_flag(0),
      pos_horiz_rel_status_flag(0),
      pos_horiz_abs_status_flag(0),
      pos_vert_abs_status_flag(0),
      pos_vert_agl_status_flag(0),
      const_pos_mode_status_flag(0),
      pred_pos_horiz_rel_status_flag(0),
      pred_pos_horiz_abs_status_flag(0),
      gps_glitch_status_flag(0),
      accel_error_status_flag(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_attitude_status_flag;
      u_attitude_status_flag.real = this->attitude_status_flag;
      *(outbuffer + offset + 0) = (u_attitude_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->attitude_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_velocity_horiz_status_flag;
      u_velocity_horiz_status_flag.real = this->velocity_horiz_status_flag;
      *(outbuffer + offset + 0) = (u_velocity_horiz_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->velocity_horiz_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_velocity_vert_status_flag;
      u_velocity_vert_status_flag.real = this->velocity_vert_status_flag;
      *(outbuffer + offset + 0) = (u_velocity_vert_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->velocity_vert_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_horiz_rel_status_flag;
      u_pos_horiz_rel_status_flag.real = this->pos_horiz_rel_status_flag;
      *(outbuffer + offset + 0) = (u_pos_horiz_rel_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pos_horiz_rel_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_horiz_abs_status_flag;
      u_pos_horiz_abs_status_flag.real = this->pos_horiz_abs_status_flag;
      *(outbuffer + offset + 0) = (u_pos_horiz_abs_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pos_horiz_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_vert_abs_status_flag;
      u_pos_vert_abs_status_flag.real = this->pos_vert_abs_status_flag;
      *(outbuffer + offset + 0) = (u_pos_vert_abs_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pos_vert_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_vert_agl_status_flag;
      u_pos_vert_agl_status_flag.real = this->pos_vert_agl_status_flag;
      *(outbuffer + offset + 0) = (u_pos_vert_agl_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pos_vert_agl_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_const_pos_mode_status_flag;
      u_const_pos_mode_status_flag.real = this->const_pos_mode_status_flag;
      *(outbuffer + offset + 0) = (u_const_pos_mode_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->const_pos_mode_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pred_pos_horiz_rel_status_flag;
      u_pred_pos_horiz_rel_status_flag.real = this->pred_pos_horiz_rel_status_flag;
      *(outbuffer + offset + 0) = (u_pred_pos_horiz_rel_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pred_pos_horiz_rel_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pred_pos_horiz_abs_status_flag;
      u_pred_pos_horiz_abs_status_flag.real = this->pred_pos_horiz_abs_status_flag;
      *(outbuffer + offset + 0) = (u_pred_pos_horiz_abs_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->pred_pos_horiz_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_gps_glitch_status_flag;
      u_gps_glitch_status_flag.real = this->gps_glitch_status_flag;
      *(outbuffer + offset + 0) = (u_gps_glitch_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gps_glitch_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_accel_error_status_flag;
      u_accel_error_status_flag.real = this->accel_error_status_flag;
      *(outbuffer + offset + 0) = (u_accel_error_status_flag.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->accel_error_status_flag);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        bool real;
        uint8_t base;
      } u_attitude_status_flag;
      u_attitude_status_flag.base = 0;
      u_attitude_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->attitude_status_flag = u_attitude_status_flag.real;
      offset += sizeof(this->attitude_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_velocity_horiz_status_flag;
      u_velocity_horiz_status_flag.base = 0;
      u_velocity_horiz_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->velocity_horiz_status_flag = u_velocity_horiz_status_flag.real;
      offset += sizeof(this->velocity_horiz_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_velocity_vert_status_flag;
      u_velocity_vert_status_flag.base = 0;
      u_velocity_vert_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->velocity_vert_status_flag = u_velocity_vert_status_flag.real;
      offset += sizeof(this->velocity_vert_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_horiz_rel_status_flag;
      u_pos_horiz_rel_status_flag.base = 0;
      u_pos_horiz_rel_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pos_horiz_rel_status_flag = u_pos_horiz_rel_status_flag.real;
      offset += sizeof(this->pos_horiz_rel_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_horiz_abs_status_flag;
      u_pos_horiz_abs_status_flag.base = 0;
      u_pos_horiz_abs_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pos_horiz_abs_status_flag = u_pos_horiz_abs_status_flag.real;
      offset += sizeof(this->pos_horiz_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_vert_abs_status_flag;
      u_pos_vert_abs_status_flag.base = 0;
      u_pos_vert_abs_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pos_vert_abs_status_flag = u_pos_vert_abs_status_flag.real;
      offset += sizeof(this->pos_vert_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pos_vert_agl_status_flag;
      u_pos_vert_agl_status_flag.base = 0;
      u_pos_vert_agl_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pos_vert_agl_status_flag = u_pos_vert_agl_status_flag.real;
      offset += sizeof(this->pos_vert_agl_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_const_pos_mode_status_flag;
      u_const_pos_mode_status_flag.base = 0;
      u_const_pos_mode_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->const_pos_mode_status_flag = u_const_pos_mode_status_flag.real;
      offset += sizeof(this->const_pos_mode_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pred_pos_horiz_rel_status_flag;
      u_pred_pos_horiz_rel_status_flag.base = 0;
      u_pred_pos_horiz_rel_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pred_pos_horiz_rel_status_flag = u_pred_pos_horiz_rel_status_flag.real;
      offset += sizeof(this->pred_pos_horiz_rel_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_pred_pos_horiz_abs_status_flag;
      u_pred_pos_horiz_abs_status_flag.base = 0;
      u_pred_pos_horiz_abs_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->pred_pos_horiz_abs_status_flag = u_pred_pos_horiz_abs_status_flag.real;
      offset += sizeof(this->pred_pos_horiz_abs_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_gps_glitch_status_flag;
      u_gps_glitch_status_flag.base = 0;
      u_gps_glitch_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->gps_glitch_status_flag = u_gps_glitch_status_flag.real;
      offset += sizeof(this->gps_glitch_status_flag);
      union {
        bool real;
        uint8_t base;
      } u_accel_error_status_flag;
      u_accel_error_status_flag.base = 0;
      u_accel_error_status_flag.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->accel_error_status_flag = u_accel_error_status_flag.real;
      offset += sizeof(this->accel_error_status_flag);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/EstimatorStatus"; };
    virtual const char * getMD5() override { return "39dbcc4be3f04b68422f784827c47dd5"; };

  };

}
#endif
