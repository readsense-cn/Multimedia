cmake_minimum_required(VERSION 3.4.1)

# 添加include目录路径
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/x264/include)

# 添加ffmpeg库
#add_library(ffmpeg SHARED IMPORTED)
#set_target_properties(ffmpeg
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libffmpeg.so)


# 添加x264
#add_library(x264 STATIC IMPORTED)
#set_target_properties(x264
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/src/main/cpp/x264/lib/libx264.a)


add_library(iduPush SHARED
        src/main/cpp/RSPush.cpp
        src/main/cpp/DoubleQueue.cpp
        )

find_library(log-lib log)

target_link_libraries(
        iduPush
        ${log-lib})