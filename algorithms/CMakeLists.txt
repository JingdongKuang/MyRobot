# algorithms 聚合模块
# 此目录本身不产出库，仅聚合下属算法子模块（滤波/规划/控制等）。

add_subdirectory(filter)

if (BUILD_ALGORITHMS)
  add_subdirectory(planning)
  add_subdirectory(control)
endif()
