# Inclure le moteur de jeu
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)

# TicTacNet (Morpion)
add_executable(TicTacNet
    TicTacNet/main.cpp
    TicTacNet/TicTacNet.cpp
)

target_link_libraries(TicTacNet PRIVATE sfml-graphics sfml-window sfml-system)

# PFCNet (Pierre Feuille Ciseaux)
add_executable(PFCNet
    PFCNet/main.cpp
    PFCNet/PFCNet.cpp
)

target_link_libraries(PFCNet PRIVATE sfml-graphics sfml-window sfml-system)


# Si tu veux aussi ImGui :
# find_package(ImGui-SFML CONFIG REQUIRED)
# target_link_libraries(client PRIVATE ImGui-SFML::ImGui-SFML))
