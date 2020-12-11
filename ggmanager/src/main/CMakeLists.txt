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

add_library(ggmanager SHARED ${SRC_LIST})

target_link_libraries(ggmanager
        android
        log
        z)