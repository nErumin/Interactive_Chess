cmake_minimum_required(VERSION 2.8)

project( NetworkLibrary )
find_package( Boost REQUIRED system thread date_time regex serialization asio  )
include_directories( ${BOOST_INCLUDE_DIRS} )

message("Boost Library Path: ${BOOST_LIBRARIES}")
message("Boost Include Header Path: ${BOOST_INCLUDE_DIR}")

aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR} SOURCES )
add_executable( NetworkLibrary ${SOURCES} )

target_link_libraries( NetworkLibrary ${BOOST_LIBRARIES} )