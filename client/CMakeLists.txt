# Inclure le dossier engine
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)

find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics)

# TicTacNet-CLI
add_executable(TicTacNet_CLI
    TicTacNet/main.cpp
    TicTacNet/TicTacNet.cpp
 )

add_executable(TicTacNet_GUI
    TicTacNet/main_gui.cpp
    TicTacNet/TicTacNetGUI.cpp
)

set_target_properties(TicTacNet_GUI PROPERTIES WIN32_EXECUTABLE TRUE)
set_target_properties(TicTacNet_GUI PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_link_libraries(TicTacNet_GUI PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(TicTacNet_GUI PRIVATE "C:/Users/kenny/vcpkg/installed/x64-windows/include")
target_link_directories(TicTacNet_GUI PRIVATE "C:/Users/kenny/vcpkg/installed/x64-windows/lib")

# PFCNet (Pierre Feuille Ciseaux)
#add_executable(PFCNet
#    PFCNet/main.cpp
#    PFCNet/PFCNet.cpp
#)

#target_link_libraries(PFCNet PRIVATE sfml-graphics sfml-window sfml-system)


# Si tu veux aussi ImGui :
# find_package(ImGui-SFML CONFIG REQUIRED)
# target_link_libraries(client PRIVATE ImGui-SFML::ImGui-SFML))
