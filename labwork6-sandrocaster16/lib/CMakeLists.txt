add_library(traveler Traveler.cpp)

target_link_libraries(
        traveler cpr
)

target_link_libraries(
        traveler nlohmann_json::nlohmann_json
)
