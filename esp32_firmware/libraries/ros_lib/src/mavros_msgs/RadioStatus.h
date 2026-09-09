#ifndef _ROS_mavros_msgs_RadioStatus_h
#define _ROS_mavros_msgs_RadioStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class RadioStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _rssi_type;
      _rssi_type rssi;
      typedef uint8_t _remrssi_type;
      _remrssi_type remrssi;
      typedef uint8_t _txbuf_type;
      _txbuf_type txbuf;
      typedef uint8_t _noise_type;
      _noise_type noise;
      typedef uint8_t _remnoise_type;
      _remnoise_type remnoise;
      typedef uint16_t _rxerrors_type;
      _rxerrors_type rxerrors;
      typedef uint16_t _fixed_type;
      _fixed_type fixed;
      typedef float _rssi_dbm_type;
      _rssi_dbm_type rssi_dbm;
      typedef float _remrssi_dbm_type;
      _remrssi_dbm_type remrssi_dbm;

    RadioStatus():
      header(),
      rssi(0),
      remrssi(0),
      txbuf(0),
      noise(0),
      remnoise(0),
      rxerrors(0),
      fixed(0),
      rssi_dbm(0),
      remrssi_dbm(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->rssi >> (8 * 0)) & 0xFF;
      offset += sizeof(this->rssi);
      *(outbuffer + offset + 0) = (this->remrssi >> (8 * 0)) & 0xFF;
      offset += sizeof(this->remrssi);
      *(outbuffer + offset + 0) = (this->txbuf >> (8 * 0)) & 0xFF;
      offset += sizeof(this->txbuf);
      *(outbuffer + offset + 0) = (this->noise >> (8 * 0)) & 0xFF;
      offset += sizeof(this->noise);
      *(outbuffer + offset + 0) = (this->remnoise >> (8 * 0)) & 0xFF;
      offset += sizeof(this->remnoise);
      *(outbuffer + offset + 0) = (this->rxerrors >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->rxerrors >> (8 * 1)) & 0xFF;
      offset += sizeof(this->rxerrors);
      *(outbuffer + offset + 0) = (this->fixed >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->fixed >> (8 * 1)) & 0xFF;
      offset += sizeof(this->fixed);
      union {
        float real;
        uint32_t base;
      } u_rssi_dbm;
      u_rssi_dbm.real = this->rssi_dbm;
      *(outbuffer + offset + 0) = (u_rssi_dbm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rssi_dbm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rssi_dbm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rssi_dbm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rssi_dbm);
      union {
        float real;
        uint32_t base;
      } u_remrssi_dbm;
      u_remrssi_dbm.real = this->remrssi_dbm;
      *(outbuffer + offset + 0) = (u_remrssi_dbm.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_remrssi_dbm.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_remrssi_dbm.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_remrssi_dbm.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->remrssi_dbm);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->rssi =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->rssi);
      this->remrssi =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->remrssi);
      this->txbuf =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->txbuf);
      this->noise =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->noise);
      this->remnoise =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->remnoise);
      this->rxerrors =  ((uint16_t) (*(inbuffer + offset)));
      this->rxerrors |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->rxerrors);
      this->fixed =  ((uint16_t) (*(inbuffer + offset)));
      this->fixed |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->fixed);
      union {
        float real;
        uint32_t base;
      } u_rssi_dbm;
      u_rssi_dbm.base = 0;
      u_rssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rssi_dbm = u_rssi_dbm.real;
      offset += sizeof(this->rssi_dbm);
      union {
        float real;
        uint32_t base;
      } u_remrssi_dbm;
      u_remrssi_dbm.base = 0;
      u_remrssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_remrssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_remrssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_remrssi_dbm.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->remrssi_dbm = u_remrssi_dbm.real;
      offset += sizeof(this->remrssi_dbm);
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/RadioStatus"; };
    virtual const char * getMD5() override { return "04e4a879bb2687140da50a1a821e2190"; };

  };

}
#endif
