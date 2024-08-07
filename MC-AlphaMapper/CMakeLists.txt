

add_executable(
	MC_Alpha_Mapper 
"globals.h" "Main.cpp" "Main.h" "Window.cpp" "Window.hpp" "Renderer.cpp" "Renderer.hpp" "Gui.cpp" "Gui.hpp" "FileLoading.cpp" "FileLoading.hpp" "Types/NBT.h" 
"_external/imgui/imgui.cpp" "_external/imgui/imgui_demo.cpp" "_external/imgui/imgui_draw.cpp" "_external/imgui/imgui_tables.cpp" "_external/imgui/imgui_widgets.cpp" 
"_external/imgui/backends/imgui_impl_sdl2.cpp" "_external/imgui/backends/imgui_impl_sdlrenderer2.cpp" 
   )

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
	# zlib
	add_custom_command(
	TARGET MC_Alpha_Mapper POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/_dependlibs/zlib/zlibd.dll
			${CMAKE_CURRENT_BINARY_DIR}/zlibd.dll
	)
endif()

target_include_directories(MC_Alpha_Mapper PRIVATE "_external/" "_external/imgui/" "_external/imgui-filebrowser/" "_external/gzip-hpp/include/" "../_dependlibs/zlib/")

# Link to required libraries
target_link_libraries(MC_Alpha_Mapper PRIVATE SDL2::SDL2 zlib)