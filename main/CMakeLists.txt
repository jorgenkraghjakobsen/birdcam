idf_build_get_property(target IDF_TARGET)

set(srcs            app_main.c)


set(include_dirs    .)
set(requires        wifi_interface camera_interface )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})

target_link_libraries(${COMPONENT_TARGET} ${links})
target_link_libraries(${COMPONENT_TARGET} ${lib})
