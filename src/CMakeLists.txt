add_library(global_variables global_variables.cpp)
target_link_libraries(global_variables ${LIBRARIES})

add_library(format_graph format_graph.cpp)
target_link_libraries(format_graph global_variables)

add_library(convert_graph convert_graph.cpp)
target_link_libraries(convert_graph global_variables)


add_executable(graphstat main_graphstat.cpp)
target_link_libraries(graphstat format_graph convert_graph)
