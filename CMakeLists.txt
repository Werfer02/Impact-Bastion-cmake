cmake_minimum_required(VERSION 3.30)
project(Impact-Bastion VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Impact-Bastion impact_bastion/main.cpp impact_bastion/Game.cpp)

target_include_directories(Impact-Bastion PRIVATE SFML-3.0.2/include impact_bastion)

target_link_directories(Impact-Bastion PRIVATE SFML-3.0.2/lib)
target_link_libraries(Impact-Bastion PRIVATE 
                                     sfml-graphics
                                     sfml-system
                                     sfml-window)

