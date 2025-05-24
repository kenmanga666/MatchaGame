add_executable(client main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(client PRIVATE ${SFML_LIB}/sfml-graphics.lib ${SFML_LIB}/sfml-window.lib ${SFML_LIB}/sfml-system.lib)
target_include_directories(client PRIVATE ${SFML_INCLUDE_DIR})

# Si tu veux aussi ImGui :
# find_package(ImGui-SFML CONFIG REQUIRED)
# target_link_libraries(client PRIVATE ImGui-SFML::ImGui-SFML))
