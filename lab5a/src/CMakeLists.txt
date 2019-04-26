set(CMAKE_EXE_LINKER_FLAGS -lmraa)
add_executable(9DOF_Example 
            Using_OLED_and_9DOF_Blocks.cpp 
            SFE_LSM9DS0.cpp 
            ../oled/Edison_OLED.cpp 
            ../spi/spi_device_edison.cpp
            ../gpio/gpio_edison.cpp
            )
