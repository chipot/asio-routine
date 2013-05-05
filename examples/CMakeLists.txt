project(asio-routine-examples CXX)

add_executable(
	echo
	echo.cc
)
target_link_libraries(
	echo
	asio-routine
)

find_package(TUNTAP)
if (TUNTAP_FOUND)
	message("${TUNTAP_INCLUDE_DIR}")
	include_directories(${TUNTAP_INCLUDE_DIR})
	add_executable(tuntap
		tuntap.cc
	)
	target_link_libraries(
		${TUNTAP_LIBRARIES}

	)
endif()

add_executable(stdin
	stdin.cc
)
target_link_libraries(stdin
  asio-routine
)

add_executable(signal
	signal.cc
)
target_link_libraries(signal
  asio-routine
)

add_executable(hello
	hello.cc
)
target_link_libraries(hello
  asio-routine
)
