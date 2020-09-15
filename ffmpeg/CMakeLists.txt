cmake_minimum_required(VERSION 3.4.1)

# 添加include目录路径
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)

# 添加ffmpeg库
add_library(ffmpeg SHARED IMPORTED)
set_target_properties(ffmpeg
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libffmpeg.so)


add_library(ffmpegin SHARED
        src/main/cpp/RSFFmpeg.cpp
        src/main/cpp/FFUtil.cpp
        src/main/cpp/RtmpPlayer.cpp
        src/main/cpp/RtmpPlayerJNI.cpp
        src/main/cpp/H264SoftDecode.cpp
        src/main/cpp/FrameQueue.cpp
        )

find_library(log-lib log)

target_link_libraries(ffmpegin
        ffmpeg
        ${log-lib}
        z android OpenSLES
        )