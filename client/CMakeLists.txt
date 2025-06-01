# Inclure le moteur de jeu
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)

find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics)

# TicTacNet (Morpion)
add_executable(TicTacNet
    TicTacNet/main.cpp
    TicTacNet/TicTacNet.cpp
)

target_link_libraries(TicTacNet PRIVATE sfml-graphics sfml-window sfml-system)
target_include_directories(TicTacNet PRIVATE "C:/Users/kenny/vcpkg/installed/x64-windows/include")
target_link_directories(TicTacNet PRIVATE "C:/Users/kenny/vcpkg/installed/x64-windows/lib")

# PFCNet (Pierre Feuille Ciseaux)
#add_executable(PFCNet
#    PFCNet/main.cpp
#    PFCNet/PFCNet.cpp
#)

#target_link_libraries(PFCNet PRIVATE sfml-graphics sfml-window sfml-system)


# Si tu veux aussi ImGui :
# find_package(ImGui-SFML CONFIG REQUIRED)
# target_link_libraries(client PRIVATE ImGui-SFML::ImGui-SFML))
