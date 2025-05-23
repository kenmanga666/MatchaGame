add_executable(client main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

target_link_libraries(client PRIVATE sfml-graphics sfml-window sfml-system)

# Si tu veux aussi ImGui :
# find_package(imgui CONFIG REQUIRED)
# target_link_libraries(client PRIVATE imgui::imgui-sfml)
