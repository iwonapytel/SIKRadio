cmake_minimum_required(VERSION 3.5)
project(SIKRadio)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(Boost)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS("-DHAS_BOOST")
ENDIF ()

find_package(Threads)
SET(GCC_COVERAGE_COMPILE_FLAGS "-g")
SET(GCC_COVERAGE_LINK_FLAGS "-lgcov")

find_package(Boost COMPONENTS program_options log REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})

link_libraries(${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

file(GLOB sender_src "src/sender/*.h" "src/sender/*.cpp")
file(GLOB receiver_src "src/receiver/*.h" "src/receiver/*.cpp")
file(GLOB utils_src "src/utils/*.h" "src/utils/*.cpp")

file(GLOB sender_main src/sender/sik_sender.cpp)
file(GLOB receiver_main src/receiver/sik_receiver.cpp)

list(REMOVE_ITEM sender_src ${sender_main})
list(REMOVE_ITEM receiver_src ${receiver_main})

add_executable(sikradio-sender ${sender_main} ${sender_src} ${utils_src})
add_executable(sikradio-receiver ${receiver_main} ${receiver_src} ${utils_src})
