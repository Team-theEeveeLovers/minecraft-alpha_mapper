

add_executable(
	MC_Alpha_Mapper 
"globals.h" "Main.cpp" "Main.h" "Window.cpp" "Window.hpp" "Renderer.cpp" "Renderer.hpp" "Gui.cpp" "Gui.hpp" "FileLoading.cpp" "FileLoading.hpp" 
"Types/NBT.h" "Types/Time.h" "Types/VectorTypes.h" 
"Utils/Base36.cpp" "Utils/Base36.hpp" 
"_external/imgui/imgui.cpp" "_external/imgui/imgui_demo.cpp" "_external/imgui/imgui_draw.cpp" "_external/imgui/imgui_tables.cpp" "_external/imgui/imgui_widgets.cpp" 
"_external/imgui/backends/imgui_impl_sdl2.cpp" "_external/imgui/backends/imgui_impl_sdlrenderer2.cpp" 
     )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MC_Alpha_Mapper PROPERTY CXX_STANDARD 20)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PROCESSOR_ARCH "x64")
else()
  set(PROCESSOR_ARCH "x86")
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug") # Debug build, debug info enabled, optimizations disabled.
    set(MAPPER_BUILD_TYPE "DEBUG")
	add_compile_definitions("MAPPER_DEBUG") 

elseif (CMAKE_BUILD_TYPE MATCHES "Release") # Release build, debug info disabled, optimizations enabled.
    set(MAPPER_BUILD_TYPE "RELEASE")
	add_compile_definitions("MAPPER_RELEASE")

elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel") # Compact build, optimized for lowest filesize.
	set(MAPPER_BUILD_TYPE "COMPACT") 
	add_compile_definitions("MAPPER_COMPACT")

elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo") # Release with debug info build, debug info enabled, optimizations enabled.
	set(MAPPER_BUILD_TYPE "RELEASE_WITH_DEBUG_INFO") 
	add_compile_definitions("MAPPER_RELEASE")
	add_compile_definitions("MAPPER_DEBUG")
	add_compile_definitions("MAPPER_RELEASE_WITH_DEBUG_INFO")

endif()

if(CMAKE_CXX_COMPILER_LOADED)
	message(STATUS "")
	message(STATUS "Minecraft Alpha Mapper was configured using the following parameters:")
	message(STATUS "")
    message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
	message(STATUS "Platform: ${CMAKE_SYSTEM}")
	message(STATUS "Platform (Generic Name): ${CMAKE_SYSTEM_NAME}")
	message(STATUS "Architecture: ${PROCESSOR_ARCH}")
	message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
	message(STATUS "")
	message(STATUS "")
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
	# using GCC on Windows through msys64 or equivalent
	if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
		# zlib
		add_custom_command(
		TARGET MC_Alpha_Mapper POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_BINARY_DIR}/_dependlibs/zlib/libzlib.dll
				${CMAKE_CURRENT_BINARY_DIR}/libzlib.dll
		)
	else() # using another compiler like MSVC
		# zlib
		add_custom_command(
		TARGET MC_Alpha_Mapper POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_BINARY_DIR}/_dependlibs/zlib/zlibd.dll
				${CMAKE_CURRENT_BINARY_DIR}/zlibd.dll
		)
	endif()
	# TODO: Are there any different filenames that happen with compilers that aren't GCC or MSVC, like clang?
endif()

target_include_directories(MC_Alpha_Mapper PRIVATE "_external/" "_external/imgui/" "_external/imgui-filebrowser/" "_external/gzip-hpp/include/" "../_dependlibs/zlib/")

# Link to required libraries
target_link_libraries(MC_Alpha_Mapper PRIVATE SDL2::SDL2 zlib)