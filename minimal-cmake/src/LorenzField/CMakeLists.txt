add_library(LorenzField
    LorenzField.cpp
)

target_include_directories(LorenzField
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)