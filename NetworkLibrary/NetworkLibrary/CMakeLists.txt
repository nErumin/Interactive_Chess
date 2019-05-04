cmake_minimum_required(VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)

project( NetworkLibrary )
find_package( Boost REQUIRED system thread date_time regex serialization )
include_directories( ${Boost_INCLUDE_DIRS} )

message("Boost Library Path: ${Boost_LIBRARIES}")
message("Boost Include Header Path: ${Boost_INCLUDE_DIR}")

aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR} SOURCES )
add_executable( NetworkLibrary ${SOURCES} )

target_link_libraries( NetworkLibrary ${Boost_LIBRARIES} )
