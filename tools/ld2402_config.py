#!/usr/bin/env python3
"""
ld2402_config.py — HLK-LD2402 毫米波雷达配置工具
=================================================
功能: 读取/设置灵敏度、探测距离、各门限阈值
用法: python3 ld2402_config.py <命令> [参数]

命令:
  read             读取当前所有配置
  set-sens <0-100> 设置运动灵敏度 (所有门)
  set-range <m>    设置最远探测距离 (0.75-6m)
  set-static <0-100> 设置静态灵敏度

示例:
  python3 ld2402_config.py COM6 read
  python3 ld2402_config.py COM6 set-sens 60
  python3 ld2402_config.py COM6 set-range 3
"""
import sys
import struct
import time

try:
    import serial
except ImportError:
    print("请先安装 pyserial: pip install pyserial")
    sys.exit(1)

# ====== 协议常量 ======
FRAME_HEADER = b'\xFD\xFC\xFB\xFA'
FRAME_FOOTER = b'\x04\x03\x02\x01'
CMD_ENTER_CONFIG = 0x00FF
CMD_EXIT_CONFIG = 0x00FE
CMD_READ_PARAM = 0x0011
CMD_WRITE_PARAM = 0x0012


def make_frame(cmd, data=b''):
    length = 2 + len(data)
    return FRAME_HEADER + struct.pack('<H', length) + struct.pack('<H', cmd) + data + FRAME_FOOTER


def read_response(ser, timeout=0.5):
    resp = b''
    end = time.time() + timeout
    while time.time() < end:
        if ser.in_waiting:
            resp += ser.read(ser.in_waiting)
        if len(resp) >= 4 and resp[-4:] == FRAME_FOOTER:
            break
        time.sleep(0.01)
    return resp


def send_cmd(ser, cmd, data=b'', desc=''):
    frame = make_frame(cmd, data)
    ser.reset_input_buffer()
    ser.write(frame)
    time.sleep(0.3)
    resp = read_response(ser)
    if resp:
        ack = resp[8:10] if len(resp) > 10 else b'--'
        ok = ack == b'\x00\x00'
        return ok, resp
    return False, b''


def enter_config(ser):
    ok, resp = send_cmd(ser, CMD_ENTER_CONFIG, desc='enter_config')
    print('  进入配置模式: %s' % ('OK' if ok else '失败'))
    return ok


def exit_config(ser):
    ok, resp = send_cmd(ser, CMD_EXIT_CONFIG, desc='exit_config')
    print('  退出配置模式: %s' % ('OK' if ok else '失败'))
    return ok


def read_param(ser, pid, name=''):
    ok, resp = send_cmd(ser, CMD_READ_PARAM, struct.pack('<H', pid))
    if ok and len(resp) > 12:
        # 解析响应: header(4) + len(2) + cmd(2) + ack(2) + data_len(2) + data(N) + footer(4)
        data_len = struct.unpack('<H', resp[10:12])[0]
        data = resp[12:12+data_len]
        print('  %s (0x%04X): %s' % (name, pid, data.hex()))
        return data
    else:
        print('  %s (0x%04X): 读取失败' % (name, pid))
        return None


def cmd_read(ser):
    if not enter_config(ser):
        return
    print('\n--- 当前配置 ---')
    read_param(ser, 0x0001, '最大探测门数')
    read_param(ser, 0x0002, '保留')
    read_param(ser, 0x0003, '运动灵敏度 (逐门)')
    read_param(ser, 0x0004, '静态灵敏度 (逐门)')
    read_param(ser, 0x0005, '无人超时')
    read_param(ser, 0x0006, '运动判断阈值')
    read_param(ser, 0x0007, '静态判断阈值')
    exit_config(ser)


def cmd_set_sensitivity(ser, value):
    """设置运动灵敏度 (所有门统一)"""
    value = max(0, min(100, value))
    if not enter_config(ser):
        return
    # 参数 0x0003: 运动灵敏度, 每个门1字节, 0xFF=所有门
    ok, resp = send_cmd(ser, CMD_WRITE_PARAM,
                        struct.pack('<H', 0x0003) + struct.pack('<H', 0xFF) + struct.pack('<B', value) + b'\x00' * 8)
    print('  设置运动灵敏度=%d: %s' % (value, 'OK' if ok else '失败'))
    exit_config(ser)


def cmd_set_static_sensitivity(ser, value):
    """设置静态灵敏度"""
    value = max(0, min(100, value))
    if not enter_config(ser):
        return
    ok, resp = send_cmd(ser, CMD_WRITE_PARAM,
                        struct.pack('<H', 0x0004) + struct.pack('<H', 0xFF) + struct.pack('<B', value) + b'\x00' * 8)
    print('  设置静态灵敏度=%d: %s' % (value, 'OK' if ok else '失败'))
    exit_config(ser)


def cmd_set_range(ser, meters):
    """设置最大探测距离"""
    gates = max(1, min(8, int(meters / 0.75)))
    if not enter_config(ser):
        return
    ok, resp = send_cmd(ser, CMD_WRITE_PARAM,
                        struct.pack('<H', 0x0001) + struct.pack('<B', gates) + b'\x00' * 9)
    print('  设置最大探测门数=%d (约%.1fm): %s' % (gates, gates * 0.75, 'OK' if ok else '失败'))
    exit_config(ser)


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)

    port = sys.argv[1]
    cmd = sys.argv[2]

    try:
        ser = serial.Serial(port, 115200, timeout=0.1)
        print('已打开 %s @ 115200' % port)
    except Exception as e:
        print('无法打开 %s: %s' % (port, e))
        sys.exit(1)

    time.sleep(0.5)
    ser.reset_input_buffer()

    if cmd == 'read':
        cmd_read(ser)
    elif cmd == 'set-sens' and len(sys.argv) >= 4:
        cmd_set_sensitivity(ser, int(sys.argv[3]))
    elif cmd == 'set-static' and len(sys.argv) >= 4:
        cmd_set_static_sensitivity(ser, int(sys.argv[3]))
    elif cmd == 'set-range' and len(sys.argv) >= 4:
        cmd_set_range(ser, float(sys.argv[3]))
    else:
        print('未知命令: %s' % cmd)
        print(__doc__)

    ser.close()
