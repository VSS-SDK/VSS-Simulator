PROJECT(vss-simulator)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

ADD_SUBDIRECTORY(VSS-Interface)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
find_package(PkgConfig)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

include_directories(${Boost_INCLUDE_DIR} ${BULLET_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE UTILS utils/*.cpp)
file(GLOB_RECURSE BULLET bullet/*.cpp)

ADD_EXECUTABLE(vss-simulator ${SOURCES} ${UTILS} ${BULLET})

target_include_directories(vss-simulator PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(vss-simulator PUBLIC ${ZeroMQ_LIBRARY})

TARGET_LINK_LIBRARIES(vss-simulator VSS-Interface ${PROTOBUF_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
TARGET_LINK_LIBRARIES(vss-simulator LINK_PUBLIC ${Boost_LIBRARIES})

INSTALL(TARGETS vss-simulator DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "vss-simulator")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A 3D Software of visualization of states given by VSS-Simulator and VSS-Vision")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://sirlab.github.io/VSS-SDK/")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Johnathan Fercher")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

INCLUDE(CPack)
