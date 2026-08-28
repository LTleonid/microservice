cmake_minimum_required(VERSION 3.20)

project(cpp_worker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(RABBITMQ REQUIRED librabbitmq)

add_executable(cpp_worker
    src/main.cpp
    src/process.pb.cc
)

target_include_directories(cpp_worker PRIVATE
    ${Protobuf_INCLUDE_DIRS}
    ${RABBITMQ_INCLUDE_DIRS}
)

target_link_directories(cpp_worker PRIVATE
    ${RABBITMQ_LIBRARY_DIRS}
)

target_link_libraries(cpp_worker PRIVATE
    protobuf::libprotobuf
    ${RABBITMQ_LIBRARIES}
)