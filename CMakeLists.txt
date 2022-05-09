cmake_minimum_required(VERSION 3.10)

project(JuniorApp)

#========================
#Sources
#========================
file(GLOB src "src/*.cpp")

add_executable(${PROJECT_NAME} ${src})
