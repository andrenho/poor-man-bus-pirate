cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(poor-man-bus-pirate C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(${CMAKE_PROJECT_NAME}
		src/main.c
		src/util/getline.c
		src/util/getline.h
		src/command.c
		src/command.h
		src/variables.c
		src/variables.h
		src/uart.c
		src/uart.h
		src/output.c
		src/output.h
		src/spi_slave.c
		src/spi_slave.h
		src/spi_master.c
		src/spi_master.h
		src/pwm.c
		src/pwm.h
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
		POST_BUILD
		COMMAND arm-none-eabi-size -G ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
)

add_custom_target(upload-swd
	COMMAND openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
	DEPENDS ${CMAKE_PROJECT_NAME}
	COMMENT "Uploading via SWD..."
	VERBATIM
)

add_custom_target(upload-usb
	COMMAND ${CMAKE_SOURCE_DIR}/upload-usb.sh ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.uf2
	DEPENDS ${CMAKE_PROJECT_NAME}
	COMMENT "Uploading via USB..."
	VERBATIM
)

add_custom_target(upload-remote
	COMMAND curl -X POST -H "Content-Type: application/octet-stream" --data-binary @${CMAKE_PROJECT_NAME}.elf http://$ENV{REMOTE_IP}:8000
	DEPENDS ${CMAKE_PROJECT_NAME}
	COMMENT "Uploading remotely..."
	VERBATIM
)

add_custom_target(talk
	COMMAND screen /dev/ttyACM0 115200
)

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

target_link_libraries(${CMAKE_PROJECT_NAME} pico_stdlib hardware_spi hardware_pwm)
