#!/bin/bash
# ============================================================
# save_map.sh — 保存 SLAM 地图
# ============================================================
# 使用: bash save_map.sh <地图名>
# 示例: bash save_map.sh lab_1f
#       地图将保存到 ~/maps/lab_1f.yaml + lab_1f.pgm
# ============================================================

MAP_NAME="${1}"
if [ -z "${MAP_NAME}" ]; then
    echo "用法: bash save_map.sh <地图名>"
    echo "示例: bash save_map.sh lab_1f"
    exit 1
fi

MAP_DIR="${HOME}/maps"
mkdir -p "${MAP_DIR}"

rosrun map_server map_saver -f "${MAP_DIR}/${MAP_NAME}"

echo ""
echo "地图已保存到: ${MAP_DIR}/${MAP_NAME}.yaml"
echo "导航时使用: bash nav_start.sh ${MAP_DIR}/${MAP_NAME}.yaml"
