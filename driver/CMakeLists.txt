wdk_add_driver(driver driver_entry.cpp driver_entry.h)
target_include_directories(driver PRIVATE ${CMAKE_SOURCE_DIR}/shared)
