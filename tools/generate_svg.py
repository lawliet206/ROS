#!/usr/bin/env python3
"""生成两层底盘 SVG 图纸 — 浏览器打开, 彩色标注"""
LO_W, LO_H = 200, 160  # 下层
UP_W, UP_H = 180, 140  # 上层
SCALE = 3.0
H_M3, H_WIRE, H_USB = 3.5, 10.0, 15.0

# 上层在左边, 下层在右边
GAP = 30
UP_X = GAP
LO_X = UP_X + UP_W + GAP
H = max(UP_H, LO_H)

def cc(x, y, r, color, label="", lx=0, ly=0, lc="black", base_x=0, base_y=0):
    cx = (base_x + x) * SCALE
    cy = (H - (base_y + y)) * SCALE
    cr = r * SCALE
    s = f'<circle cx="{cx:.1f}" cy="{cy:.1f}" r="{cr:.1f}" fill="none" stroke="{color}" stroke-width="1.2"/>'
    if label:
        lx2 = (base_x + x + lx) * SCALE if lx else cx
        ly2 = (H - (base_y + y + ly)) * SCALE if ly else cy + cr + 10
        s += f'\n<text x="{lx2:.1f}" y="{ly2:.1f}" fill="{lc}" font-size="8" font-family="monospace" text-anchor="middle">{label}</text>'
    return s

def rr(x, y, w, h, color, label="", base_x=0, base_y=0):
    rx = (base_x + x) * SCALE
    ry = (H - (base_y + y)) * SCALE
    s = f'<rect x="{rx:.1f}" y="{ry-h*SCALE:.1f}" width="{w*SCALE:.1f}" height="{h*SCALE:.1f}" fill="none" stroke="{color}" stroke-width="1" stroke-dasharray="4,2"/>'
    if label:
        s += f'\n<text x="{(base_x+x+w/2)*SCALE:.1f}" y="{ry-h*SCALE-4:.1f}" fill="{color}" font-size="8" font-family="monospace" text-anchor="middle">{label}</text>'
    return s

svg = f'''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" width="{(LO_X+LO_W+GAP)*SCALE:.0f}" height="{H*SCALE+60}">
<rect x="{(UP_X-GAP/2)*SCALE:.0f}" y="10" width="{(LO_W+UP_W+GAP*2)*SCALE:.0f}" height="{H*SCALE+10}" fill="#fafafa"/>
'''

# 外框
svg += f'<rect x="{UP_X*SCALE:.0f}" y="{(H-UP_H)*SCALE:.0f}" width="{UP_W*SCALE:.0f}" height="{UP_H*SCALE:.0f}" fill="white" stroke="#333" stroke-width="2.5"/>'
svg += f'<rect x="{LO_X*SCALE:.0f}" y="{(H-LO_H)*SCALE:.0f}" width="{LO_W*SCALE:.0f}" height="{LO_H*SCALE:.0f}" fill="white" stroke="#333" stroke-width="2.5"/>'

# 标题
svg += f'<text x="{LO_X*SCALE:.0f}" y="{H*SCALE+15:.0f}" fill="#333" font-size="13" font-family="sans-serif" font-weight="bold">上层 180×140mm (电控)</text>'
svg += f'<text x="{(LO_X+LO_W)*SCALE:.0f}" y="{H*SCALE+30:.0f}" fill="#333" font-size="13" font-family="sans-serif" font-weight="bold">下层 200×160mm (机械)</text>'

# ==== 下层板 ====
bx, by = LO_X, 0
cx, cy = bx+LO_W/2, by+LO_H/2

# 左电机 M1-M4
for dx, dy, label in [(-14.5, -14.5, "M1"), (14.5, -14.5, "M2"), (-14.5, 14.5, "M3"), (14.5, 14.5, "M4")]:
    svg += cc(cx+dx-bx, 26+dy-by, H_M3/2, "#2980b9", label, lx=0, ly=-6, lc="#2980b9", base_x=bx, base_y=by)

# 右电机 M5-M8
for dx, dy, label in [(-14.5, -14.5, "M5"), (14.5, -14.5, "M6"), (-14.5, 14.5, "M7"), (14.5, 14.5, "M8")]:
    svg += cc(cx+dx-bx, 134+dy-by, H_M3/2, "#2980b9", label, lx=0, ly=10, lc="#2980b9", base_x=bx, base_y=by)

# 牛眼轮
svg += cc(165-bx, cy-10-by, H_M3/2, "#27ae60", "C1", lx=0, ly=10, lc="#27ae60", base_x=bx, base_y=by)
svg += cc(165-bx, cy+10-by, H_M3/2, "#27ae60", "C2", lx=0, ly=10, lc="#27ae60", base_x=bx, base_y=by)
svg += cc(25-bx, cy-10-by, H_M3/2, "#27ae60", "C3", lx=0, ly=10, lc="#27ae60", base_x=bx, base_y=by)
svg += cc(25-bx, cy+10-by, H_M3/2, "#27ae60", "C4", lx=0, ly=10, lc="#27ae60", base_x=bx, base_y=by)

# 电池槽
svg += rr(68, 35, 12, 90, "#e74c3c", "S1 电池", bx, by)
svg += rr(120, 35, 12, 90, "#e74c3c", "S2", bx, by)

