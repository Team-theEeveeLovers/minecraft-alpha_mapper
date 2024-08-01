add_executable(MC_Alpha_Mapper "Main.cpp" "Main.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MC_Alpha_Mapper PROPERTY CXX_STANDARD 20)
endif()