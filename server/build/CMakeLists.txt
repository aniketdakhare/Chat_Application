cmake_minimum_required(VERSION 3.16.3)

project(Server)

add_executable(executableServer ../controller/serverController.cpp ../model/server.cpp ../view/serverView.cpp)