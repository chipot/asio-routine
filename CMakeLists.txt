project(tNETacle CXX)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAME_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX "/usr/local/")
set(${CMAKE_SYSTEM_NAME} True)

add_definitions(-D${CMAKE_SYSTEM_NAME})
add_definitions(-DBOOST_ASIO_HAS_MOVE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost COMPONENTS context system thread REQUIRED)

include_directories(
        ${Boost_INCLUDE_DIR}
)

set(	
	ASIOROUTINE_SRC

	asio-routine/asiofwd.hh
	asio-routine/coroutine_function.cc
	asio-routine/coroutine_function.hh
	asio-routine/coroutine_object.cc
	asio-routine/coroutine_object.hh
	asio-routine/descriptor.cc
	asio-routine/descriptor.hh
	asio-routine/future.hh
	asio-routine/print.hh
	asio-routine/scope.cc
	asio-routine/scope.hh
	asio-routine/server.cc
	asio-routine/server.hh
	asio-routine/session.cc
	asio-routine/session.hh
	asio-routine/signal.cc
	asio-routine/signal.hh
	asio-routine/thread.hh
	asio-routine/utility.cc
	asio-routine/utility.hh
)

add_library(
	asio-routine SHARED
	${ASIOROUTINE_SRC}
)

target_link_libraries(
	asio-routine
        ${Boost_LIBRARIES}
)

add_subdirectory(examples)
