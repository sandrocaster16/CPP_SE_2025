add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE commands)
target_link_libraries(${PROJECT_NAME} PRIVATE console)
target_link_libraries(${PROJECT_NAME} PRIVATE game_logic)
target_link_libraries(${PROJECT_NAME} PRIVATE settings)
target_link_libraries(${PROJECT_NAME} PRIVATE util)

target_link_libraries(${PROJECT_NAME} PRIVATE BattleShip)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})