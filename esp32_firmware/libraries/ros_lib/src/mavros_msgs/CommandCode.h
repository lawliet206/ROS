#ifndef _ROS_mavros_msgs_CommandCode_h
#define _ROS_mavros_msgs_CommandCode_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace mavros_msgs
{

  class CommandCode : public ros::Msg
  {
    public:
      enum { AIRFRAME_CONFIGURATION =  2520 };
      enum { ARM_AUTHORIZATION_REQUEST =  3001             };
      enum { CAMERA_TRACK_POINT =  2004                    };
      enum { CAMERA_TRACK_RECTANGLE =  2005                };
      enum { CAMERA_STOP_TRACKING =  2010                  };
      enum { CAN_FORWARD =  32000                          };
      enum { COMPONENT_ARM_DISARM =  400                   };
      enum { CONDITION_DELAY =  112                        };
      enum { CONDITION_CHANGE_ALT =  113                   };
      enum { CONDITION_DISTANCE =  114                     };
      enum { CONDITION_YAW =  115                          };
      enum { CONDITION_LAST =  159                         };
      enum { CONTROL_HIGH_LATENCY =  2600                  };
      enum { DO_FOLLOW =  32                               };
      enum { DO_FOLLOW_REPOSITION =  33                    };
      enum { DO_SET_MODE =  176                            };
      enum { DO_JUMP =  177                                };
      enum { DO_CHANGE_SPEED =  178                        };
      enum { DO_SET_HOME =  179                            };
      enum { DO_SET_PARAMETER =  180                       };
      enum { DO_SET_RELAY =  181                           };
      enum { DO_REPEAT_RELAY =  182                        };
      enum { DO_SET_SERVO =  183                           };
      enum { DO_REPEAT_SERVO =  184                        };
      enum { DO_FLIGHTTERMINATION =  185                   };
      enum { DO_CHANGE_ALTITUDE =  186                     };
      enum { DO_LAND_START =  189                          };
      enum { DO_RALLY_LAND =  190                          };
      enum { DO_GO_AROUND =  191                           };
      enum { DO_REPOSITION =  192                          };
      enum { DO_PAUSE_CONTINUE =  193                      };
      enum { DO_SET_REVERSE =  194                         };
      enum { DO_SET_ROI_LOCATION =  195                    };
      enum { DO_SET_ROI_WPNEXT_OFFSET =  196               };
      enum { DO_SET_ROI_NONE =  197                        };
      enum { DO_SET_ROI_SYSID =  198                       };
      enum { DO_CONTROL_VIDEO =  200                       };
      enum { DO_SET_ROI =  201                             };
      enum { DO_DIGICAM_CONFIGURE =  202                   };
      enum { DO_DIGICAM_CONTROL =  203                     };
      enum { DO_MOUNT_CONFIGURE =  204                     };
      enum { DO_MOUNT_CONTROL =  205                       };
      enum { DO_SET_CAM_TRIGG_DIST =  206                  };
      enum { DO_FENCE_ENABLE =  207                        };
      enum { DO_PARACHUTE =  208                           };
      enum { DO_MOTOR_TEST =  209                          };
      enum { DO_INVERTED_FLIGHT =  210                     };
      enum { DO_GRIPPER =  211                             };
      enum { DO_AUTOTUNE_ENABLE =  212                     };
      enum { DO_SET_CAM_TRIGG_INTERVAL =  214              };
      enum { DO_MOUNT_CONTROL_QUAT =  220                  };
      enum { DO_GUIDED_MASTER =  221                       };
      enum { DO_GUIDED_LIMITS =  222                       };
      enum { DO_ENGINE_CONTROL =  223                      };
      enum { DO_SET_MISSION_CURRENT =  224                 };
      enum { DO_LAST =  240                                };
      enum { DO_JUMP_TAG =  601                            };
      enum { DO_GIMBAL_MANAGER_PITCHYAW =  1000            };
      enum { DO_GIMBAL_MANAGER_CONFIGURE =  1001           };
      enum { DO_TRIGGER_CONTROL =  2003                    };
      enum { DO_VTOL_TRANSITION =  3000                    };
      enum { DO_ADSB_OUT_IDENT =  10001                    };
      enum { DO_WINCH =  42600                             };
      enum { FIXED_MAG_CAL_YAW =  42006                    };
      enum { GET_HOME_POSITION =  410                      };
      enum { GET_MESSAGE_INTERVAL =  510                   };
      enum { IMAGE_START_CAPTURE =  2000                   };
      enum { IMAGE_STOP_CAPTURE =  2001                    };
      enum { JUMP_TAG =  600                               };
      enum { LOGGING_START =  2510                         };
      enum { LOGGING_STOP =  2511                          };
      enum { MISSION_START =  300                          };
      enum { NAV_WAYPOINT =  16                            };
      enum { NAV_LOITER_UNLIM =  17                        };
      enum { NAV_LOITER_TURNS =  18                        };
      enum { NAV_LOITER_TIME =  19                         };
      enum { NAV_RETURN_TO_LAUNCH =  20                    };
      enum { NAV_LAND =  21                                };
      enum { NAV_TAKEOFF =  22                             };
      enum { NAV_LAND_LOCAL =  23                          };
      enum { NAV_TAKEOFF_LOCAL =  24                       };
      enum { NAV_FOLLOW =  25                              };
      enum { NAV_CONTINUE_AND_CHANGE_ALT =  30             };
      enum { NAV_LOITER_TO_ALT =  31                       };
      enum { NAV_ROI =  80                                 };
      enum { NAV_PATHPLANNING =  81                        };
      enum { NAV_SPLINE_WAYPOINT =  82                     };
      enum { NAV_VTOL_TAKEOFF =  84                        };
      enum { NAV_VTOL_LAND =  85                           };
      enum { NAV_GUIDED_ENABLE =  92                       };
      enum { NAV_DELAY =  93                               };
      enum { NAV_PAYLOAD_PLACE =  94                       };
      enum { NAV_LAST =  95                                };
      enum { NAV_SET_YAW_SPEED =  213                      };
      enum { NAV_FENCE_RETURN_POINT =  5000                };
      enum { NAV_FENCE_POLYGON_VERTEX_INCLUSION =  5001    };
      enum { NAV_FENCE_POLYGON_VERTEX_EXCLUSION =  5002    };
      enum { NAV_FENCE_CIRCLE_INCLUSION =  5003            };
      enum { NAV_FENCE_CIRCLE_EXCLUSION =  5004            };
      enum { NAV_RALLY_POINT =  5100                       };
      enum { OBLIQUE_SURVEY =  260                         };
      enum { OVERRIDE_GOTO =  252                          };
      enum { PANORAMA_CREATE =  2800                       };
      enum { PAYLOAD_PREPARE_DEPLOY =  30001               };
      enum { PAYLOAD_CONTROL_DEPLOY =  30002               };
      enum { PREFLIGHT_CALIBRATION =  241                  };
      enum { PREFLIGHT_SET_SENSOR_OFFSETS =  242           };
      enum { PREFLIGHT_UAVCAN =  243                       };
      enum { PREFLIGHT_STORAGE =  245                      };
      enum { PREFLIGHT_REBOOT_SHUTDOWN =  246              };
      enum { REQUEST_MESSAGE =  512                        };
      enum { REQUEST_PROTOCOL_VERSION =  519               };
      enum { REQUEST_AUTOPILOT_CAPABILITIES =  520         };
      enum { REQUEST_CAMERA_INFORMATION =  521             };
      enum { REQUEST_CAMERA_SETTINGS =  522                };
      enum { REQUEST_STORAGE_INFORMATION =  525            };
      enum { REQUEST_CAMERA_CAPTURE_STATUS =  527          };
      enum { REQUEST_FLIGHT_INFORMATION =  528             };
      enum { REQUEST_VIDEO_STREAM_INFORMATION =  2504      };
      enum { REQUEST_VIDEO_STREAM_STATUS =  2505           };
      enum { RESET_CAMERA_SETTINGS =  529                  };
      enum { RUN_PREARM_CHECKS =  401                      };
      enum { SET_MESSAGE_INTERVAL =  511                   };
      enum { SET_CAMERA_MODE =  530                        };
      enum { SET_CAMERA_ZOOM =  531                        };
      enum { SET_CAMERA_FOCUS =  532                       };
      enum { SET_GUIDED_SUBMODE_STANDARD =  4000           };
      enum { SET_GUIDED_SUBMODE_CIRCLE =  4001             };
      enum { START_RX_PAIR =  500                          };
      enum { STORAGE_FORMAT =  526                         };
      enum { UAVCAN_GET_NODE_INFO =  5200                  };
      enum { VIDEO_START_CAPTURE =  2500                   };
      enum { VIDEO_STOP_CAPTURE =  2501                    };
      enum { VIDEO_START_STREAMING =  2502                 };
      enum { VIDEO_STOP_STREAMING =  2503                  };

    CommandCode()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return "mavros_msgs/CommandCode"; };
    virtual const char * getMD5() override { return "059d72cceadaa0abca57de773508431c"; };

  };

}
#endif
