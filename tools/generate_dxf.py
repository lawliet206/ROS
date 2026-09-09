#!/usr/bin/env python3
"""
生成两层亚克力底盘 DXF — 下层(机械) + 上层(电子)
孔径: M3螺丝=3.5mm  走线=10mm  USB=15mm
"""
FILENAME = "chassis_2layer.dxf"

H_M3   = 1.75   # M3 螺丝 3.5mm 直径
H_WIRE = 5.0    # 走线孔 10mm
H_USB  = 7.5    # USB 孔 15mm

# ===== 下层板: 200×160mm, 放电机+万向轮+电池 =====
LO_W, LO_H = 200, 160

# ===== 上层板: 180×140mm, 放电控+雷达 =====
UP_W, UP_H = 180, 140

# 拼板间距
GAP = 20

import math

def commit(entities):
    global all_entities
    all_entities += entities

# ===== DXF 原语 (Y 不翻转, 店家自己认方向) =====
def header():
    return "0\nSECTION\n2\nHEADER\n9\n$ACADVER\n1\nAC1009\n0\nENDSEC\n0\nSECTION\n2\nTABLES\n0\nENDSEC\n0\nSECTION\n2\nBLOCKS\n0\nENDSEC\n0\nSECTION\n2\nENTITIES\n"

def footer():
    return "0\nENDSEC\n0\nEOF\n"

def circ(x, y, r):
    return f"0\nCIRCLE\n8\n0\n10\n{x:.1f}\n20\n{y:.1f}\n40\n{r:.1f}\n"

def line(x1, y1, x2, y2):
    return f"0\nLINE\n8\n0\n10\n{x1:.1f}\n20\n{y1:.1f}\n11\n{x2:.1f}\n21\n{y2:.1f}\n"

def arc(cx, cy, r, a1, a2):
    return f"0\nARC\n8\n0\n10\n{cx:.1f}\n20\n{cy:.1f}\n40\n{r:.1f}\n50\n{a1:.1f}\n51\n{a2:.1f}\n"

def poly_closed(pts):
    n = len(pts)
    h = f"0\nPOLYLINE\n8\n0\n66\n1\n70\n1\n"
    v = ""
    for x, y in pts:
        v += f"0\nVERTEX\n8\n0\n10\n{x:.1f}\n20\n{y:.1f}\n"
    return h + v + "0\nSEQEND\n"

def rect_rounded(w, h, r, off_x=0, off_y=0):
    """圆角矩形外框"""
    s = ""
    s += line(off_x+r, off_y, off_x+w-r, off_y)
    s += arc(off_x+r, off_y+r, r, 180, 270)
    s += line(off_x, off_y+r, off_x, off_y+h-r)
    s += arc(off_x+r, off_y+h-r, r, 90, 180)
    s += line(off_x+r, off_y+h, off_x+w-r, off_y+h)
    s += arc(off_x+w-r, off_y+h-r, r, 0, 90)
    s += line(off_x+w, off_y+r, off_x+w, off_y+h-r)
    s += arc(off_x+w-r, off_y+r, r, 270, 360)
    return s

all_entities = ""

# ========== 下层板 ==========
off_x, off_y = 0, 0
w, h = LO_W, LO_H
cx = off_x + w/2   # X 中心 = 100
cy = off_y + h/2   # Y 中心 = 80

commit(rect_rounded(w, h, 8, off_x, off_y))

# --- 左电机 M3 (4 孔, 29mm 正方, Y≈26) ---
for dx, dy in [(-14.5, -14.5), (14.5, -14.5), (-14.5, 14.5), (14.5, 14.5)]:
    commit(circ(cx+dx, 26+dy, H_M3))

# --- 右电机 M3 (4 孔, Y≈134) ---
for dx, dy in [(-14.5, -14.5), (14.5, -14.5), (-14.5, 14.5), (14.5, 14.5)]:
    commit(circ(cx+dx, 134+dy, H_M3))

# --- 前牛眼轮 (2 孔, M3, X≈165, 间距 20mm) ---
commit(circ(165, cy-10, H_M3))
commit(circ(165, cy+10, H_M3))

