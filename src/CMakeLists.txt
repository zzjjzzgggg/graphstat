# add_library(config config.cpp)
# set(LIBRARIES config ${LIBRARIES})

# add_library(walk_engine walk_engine.cpp)
# target_link_libraries(walk_engine ${LIBRARIES})

# add_library(greedy_manager greedy_manager.cpp)
# target_link_libraries(greedy_manager ${LIBRARIES})


add_executable(graphstat main_graphstat.cpp)
target_link_libraries(graphstat  ${LIBRARIES})
