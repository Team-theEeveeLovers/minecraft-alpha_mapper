add_executable(MC_Alpha_Mapper "globals.h" "Main.cpp" "Main.h" "Window.cpp" "Window.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MC_Alpha_Mapper PROPERTY CXX_STANDARD 20)
endif()

# COPY DLLs
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	# SDL2
	add_custom_command(
	TARGET MC_Alpha_Mapper POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/_dependlibs/SDL2/SDL2d.dll
			${CMAKE_CURRENT_BINARY_DIR}/SDL2d.dll
	)
endif()

# Link to required libraries
target_link_libraries(MC_Alpha_Mapper PRIVATE SDL2::SDL2)