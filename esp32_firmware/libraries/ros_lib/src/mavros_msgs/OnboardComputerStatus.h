#ifndef _ROS_mavros_msgs_OnboardComputerStatus_h
#define _ROS_mavros_msgs_OnboardComputerStatus_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class OnboardComputerStatus : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _component_type;
      _component_type component;
      typedef uint32_t _uptime_type;
      _uptime_type uptime;
      typedef uint8_t _type_type;
      _type_type type;
      uint8_t cpu_cores[8];
      uint8_t cpu_combined[10];
      uint8_t gpu_cores[4];
      uint8_t gpu_combined[10];
      typedef int8_t _temperature_board_type;
      _temperature_board_type temperature_board;
      int8_t temperature_core[8];
      int16_t fan_speed[4];
      typedef uint32_t _ram_usage_type;
      _ram_usage_type ram_usage;
      typedef uint32_t _ram_total_type;
      _ram_total_type ram_total;
      uint32_t storage_type[4];
      uint32_t storage_usage[4];
      uint32_t storage_total[4];
      uint32_t link_type[6];
      uint32_t link_tx_rate[6];
      uint32_t link_rx_rate[6];
      uint32_t link_tx_max[6];
      uint32_t link_rx_max[6];

    OnboardComputerStatus():
      header(),
      component(0),
      uptime(0),
      type(0),
      cpu_cores(),
      cpu_combined(),
      gpu_cores(),
      gpu_combined(),
      temperature_board(0),
      temperature_core(),
      fan_speed(),
      ram_usage(0),
      ram_total(0),
      storage_type(),
      storage_usage(),
      storage_total(),
      link_type(),
      link_tx_rate(),
      link_rx_rate(),
      link_tx_max(),
      link_rx_max()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->component >> (8 * 0)) & 0xFF;
      offset += sizeof(this->component);
      *(outbuffer + offset + 0) = (this->uptime >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->uptime >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->uptime >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->uptime >> (8 * 3)) & 0xFF;
      offset += sizeof(this->uptime);
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      for( uint32_t i = 0; i < 8; i++){
      *(outbuffer + offset + 0) = (this->cpu_cores[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cpu_cores[i]);
      }
      for( uint32_t i = 0; i < 10; i++){
      *(outbuffer + offset + 0) = (this->cpu_combined[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cpu_combined[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      *(outbuffer + offset + 0) = (this->gpu_cores[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gpu_cores[i]);
      }
      for( uint32_t i = 0; i < 10; i++){
      *(outbuffer + offset + 0) = (this->gpu_combined[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->gpu_combined[i]);
      }
      union {
        int8_t real;
        uint8_t base;
      } u_temperature_board;
      u_temperature_board.real = this->temperature_board;
      *(outbuffer + offset + 0) = (u_temperature_board.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->temperature_board);
      for( uint32_t i = 0; i < 8; i++){
      union {
        int8_t real;
        uint8_t base;
      } u_temperature_corei;
      u_temperature_corei.real = this->temperature_core[i];
      *(outbuffer + offset + 0) = (u_temperature_corei.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->temperature_core[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      union {
        int16_t real;
        uint16_t base;
      } u_fan_speedi;
      u_fan_speedi.real = this->fan_speed[i];
      *(outbuffer + offset + 0) = (u_fan_speedi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_fan_speedi.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->fan_speed[i]);
      }
      *(outbuffer + offset + 0) = (this->ram_usage >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ram_usage >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ram_usage >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ram_usage >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ram_usage);
      *(outbuffer + offset + 0) = (this->ram_total >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->ram_total >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->ram_total >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->ram_total >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ram_total);
      for( uint32_t i = 0; i < 4; i++){
      *(outbuffer + offset + 0) = (this->storage_type[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->storage_type[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->storage_type[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->storage_type[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->storage_type[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      *(outbuffer + offset + 0) = (this->storage_usage[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->storage_usage[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->storage_usage[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->storage_usage[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->storage_usage[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      *(outbuffer + offset + 0) = (this->storage_total[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->storage_total[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->storage_total[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->storage_total[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->storage_total[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      *(outbuffer + offset + 0) = (this->link_type[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_type[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_type[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_type[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_type[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      *(outbuffer + offset + 0) = (this->link_tx_rate[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_tx_rate[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_tx_rate[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_tx_rate[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_tx_rate[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      *(outbuffer + offset + 0) = (this->link_rx_rate[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_rx_rate[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_rx_rate[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_rx_rate[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_rx_rate[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      *(outbuffer + offset + 0) = (this->link_tx_max[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_tx_max[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_tx_max[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_tx_max[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_tx_max[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      *(outbuffer + offset + 0) = (this->link_rx_max[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->link_rx_max[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->link_rx_max[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->link_rx_max[i] >> (8 * 3)) & 0xFF;
      offset += sizeof(this->link_rx_max[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->component =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->component);
      this->uptime =  ((uint32_t) (*(inbuffer + offset)));
      this->uptime |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->uptime |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->uptime |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->uptime);
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      for( uint32_t i = 0; i < 8; i++){
      this->cpu_cores[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->cpu_cores[i]);
      }
      for( uint32_t i = 0; i < 10; i++){
      this->cpu_combined[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->cpu_combined[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      this->gpu_cores[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->gpu_cores[i]);
      }
      for( uint32_t i = 0; i < 10; i++){
      this->gpu_combined[i] =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->gpu_combined[i]);
      }
      union {
        int8_t real;
        uint8_t base;
      } u_temperature_board;
      u_temperature_board.base = 0;
      u_temperature_board.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->temperature_board = u_temperature_board.real;
      offset += sizeof(this->temperature_board);
      for( uint32_t i = 0; i < 8; i++){
      union {
        int8_t real;
        uint8_t base;
      } u_temperature_corei;
      u_temperature_corei.base = 0;
      u_temperature_corei.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->temperature_core[i] = u_temperature_corei.real;
      offset += sizeof(this->temperature_core[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      union {
        int16_t real;
        uint16_t base;
      } u_fan_speedi;
      u_fan_speedi.base = 0;
      u_fan_speedi.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_fan_speedi.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->fan_speed[i] = u_fan_speedi.real;
      offset += sizeof(this->fan_speed[i]);
      }
      this->ram_usage =  ((uint32_t) (*(inbuffer + offset)));
      this->ram_usage |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ram_usage |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ram_usage |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ram_usage);
      this->ram_total =  ((uint32_t) (*(inbuffer + offset)));
      this->ram_total |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->ram_total |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->ram_total |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->ram_total);
      for( uint32_t i = 0; i < 4; i++){
      this->storage_type[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->storage_type[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->storage_type[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->storage_type[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->storage_type[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      this->storage_usage[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->storage_usage[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->storage_usage[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->storage_usage[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->storage_usage[i]);
      }
      for( uint32_t i = 0; i < 4; i++){
      this->storage_total[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->storage_total[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->storage_total[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->storage_total[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->storage_total[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      this->link_type[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->link_type[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_type[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_type[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_type[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      this->link_tx_rate[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->link_tx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_tx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_tx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_tx_rate[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      this->link_rx_rate[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->link_rx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_rx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_rx_rate[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_rx_rate[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      this->link_tx_max[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->link_tx_max[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_tx_max[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_tx_max[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_tx_max[i]);
      }
      for( uint32_t i = 0; i < 6; i++){
      this->link_rx_max[i] =  ((uint32_t) (*(inbuffer + offset)));
      this->link_rx_max[i] |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->link_rx_max[i] |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->link_rx_max[i] |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->link_rx_max[i]);
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/OnboardComputerStatus"; };
    virtual const char * getMD5() override { return "aebe864fac2952ca9de45a2b65875a60"; };

  };

}
#endif
