cmake_minimum_required(VERSION 3.16.3)

project(User)

add_executable(executableUser ../controller/userController.cpp ../model/user.cpp ../view/userView.cpp)