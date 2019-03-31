cmake_minimum_required(VERSION 2.8)

project( ChessBoardRecognizer )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR} SOURCES )

add_executable( ChessBoardRecognizer ${SOURCES} )

target_link_libraries( ChessBoardRecognizer ${OpenCV_LIBS} )

SET(CMAKE_CXX_FLAGS -pthread)
