# target_link_libraries(stand_walk_detect -lmraa)
set(CMAKE_EXE_LINKER_FLAGS -lmraa)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(9DOF_Example Using_OLED_and_9DOF_Blocks.cpp SFE_LSM9DS0.cpp)
