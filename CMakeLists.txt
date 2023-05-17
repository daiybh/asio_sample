cmake_minimum_required (VERSION 3.8)

#ADD_DEFINITIONS(-DUNICODE)
#ADD_DEFINITIONS(-DUNICODE)
#ADD_DEFINITIONS(-D_UNICODE)

project ("led_server" CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test)

find_package(asio CONFIG REQUIRED)


#add_executable(asio_httpserver asio_httpserver.cpp)
#target_link_libraries(asio_httpserver PRIVATE asio::asio)

#add_executable(asio_tcpProxy "asio_tcpProxy2.cpp")
#target_link_libraries(asio_tcpProxy PRIVATE asio::asio)

#add_executable(asio_tcpserver "asio_tcpServer_block.cpp")
add_executable(asio_tcpserver "asio_tcpServer_async.cpp")
target_link_libraries(asio_tcpserver PRIVATE asio::asio)
