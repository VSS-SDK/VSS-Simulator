PROJECT(vss-simulator)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-g -Wformat -ffast-math -lm -lc -std=c++11 -pthread")


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(Boost 1.40 COMPONENTS program_options REQUIRED)
FIND_PACKAGE(Bullet)
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

include_directories(${PROTOBUF_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${BULLET_INCLUDE_DIR})

file(GLOB_RECURSE SRCS_CPP src/*.cpp)
file(GLOB_RECURSE UTILS_CPP utils/*.cpp)
file(GLOB_RECURSE BULLET_CPP bullet/*.cpp)

set (VSS_INTERFACE libvss-interface.so)
link_directories(usr/lib)
include_directories(usr/local/include)

ADD_EXECUTABLE(vss-simulator ${SRCS_CPP} ${UTILS_CPP} ${BULLET_CPP})

TARGET_LINK_LIBRARIES(vss-simulator ${VSS_INTERFACE} ${PROTOBUF_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${BULLET_LIBRARIES})
TARGET_LINK_LIBRARIES(vss-simulator LINK_PUBLIC ${Boost_LIBRARIES})
target_include_directories(vss-simulator PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(vss-simulator PUBLIC ${ZeroMQ_LIBRARY})

INSTALL(TARGETS vss-simulator DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "vss-simulator")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A Simulator for the competition of Robot Soccer IEEE Very Small Size")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://sirlab.github.io/VSS-SDK/")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Johnathan Fercher")
SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

INCLUDE(CPack)
