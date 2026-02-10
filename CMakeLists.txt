cmake_minimum_required(VERSION 3.25)

# Adjust the first 2 variables for newer versions:
# Note: These variables are also referenced in 'Sourcetrail.json.in'

set(CREATOR_MAJOR_MINOR_VERSION "18.0")
set(CREATOR_FULL_VERSION "18.0.2")
set(CREATOR_COMPAT_VERSION "18.0.0")

# Do the download:

set(CREATOR_URL "https://download.qt.io/official_releases/qtcreator/${CREATOR_MAJOR_MINOR_VERSION}/${CREATOR_FULL_VERSION}/installer_source")
if (LINUX)
	set(CREATOR_URL "${CREATOR_URL}/linux_x64")
elseif(WIN32)
	set(CREATOR_URL "${CREATOR_URL}/windows_x64")
else()
	message(FATAL_ERROR, "Untested platform ${CMAKE_SYSTEM_NAME}!")
endif()

project(QtCreator-SourcetrailPlugin VERSION 2026.2.10 LANGUAGES CXX)

function(downloadAndExtract targetDir url archiveName)
	if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${targetDir}")
		message(STATUS "Downloading ${url} ...")
		file(DOWNLOAD "${url}" "${CMAKE_CURRENT_BINARY_DIR}/${archiveName}" SHOW_PROGRESS)
		execute_process(COMMAND 7z x -y "${CMAKE_CURRENT_BINARY_DIR}/${archiveName}" -o${CMAKE_CURRENT_BINARY_DIR}/${targetDir})
	endif()
endfunction()

downloadAndExtract("qt-src" "${CREATOR_URL}/qtcreator_dev.7z" "qt-dev.7z")
downloadAndExtract("qt-bin" "${CREATOR_URL}/qtcreator.7z" "qt-bin.7z")

# Make Ninja build verbose as well (https://github.com/ninja-build/ninja/issues/900):
if (CMAKE_VERBOSE_MAKEFILE)
	set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Verbose Makefile" FORCE)
endif()

# Hide things by default
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# Enable automatic handling of Qt meta-features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(QtCreatorAPI REQUIRED COMPONENTS Core CppEditor)
find_package(Qt6 REQUIRED COMPONENTS Gui Network Widgets)

configure_file(Sourcetrail.json.in Sourcetrail.json @ONLY)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(Sourcetrail SHARED
	sourcetrail.qrc
	
	src/sourcetrailpluginsettingspage.ui
	
	src/sourcetrailconstants.h
	src/sourcetrail_global.h
	
	src/sourcetrailpluginsettingspage.h
	src/sourcetrailpluginsettingspage.cpp
	
	src/sourcetrailpluginsettings.h
	src/sourcetrailpluginsettings.cpp
	
	src/sourcetrailplugin.h
	src/sourcetrailplugin.cpp
)

target_include_directories(Sourcetrail 
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/qt-src/include/qtcreator/src/libs/
		${CMAKE_CURRENT_BINARY_DIR}/qt-src/include/qtcreator/src/plugins/
)

target_link_directories(Sourcetrail
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/qt-bin/lib/qtcreator/plugins/
)

target_link_libraries(Sourcetrail 
	PRIVATE
		Qt6::Gui
		Qt6::Network
		Qt6::Widgets
		
		Core # QtCreator::Core
)

if (WIN32)
	target_link_directories(Sourcetrail
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}/qt-src/bin/
			${CMAKE_CURRENT_BINARY_DIR}/qt-src/lib/qtcreator/plugins/
	)
	
	target_link_libraries(Sourcetrail
		PRIVATE
			Utils           # QtCreator::Utils 
			ExtensionSystem # QtCreator::ExtensionSystem
	)
endif()

set(CPACK_GENERATOR ZIP)
install(TARGETS Sourcetrail DESTINATION .)
include(CPack)
