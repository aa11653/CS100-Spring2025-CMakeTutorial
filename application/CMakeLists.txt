cmake_minimum_required(VERSION 3.5...3.10)
project(CS100RecitationFinal)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

# 添加 Corrade 作为子项目
add_subdirectory(third_party/corrade EXCLUDE_FROM_ALL)

# 添加 Magnum 作为子项目
# 手动添加 SDL2 的路径供 MAGNUM 使用, 这一块会交给 Magnum 来处理
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/third_party/SDL2-2.30.9 ${CMAKE_PREFIX_PATH})
set(MAGNUM_WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
add_subdirectory(third_party/magnum EXCLUDE_FROM_ALL)

add_subdirectory(src)
