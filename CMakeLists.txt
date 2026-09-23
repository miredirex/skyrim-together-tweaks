cmake_minimum_required(VERSION 3.23)
set(NAME "SkyrimTogetherTweaks" CACHE STRING "")
set(VERSION 1.0.2 CACHE STRING "")

# ---- Options ----

option(COPY_BUILD "Copy the build output to the Skyrim directory." TRUE)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

macro(find_commonlib_path)
	if (CommonLibName AND NOT ${CommonLibName} STREQUAL "")
		# Check extern
		find_path(CommonLibPath
		include/REL/Relocation.h
		PATHS extern/${CommonLibName})
		if (${CommonLibPath} STREQUAL "CommonLibPath-NOTFOUND")
			#Check path
			set_from_environment(${CommonLibName}Path)
			set(CommonLibPath ${${CommonLibName}Path})
		endif()
	endif()
endmacro()

set_from_environment(VCPKG_ROOT)

# -- Target Skyrim AE only
add_compile_definitions(SKYRIM_AE)
add_compile_definitions(SKYRIM_SUPPORT_AE)
set(CommonLibName "CommonLibSSE")
set(SkyrimVersion "Skyrim AE")
set_from_environment(SkyrimPath)

find_commonlib_path()
message(
	STATUS
	"Building ${NAME} ${VERSION} for ${SkyrimVersion} at ${SkyrimPath} with ${CommonLibName} at ${CommonLibPath}."
)

if (DEFINED VCPKG_ROOT)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
else ()
	message(
		WARNING
		"Variable VCPKG_ROOT is not set. Continuing without vcpkg."
	)
endif ()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
	@ONLY
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# ---- Globals ----

add_compile_definitions(
	SKSE_SUPPORT_XBYAK
	REX_OPTION_INI
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

# ---- Dependencies ----
include(cmake/GitPatch.cmake)

if (DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${CommonLibPath})
	git_apply_patch(${CommonLibPath} patches/0001-no-address-lib-init.patch)
	add_subdirectory(${CommonLibPath} ${CommonLibName})
else ()
	message(
		FATAL_ERROR
		"Variable ${CommonLibName}Path is not set or in extern/."
	)
endif()

find_package(minhook CONFIG REQUIRED)

# ---- Add source files ----

file(GLOB_RECURSE headers
	CONFIGURE_DEPENDS
	"src/*.h"
	"src/*.hpp"
)

file(GLOB_RECURSE sources
	CONFIGURE_DEPENDS
	"src/*.c"
	"src/*.cpp"
)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
		${sources}
)

source_group(
	TREE
		${CMAKE_CURRENT_BINARY_DIR}
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
)

# ---- Create DLL ----

add_library(
	${PROJECT_NAME}
	SHARED
	${headers}
	${sources}
	${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_compile_features(
	${PROJECT_NAME}
	PRIVATE
		cxx_std_23
)

target_compile_definitions(
	${PROJECT_NAME}
	PRIVATE
		_UNICODE
)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
		${CommonLibName}::${CommonLibName}
		minhook::minhook
)

target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)

if (MSVC)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
			/sdl             # Enable Additional Security Checks
			/utf-8           # Set Source and Executable character sets to UTF-8
			/Zi              # Debug Information Format
			/guard:cf        # Control Flow Guard

			/permissive-     # Standards conformance
			/Zc:preprocessor # Enable preprocessor conformance mode

			/wd4200          # nonstandard extension used : zero-sized array in struct/union

			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3;/Gw>"
	)

	target_link_options(
		${PROJECT_NAME}
		PRIVATE
			"/PDBALTPATH:$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
			/guard:cf
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL>"
	)
endif ()

# ---- Post build ----

if (COPY_BUILD)
	if (DEFINED SkyrimPath)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${SkyrimPath}/SKSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${SkyrimPath}/SKSE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Skyrim/Data/SKSE/Plugins/SkyrimTogetherTweaks_preload.txt ${SkyrimPath}/SKSE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable 'SkyrimPath' is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
