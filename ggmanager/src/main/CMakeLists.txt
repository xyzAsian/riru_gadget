cmake_minimum_required(VERSION 3.4.1)
enable_language(ASM)
project("ggmanager")
include_directories(
        "jni"
)

SET(SRC_LIST
        "jni/main.cpp"
        "jni/cmd.cpp"
        "jni/sign_killer.cpp")

add_library(xyzggmanager SHARED ${SRC_LIST})

target_link_libraries(xyzggmanager
        android
        log
        z)