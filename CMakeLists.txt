cmake_minimum_required(VERSION 3.20)
project(carbonApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
find_package(TGUI REQUIRED)

add_executable(carbonApp main.cpp
        ui/Screen.h
        ui/Screen.cpp
        ui/LoginScreen.h
        ui/LoginScreen.cpp
        ui/QuestionScreen.h
        ui/QuestionScreen.cpp
        ui/ResultsScreen.h
        ui/ResultsScreen.cpp
        Core/StateOfApp.h
        Core/Calculator.h
        Core/Calculator.cpp
        Core/Profiles.h
        Core/Profiles.cpp)

target_link_libraries(carbonApp PRIVATE
        TGUI::TGUI
        SFML::Graphics
        SFML::Window
        SFML::System
)


