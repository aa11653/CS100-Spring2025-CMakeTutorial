find_package(Magnum REQUIRED)

add_library(LorenzField STATIC
    LorenzField.cpp
    LorenzField.h
)

target_link_libraries(
    LorenzField PUBLIC Magnum::Magnum
)

target_include_directories(
    LorenzField PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
