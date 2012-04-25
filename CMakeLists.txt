cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
if(POLICY CMP0017)
    cmake_policy(SET CMP0017 NEW)
endif(POLICY CMP0017)
project(kvirtual)

set(CMAKE_INSTALL_PREFIX "/usr")

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
find_package(Msgfmt REQUIRED)
find_package(Gettext REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
add_subdirectory( po )