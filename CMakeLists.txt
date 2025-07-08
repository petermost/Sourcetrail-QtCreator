cmake_minimum_required(VERSION 3.25)

set(CREATOR_VERSION "17.0.0")
set(CREATOR_URL "https://download.qt.io/official_releases/qtcreator/17.0/${CREATOR_VERSION}/installer_source")

if (LINUX)
	set(CREATOR_URL "${CREATOR_URL}/linux_x64")
elseif(WINDOWS)
	set(CREATOR_URL "${CREATOR_URL}/windows_x64")
endif()

project(QtCreatorSourcetrailPlugin VERSION ${CREATOR_VERSION} LANGUAGES CXX)

function(downloadAndExtract targetDir url archiveName)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/${targetDir}")
        message(STATUS "Downloading ${url} ...")
        file(DOWNLOAD "${url}" "${CMAKE_BINARY_DIR}/${archiveName}" SHOW_PROGRESS)
        execute_process(COMMAND 7z x -y "${CMAKE_BINARY_DIR}/${archiveName}" -o${CMAKE_BINARY_DIR}/${targetDir})
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

# Include directories
target_include_directories(Sourcetrail 
    PRIVATE
        ${CMAKE_BINARY_DIR}/qt-src/include/qtcreator/src/libs/
        ${CMAKE_BINARY_DIR}/qt-src/include/qtcreator/src/plugins/
)

target_link_directories(Sourcetrail
	PRIVATE
		${CMAKE_BINARY_DIR}/qt-bin/lib/qtcreator/plugins/
)

# Link to Qt libraries
target_link_libraries(Sourcetrail PRIVATE
    Qt6::Gui
    Qt6::Network
    Qt6::Widgets
    
	Core # QtCreator::Core
	# QtCreator::ExtensionSystem
	# QtCreator::Utils
)

set(CPACK_GENERATOR ZIP)
install(TARGETS Sourcetrail DESTINATION .)
include(CPack)
