cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)

add_executable(hello_threading hello_threading.cpp)
add_executable(sharing_data sharing_data.cpp)
add_executable(synchronizing synchronizing.cpp)
add_executable(futures futures.cpp)
add_executable(promises promises.cpp)

if (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(hello_threading Threads::Threads)
    target_link_libraries(sharing_data Threads::Threads)
    target_link_libraries(synchronizing Threads::Threads)
    target_link_libraries(futures Threads::Threads)
    target_link_libraries(promises Threads::Threads)
endif()
