cmake_minimum_required(VERSION 3.30)
project(Impact-Bastion VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Impact-Bastion impact_bastion/main.cpp impact_bastion/Game.cpp)

target_include_directories(Impact-Bastion PRIVATE SFML-3.0.2/include impact_bastion)

target_link_directories(Impact-Bastion PRIVATE SFML-3.0.2/lib)
target_link_libraries(Impact-Bastion PRIVATE 
                                     sfml-graphics
                                     sfml-system
                                     sfml-window)

set(EXECUTABLE_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/impact_bastion/resources")

add_custom_command(TARGET Impact-Bastion POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXECUTABLE_RESOURCES_DIR}/Roboto-bold.ttf" "${CMAKE_BINARY_DIR}/$<CONFIG>"
)

if (MSVC)
    add_custom_command(TARGET Impact-Bastion POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${EXECUTABLE_RESOURCES_DIR}/$<CONFIG>" "${CMAKE_BINARY_DIR}/$<CONFIG>"
    )
endif()