CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
aux_source_directory(. DIR_CLIENTEXT)
ADD_EXECUTABLE(netDemoClient netDemoClient.cpp)