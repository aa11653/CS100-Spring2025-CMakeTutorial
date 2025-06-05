find_package(Corrade REQUIRED Main)
find_package(Magnum REQUIRED GL MeshTools Primitives Shaders Trade Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(Viewer Viewer.cpp)
target_link_libraries(Viewer PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
    Magnum::MeshTools
    Magnum::Primitives
    LorenzField
)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Viewer)
