cmake_minimum_required(VERSION 2.8)

project( ChessPieceDetector )

find_library(
    NETWORK_LIBRARY
    NAMES libNetworkLibrary.so
    HINTS ../lib_so
    REQUIRED)

message("Network library Path: ${NETWORK_LIBRARY}")

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR} SOURCES )

add_executable( ChessPieceDetector ${SOURCES} )

target_link_libraries( ChessPieceDetector ${OpenCV_LIBS} )
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_link_libraries(${PROJECT_NAME} ${NETWORK_LIBRARY})

SET(CMAKE_CXX_FLAGS -pthread)
