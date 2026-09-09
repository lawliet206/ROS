#ifndef _ROS_mavros_msgs_Mavlink_h
#define _ROS_mavros_msgs_Mavlink_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace mavros_msgs
{

  class Mavlink : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _framing_status_type;
      _framing_status_type framing_status;
      typedef uint8_t _magic_type;
      _magic_type magic;
      typedef uint8_t _len_type;
      _len_type len;
      typedef uint8_t _incompat_flags_type;
      _incompat_flags_type incompat_flags;
      typedef uint8_t _compat_flags_type;
      _compat_flags_type compat_flags;
      typedef uint8_t _seq_type;
      _seq_type seq;
      typedef uint8_t _sysid_type;
      _sysid_type sysid;
      typedef uint8_t _compid_type;
      _compid_type compid;
      typedef uint32_t _msgid_type;
      _msgid_type msgid;
      typedef uint16_t _checksum_type;
      _checksum_type checksum;
      uint32_t payload64_length;
      typedef uint64_t _payload64_type;
      _payload64_type st_payload64;
      _payload64_type * payload64;
      uint32_t signature_length;
      typedef uint8_t _signature_type;
      _signature_type st_signature;
      _signature_type * signature;
      enum { FRAMING_OK =  1 };
      enum { FRAMING_BAD_CRC =  2 };
      enum { FRAMING_BAD_SIGNATURE =  3 };
      enum { MAVLINK_V10 =  254 };
      enum { MAVLINK_V20 =  253 };

    Mavlink():
      header(),
      framing_status(0),
      magic(0),
      len(0),
      incompat_flags(0),
      compat_flags(0),
      seq(0),
      sysid(0),
      compid(0),
      msgid(0),
      checksum(0),
      payload64_length(0), st_payload64(), payload64(nullptr),
      signature_length(0), st_signature(), signature(nullptr)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->framing_status >> (8 * 0)) & 0xFF;
      offset += sizeof(this->framing_status);
      *(outbuffer + offset + 0) = (this->magic >> (8 * 0)) & 0xFF;
      offset += sizeof(this->magic);
      *(outbuffer + offset + 0) = (this->len >> (8 * 0)) & 0xFF;
      offset += sizeof(this->len);
      *(outbuffer + offset + 0) = (this->incompat_flags >> (8 * 0)) & 0xFF;
      offset += sizeof(this->incompat_flags);
      *(outbuffer + offset + 0) = (this->compat_flags >> (8 * 0)) & 0xFF;
      offset += sizeof(this->compat_flags);
      *(outbuffer + offset + 0) = (this->seq >> (8 * 0)) & 0xFF;
      offset += sizeof(this->seq);
      *(outbuffer + offset + 0) = (this->sysid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->sysid);
      *(outbuffer + offset + 0) = (this->compid >> (8 * 0)) & 0xFF;
      offset += sizeof(this->compid);
      *(outbuffer + offset + 0) = (this->msgid >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->msgid >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->msgid >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->msgid >> (8 * 3)) & 0xFF;
      offset += sizeof(this->msgid);
      *(outbuffer + offset + 0) = (this->checksum >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->checksum >> (8 * 1)) & 0xFF;
      offset += sizeof(this->checksum);
      *(outbuffer + offset + 0) = (this->payload64_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->payload64_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->payload64_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->payload64_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->payload64_length);
      for( uint32_t i = 0; i < payload64_length; i++){
      *(outbuffer + offset + 0) = (this->payload64[i] >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->payload64[i] >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->payload64[i] >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->payload64[i] >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (this->payload64[i] >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (this->payload64[i] >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (this->payload64[i] >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (this->payload64[i] >> (8 * 7)) & 0xFF;
      offset += sizeof(this->payload64[i]);
      }
      *(outbuffer + offset + 0) = (this->signature_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->signature_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->signature_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->signature_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->signature_length);
      for( uint32_t i = 0; i < signature_length; i++){
      *(outbuffer + offset + 0) = (this->signature[i] >> (8 * 0)) & 0xFF;
      offset += sizeof(this->signature[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->framing_status =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->framing_status);
      this->magic =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->magic);
      this->len =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->len);
      this->incompat_flags =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->incompat_flags);
      this->compat_flags =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->compat_flags);
      this->seq =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->seq);
      this->sysid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->sysid);
      this->compid =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->compid);
      this->msgid =  ((uint32_t) (*(inbuffer + offset)));
      this->msgid |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->msgid |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->msgid |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->msgid);
      this->checksum =  ((uint16_t) (*(inbuffer + offset)));
      this->checksum |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      offset += sizeof(this->checksum);
      uint32_t payload64_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      payload64_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      payload64_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      payload64_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->payload64_length);
      if(payload64_lengthT > payload64_length)
        this->payload64 = (uint64_t*)realloc(this->payload64, payload64_lengthT * sizeof(uint64_t));
      payload64_length = payload64_lengthT;
      for( uint32_t i = 0; i < payload64_length; i++){
      this->st_payload64 =  ((uint64_t) (*(inbuffer + offset)));
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      this->st_payload64 |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      offset += sizeof(this->st_payload64);
        memcpy( &(this->payload64[i]), &(this->st_payload64), sizeof(uint64_t));
      }
      uint32_t signature_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      signature_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      signature_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      signature_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->signature_length);
      if(signature_lengthT > signature_length)
        this->signature = (uint8_t*)realloc(this->signature, signature_lengthT * sizeof(uint8_t));
      signature_length = signature_lengthT;
      for( uint32_t i = 0; i < signature_length; i++){
      this->st_signature =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->st_signature);
        memcpy( &(this->signature[i]), &(this->st_signature), sizeof(uint8_t));
      }
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/Mavlink"; };
    virtual const char * getMD5() override { return "41093e1fd0f3eea1da2aa33a177e5ba6"; };

  };

}
#endif
