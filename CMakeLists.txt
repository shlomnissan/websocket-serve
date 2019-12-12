cmake_minimum_required(VERSION 3.10.2)
project(cpp_websocket_server)

set(CMAKE_CXX_STANDARD 17)
set(Boost_NO_BOOST_CMAKE ON)

find_package(Boost REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(websocket-serve main.cpp beast.hpp net.hpp library/listener.cpp library/listener.hpp library/shared_state.cpp library/shared_state.hpp library/web_socket_session.cpp library/web_socket_session.hpp library/http_session.cpp library/http_session.hpp)

target_link_libraries(websocket-serve PRIVATE Threads::Threads)