# 走线孔
svg += cc(cx-bx, 50-by, H_WIRE/2, "#16a085", "W1", lx=0, ly=10, lc="#16a085", base_x=bx, base_y=by)
svg += cc(cx-bx, 110-by, H_WIRE/2, "#16a085", "W2", lx=0, ly=10, lc="#16a085", base_x=bx, base_y=by)
svg += cc(cx-bx, cy-by, H_WIRE/2, "#16a085", "W3", lx=0, ly=-8, lc="#16a085", base_x=bx, base_y=by)

# 四角铜柱
for xi, yi in [(10,10), (10,LO_H-10), (LO_W-10,10), (LO_W-10,LO_H-10)]:
    svg += cc(xi, yi, H_M3/2, "#7f8c8d", "S", lx=8, ly=0, lc="#999", base_x=bx, base_y=by)

# 网格孔
for xi in [40, 60, 80, 120, 140, 160]:
    for yi in [45, 65, 85, 105]:
        svg += cc(xi, yi, H_M3/2, "#bdc3c7", base_x=bx, base_y=by)

# ==== 上层板 ====
ux, uy = UP_X, 0
uw, uh = UP_W, UP_H

# BTS7960
bx2, by2 = ux+30, uy+20
for dx, dy, label in [(-22.5, -17.5, "B1"), (22.5, -17.5, "B2"), (-22.5, 17.5, "B3"), (22.5, 17.5, "B4")]:
    svg += cc(bx2+dx-ux, by2+dy-uy, H_M3/2, "#8e44ad", label, lx=0, ly=-6, lc="#8e44ad", base_x=ux, base_y=uy)

# ESP32
ex2 = ux+110
svg += cc(ex2-22.5-ux, uy+25-uy, H_M3/2, "#e67e22", "E1", lx=0, ly=-6, lc="#e67e22", base_x=ux, base_y=uy)
svg += cc(ex2+22.5-ux, uy+25-uy, H_M3/2, "#e67e22", "E2", lx=0, ly=-6, lc="#e67e22", base_x=ux, base_y=uy)

# 激光雷达
lx2, ly2 = ux+uw/2, uy+85
for dx, dy, label in [(-15, -15, "L1"), (15, -15, "L2"), (-15, 15, "L3"), (15, 15, "L4")]:
    svg += cc(lx2+dx-ux, ly2+dy-uy, H_M3/2, "#c0392b", label, lx=0, ly=-6, lc="#c0392b", base_x=ux, base_y=uy)

# 树莓派
px2, py2 = ux+45, uy+85
for dy2 in [-24.5, 24.5]:
    for dx2 in [-29, 29]:
        svg += cc(px2+dx2-ux, py2+dy2-uy, H_M3/2, "#d35400", base_x=ux, base_y=uy)

# USB 孔
svg += cc(ux+110-ux, uy+55-uy, H_USB/2, "#d35400", "U1", lx=10, ly=0, lc="#d35400", base_x=ux, base_y=uy)
svg += cc(ux+uw/2-ux, uy+uh-25-uy, H_USB/2, "#d35400", "U2", lx=10, ly=0, lc="#d35400", base_x=ux, base_y=uy)
svg += cc(ux+20-ux, uy+uh-25-uy, H_USB/2, "#d35400", "U3", lx=-10, ly=0, lc="#d35400", base_x=ux, base_y=uy)

# 走线孔
svg += cc(ux+uw/2-ux, uy+50-uy, H_WIRE/2, "#16a085", base_x=ux, base_y=uy)
svg += cc(ux+uw-30-ux, uy+70-uy, H_WIRE/2, "#16a085", base_x=ux, base_y=uy)

# 四角铜柱
for xi2, yi2 in [(10,10), (10,uh-10), (uw-10,10), (uw-10,uh-10)]:
    svg += cc(xi2, yi2, H_M3/2, "#7f8c8d", "S", lx=8, ly=0, lc="#999", base_x=ux, base_y=uy)

# 双层示意线
svg += f'<line x1="{(ux+10)*SCALE:.0f}" y1="{(H-uy-10)*SCALE:.0f}" x2="{(bx+10)*SCALE:.0f}" y2="{(H-by-10)*SCALE:.0f}" stroke="#999" stroke-width="1" stroke-dasharray="3,3"/>'
svg += f'<text x="{((ux+10+bx+10)/2)*SCALE:.0f}" y="{(H-20)*SCALE:.0f}" fill="#999" font-size="8" font-family="monospace" text-anchor="middle">50mm 铜柱连接四角</text>'

# 图例
ly0 = H*SCALE + 40
items = [("#2980b9","M1-M8 电机"),("#27ae60","C1-C4 牛眼轮"),("#8e44ad","B1-B4 BTS7960"),
         ("#e67e22","E1-E2 ESP32"),("#c0392b","L1-L4 激光雷达"),("#d35400","树莓派+USB"),
         ("#16a085","W 走线10mm"),("#7f8c8d","S 支撑铜柱"),("#e74c3c","S 电池槽")]
for i,(c,d) in enumerate(items):
    x = 15 + (i%5)*95
    y = ly0 + (i//5)*16
    svg += f'<circle cx="{x+4:.0f}" cy="{y-3:.0f}" r="4" fill="{c}" opacity="0.5"/>'
    svg += f'<text x="{x+14:.0f}" y="{y:.0f}" fill="#333" font-size="9" font-family="monospace">{d}</text>'

svg += '</svg>'

with open("chassis_diagram.svg", "w", encoding="utf-8") as f:
    f.write(svg)

print("已生成: chassis_diagram.svg")
print(f"下层 {LO_W}×{LO_H}mm (电机+电池)   上层 {UP_W}×{UP_H}mm (电控)")
print("浏览器打开查看")
