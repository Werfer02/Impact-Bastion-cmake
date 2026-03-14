cmake_minimum_required(VERSION 3.30)
project(Impact-Bastion VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Impact-Bastion impact_bastion/main.cpp impact_bastion/Game.cpp)

target_include_directories(Impact-Bastion PRIVATE SFML-3.0.2/include impact_bastion)

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

target_link_libraries(Impact-Bastion PRIVATE 
                                     sfml-graphics
                                     sfml-system
                                     sfml-window
                                     sfml-audio)

set(EXECUTABLE_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/impact_bastion/resources")

set(FINAL_BINARY_DIR "${CMAKE_BINARY_DIR}/$<IF:$<BOOL:$<CONFIG>>,$<CONFIG>,.>")

add_custom_command(TARGET Impact-Bastion POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${FINAL_BINARY_DIR}/graphics"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${FINAL_BINARY_DIR}/audio"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXECUTABLE_RESOURCES_DIR}/*.ttf" "${FINAL_BINARY_DIR}/"
    "${EXECUTABLE_RESOURCES_DIR}/*.txt" "${FINAL_BINARY_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXECUTABLE_RESOURCES_DIR}/audio/*" "${FINAL_BINARY_DIR}/audio"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${EXECUTABLE_RESOURCES_DIR}/graphics/*" "${FINAL_BINARY_DIR}/graphics"
)

if (MSVC)
    add_custom_command(TARGET Impact-Bastion POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${EXECUTABLE_RESOURCES_DIR}/$<CONFIG>" "${FINAL_BINARY_DIR}/$<CONFIG>"
    )
endif()