# --- 后牛眼轮 (2 孔, M3, X≈25, 间距 20mm) ---
commit(circ(25, cy-10, H_M3))
commit(circ(25, cy+10, H_M3))

# --- 电池魔术贴槽 (2 条 12×90mm, 板子中后部) ---
commit(poly_closed([(68, 35), (80, 35), (80, 125), (68, 125)]))
commit(poly_closed([(120, 35), (132, 35), (132, 125), (120, 125)]))

# --- 走线孔 (电机线穿到上层) ---
commit(circ(cx, 50, H_WIRE))   # 左电机走线
commit(circ(cx, 110, H_WIRE))  # 右电机走线
commit(circ(cx, cy, H_WIRE))   # 电池/电源走线

# --- 四角支撑铜柱 (M3, 连接上层板) ---
commit(circ(10, 10, H_M3))
commit(circ(10, h-10, H_M3))
commit(circ(w-10, 10, H_M3))
commit(circ(w-10, h-10, H_M3))

# --- 扩展孔 (3.5mm, 方便以后加东西) ---
for xi in [40, 60, 80, 120, 140, 160]:
    for yi in [45, 65, 85, 105]:
        commit(circ(xi, yi, H_M3))


# ========== 上层板 ==========
off_x2, off_y2 = 0, LO_H + GAP
w2, h2 = UP_W, UP_H
cx2 = off_x2 + w2/2   # 90
cy2 = off_y2 + h2/2   # LO_H + GAP + 70

commit(rect_rounded(w2, h2, 8, off_x2, off_y2))

# --- 四角支撑孔 (与下层对应, 间距约 160×120) ---
commit(circ(off_x2+10, off_y2+10, H_M3))
commit(circ(off_x2+10, off_y2+h2-10, H_M3))
commit(circ(off_x2+w2-10, off_y2+10, H_M3))
commit(circ(off_x2+w2-10, off_y2+h2-10, H_M3))

# --- BTS7960 (4 孔, 45×35mm, 上层左前) ---
bx, by = off_x2+30, off_y2+20
for dx, dy in [(-22.5, -17.5), (22.5, -17.5), (-22.5, 17.5), (22.5, 17.5)]:
    commit(circ(bx+dx, by+dy, H_M3))

# --- ESP32 (2 孔, 间距 45mm, 上层右前) ---
ex, ey = off_x2+110, off_y2+25
commit(circ(ex-22.5, ey, H_M3))
commit(circ(ex+22.5, ey, H_M3))

# --- 激光雷达 (4 孔, 30mm 正方, 上层正中偏后) ---
lx, ly = cx2, off_y2+85
for dx, dy in [(-15, -15), (15, -15), (-15, 15), (15, 15)]:
    commit(circ(lx+dx, ly+dy, H_M3))

# --- 树莓派 (4 孔, 58×49mm, 上层左后) ---
# 树莓派 4B 安装孔: 58×49mm
px, py = off_x2+45, off_y2+85
for dy in [-24.5, 24.5]:
    for dx in [-29, 29]:
        commit(circ(px+dx, py+dy, H_M3))

# --- USB 穿线孔 (ESP32, 雷达, 树莓派) ---
commit(circ(off_x2+110, off_y2+55, H_USB))   # ESP32 USB
commit(circ(cx2, off_y2+h2-25, H_USB))         # 雷达 USB
commit(circ(off_x2+20, off_y2+h2-25, H_USB))    # 树莓派供电

# --- 走线孔 (10mm, 上下层线缆穿过) ---
commit(circ(cx2, off_y2+50, H_WIRE))
commit(circ(off_x2+w2-30, off_y2+70, H_WIRE))

# ===== 输出 =====
dxf = header() + all_entities + footer()
with open(FILENAME, "w") as f:
    f.write(dxf)

print(f"已生成: {FILENAME}")
print(f"下层板: {LO_W}×{LO_H}mm  电机+万向轮+电池")
print(f"上层板: {UP_W}×{UP_H}mm  电控+雷达+树莓派")
print(f"安装孔: 3.5mm(M3)  走线:10mm  USB:15mm")
print(f"两层间距 ~50mm (用 M3 双通铜柱 + 螺丝连接四角)")
