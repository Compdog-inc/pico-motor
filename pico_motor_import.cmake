add_subdirectory(pico-motor)
link_directories(${CMAKE_CURRENT_LIST_DIR}/pico-motor)

macro(pico_motor_init target)
    target_include_directories(target PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pico-motor/include)
endmacro()
