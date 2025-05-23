add_executable(client main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS system window graphics)

target_link_libraries(client PRIVATE sfml-system sfml-window sfml-graphics)

# Si tu veux aussi ImGui :
# find_package(imgui CONFIG REQUIRED)
# target_link_libraries(client PRIVATE imgui::imgui-sfml)
