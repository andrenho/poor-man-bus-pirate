cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(poor-man-bus-pirate C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(poor-man-bus-pirate
	  main.c
)

add_custom_target(upload-swd
	COMMAND openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program poor-man-bus-pirate.elf verify reset exit"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	DEPENDS poor-man-bus-pirate
	COMMENT "Uploading via SWD..."
	VERBATIM
)

pico_enable_stdio_usb(poor-man-bus-pirate 1)
pico_enable_stdio_uart(poor-man-bus-pirate 1)

pico_add_extra_outputs(poor-man-bus-pirate)

target_link_libraries(poor-man-bus-pirate pico_stdlib)
